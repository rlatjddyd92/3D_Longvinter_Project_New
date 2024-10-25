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

	srand(time(NULL)); // <- ���� �õ� ���� 

	/* ������ ����ϱ����� ���� �ʱ�ȭ ������ �����Ѵ�. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, _uint(LEVELID::LEVEL_END), EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Gara()))
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

	if (m_pGameInstance->Get_DIKeyState(DIK_0) & 0x80)
	{
		if (m_bClip)
		{
			ClipCursor(NULL);
			m_bClip = false;
		}
		else
		{
			HWND hwnd = GetForegroundWindow();  // ���� Ȱ��ȭ�� �����츦 ������
			RECT windowRect;

			// �������� ��ǥ�� ũ�⸦ �����´�
			if (GetWindowRect(hwnd, &windowRect)) {
				// ������ ���η� ���콺 Ŀ���� ����
				ClipCursor(&windowRect);
			}

			m_bClip = true;
		}
			
	}
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw_Engine();

	/*m_fFPS = 1.f / m_fFPS;
	_int iFPS = m_fFPS / 1;
	_tchar* tTemp = new _tchar[5];

	swprintf(tTemp, 5, L"%d", iFPS);
	
	XMVECTOR vFPSColor = { 0.f,0.f,0.f,1.f };

	if (iFPS < 30)
		vFPSColor = { 220.f / 255.f,20.f / 255.f,60.f / 255.f,1.f };

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), tTemp, _vector{ 1200.f,100.f,0.f,0.f }, 0.8f, false, vFPSColor);*/

	m_pGameInstance->Render_End();

	//Safe_Delete_Array(tTemp);

	

	return S_OK;
}


HRESULT CMainApp::Ready_Gara()
{
	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc{};
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;
	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = 0;

	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint* pPixel = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	D3D11_SUBRESOURCE_DATA	InitialDesc{};
	InitialDesc.pSysMem = pPixel;
	InitialDesc.SysMemPitch = TextureDesc.Width * sizeof(_uint);

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialDesc, &pTexture2D)))
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubResource);

	/* a b g r */
	for (size_t i = 0; i < 256; i++)
	{
		for (size_t j = 0; j < 256; j++)
		{
			_uint iIndex = i * 256 + j;

			if (j < 128)
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);
			else
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	m_pContext->Unmap(pTexture2D, 0);

	Safe_Delete_Array(pPixel);
	Safe_Release(pTexture2D);

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

