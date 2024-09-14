#include "stdafx.h"
#include "..\Public\UIPage_Test.h"

#include "GameInstance.h"

CUIPage_Test::CUIPage_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Test::CUIPage_Test(const CUIPage_Test& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Test::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Test::Initialize(void* pArg)
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_UIPart();

	return S_OK;
}

void CUIPage_Test::Priority_Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_Test::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPage_Test::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIPage_Test::Render()
{

	return S_OK;
}

void CUIPage_Test::AddRender_UIPage_Test()
{
}

void CUIPage_Test::Ready_UIPart()
{
}

HRESULT CUIPage_Test::Ready_Components()
{


	return S_OK;
}

CUIPage_Test* CUIPage_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Test* pInstance = new CUIPage_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Test::Clone(void* pArg)
{
	CUIPage_Test* pInstance = new CUIPage_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Test::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
