#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "ClientInstance.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	/* ���� ���۷��� ī��Ʈ�� ������Ű�� ���� : */
	/* ���� �����ϱ����� �����Ϳ� ������ �Ҷ��� ������Ű�� �ʴ´�. */
	/* ���� ������ ��ġ�� �ִ� �ּҸ� �ٸ� �������������� ������ �ϰԵǸ� �̶��� ������Ų��. */
	Safe_AddRef(m_pGameInstance);	
}

HRESULT CMainApp::Initialize()
{	
	ENGINE_DESC			EngineDesc{};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowsed = true;

	/* ������ ����ϱ����� ���� �ʱ�ȭ ������ �����Ѵ�. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, _uint(LEVELID::LEVEL_END), EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	GET_INSTANCE->Setting_Program_Start(&m_pDevice, &m_pContext, m_pGameInstance);

	/*m_pContext->RSSetState();
	m_pContext->OMSetDepthStencilState();

	D3D11_BLEND_DESC		bLENDdEESC;
	ID3D11BlendState*		pBlendState;
	m_pDevice->CreateBlendState();
	m_pContext->OMSetBlendState(pBlendState);*/

	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVELID::LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);
	m_fFPS = fTimeDelta;
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw_Engine();

	m_fFPS = 1.f / m_fFPS;
	_int iFPS = m_fFPS / 1;
	_tchar* tTemp = new _tchar[5];

	swprintf(tTemp, 5, L"%d", iFPS);
	
	XMVECTOR vFPSColor = { 0.f,0.f,0.f,1.f };

	if (iFPS < 30)
		vFPSColor = { 220.f / 255.f,20.f / 255.f,60.f / 255.f,1.f };

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), tTemp, _vector{ 1200.f,100.f,0.f,0.f }, 0.8f, false, vFPSColor);

	m_pGameInstance->Render_End();

	Safe_Delete_Array(tTemp);

	

	return S_OK;
}


HRESULT CMainApp::Ready_Prototype_Component_Static()
{


	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevelID)
{	
	/* � ������ �����ϴ� �ε� ������ �� �����Ѵ�.  */
	/* �ε��������� ���� ������ ������ �ʿ��� �ڿ��� �غ��Ѵ�. */
	/* �ε� ������ ��������(���� ������ ����)�� ���������� �����ؾ��Ѵ�. */
	if (FAILED(GET_INSTANCE->ChangeLevel(eStartLevelID)))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	GET_INSTANCE->Release_CilentInstance();
	m_pGameInstance->Release_Engine();

	while (m_pGameInstance)
		Safe_Release(m_pGameInstance);
}

