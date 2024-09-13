#include "stdafx.h"
#include "..\Public\Level_Editor.h"

#include "FreeCamera.h"
#include "GameInstance.h"
#include "ClientInstance.h"

CLevel_Editor::CLevel_Editor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Editor::Initialize()
{
	GET_INSTANCE->Setting_Editor_Start();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Effect()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Monster()))
		return E_FAIL;
	if (FAILED(Ready_ImguiManager()))
		return E_FAIL;



	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Editor::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Editor::Render()
{
	SetWindowText(g_hWnd, TEXT("에디터입니다."));
	return S_OK;
}

HRESULT CLevel_Editor::Ready_Lights()
{
	/* 게임플레이 레벨에 필요한 광원을 준비한다. */


	return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_Camera()
{


	return S_OK;
}


HRESULT CLevel_Editor::Ready_Layer_BackGround()
{

	return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_Effect()
{


	return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_Monster()
{



	return S_OK;
}

HRESULT CLevel_Editor::Ready_LandObjects()
{

	return S_OK;
}

HRESULT CLevel_Editor::Ready_ImguiManager()
{


	return S_OK;
}


CLevel_Editor* CLevel_Editor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Editor* pInstance = new CLevel_Editor(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Editor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Editor::Free()
{
	__super::Free();

}
