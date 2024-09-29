#include "stdafx.h"
#include "..\Public\UIPage_ToolTip.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_ToolTip::CUIPage_ToolTip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_ToolTip::CUIPage_ToolTip(const CUIPage_ToolTip& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_ToolTip::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_ToolTip::Initialize(void* pArg)
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

	m_fX = 400.f;
	m_fY = 300.f;

	m_fSizeX = 300.f;
	m_fSizeY = 300.f;

	__super::SetOff(true);

	Ready_UIPart();

	return S_OK;
}

void CUIPage_ToolTip::Priority_Update(_float fTimeDelta)
{



	int a = 10;
}

void CUIPage_ToolTip::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_ToolTip::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_ToolTip::Render()
{

	return S_OK;
}

void CUIPage_ToolTip::AddRender_UIPage()
{
	m_pBack_Window->AddRender_UIPart();
}

void CUIPage_ToolTip::Ready_UIPart()
{
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
}

_bool CUIPage_ToolTip::Key_Action()
{

	_int Check = 0;

	Check += m_pBack_Window->IsPushed();
	
	if (Check == 0)
		return false;

	return true;
}

void CUIPage_ToolTip::ShowInfo(CItemManager::ItemInfo& tInfo)
{




}



HRESULT CUIPage_ToolTip::Ready_Components()
{


	return S_OK;
}

CUIPage_ToolTip* CUIPage_ToolTip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_ToolTip* pInstance = new CUIPage_ToolTip(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_ToolTip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_ToolTip::Clone(void* pArg)
{
	CUIPage_ToolTip* pInstance = new CUIPage_ToolTip(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_ToolTip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_ToolTip::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
