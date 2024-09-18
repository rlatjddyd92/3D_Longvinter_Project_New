#include "stdafx.h"
#include "..\Public\UIPage_Option.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Option::CUIPage_Option(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Option::CUIPage_Option(const CUIPage_Option& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Option::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Option::Initialize(void* pArg)
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

void CUIPage_Option::Priority_Update(_float fTimeDelta)
{



	int a = 10;
}

void CUIPage_Option::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_Option::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Option::Render()
{

	return S_OK;
}

void CUIPage_Option::AddRender_UIPage()
{
	m_pButton_Close->AddRender_UIPart();
	m_pBack_Window_Header->AddRender_UIPart();
	m_pBack_Window->AddRender_UIPart();
}

void CUIPage_Option::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeX / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

}

_bool CUIPage_Option::Key_Action()
{

	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);

	_int Check = 0;

	Check += m_pButton_Close->IsPushed();
	Check += m_pBack_Window->IsPushed();
	Check += m_pBack_Window_Header->IsPushed();

	if (Check == 0)
		return false;

	if ((m_pBack_Window_Header->IsPushed()) && (!m_pBack_Window_Header->IsPressing()))
	{
		m_bMoving = true;
		POINT			ptMouse{};
		GetCursorPos(&ptMouse);

		m_fBeforeX = ptMouse.x;
		m_fBeforeY = ptMouse.y;
	}
	if ((m_bMoving) && (m_pBack_Window_Header->IsPressing()))
	{
		POINT			ptMouse{};
		GetCursorPos(&ptMouse);

		_float fMovingX = ptMouse.x - m_fBeforeX;
		_float fMovingY = ptMouse.y - m_fBeforeY;

		m_fBeforeX = ptMouse.x;
		m_fBeforeY = ptMouse.y;

		__super::Move_UI(fMovingX, fMovingY);
		m_pButton_Close->Move_UI(fMovingX, fMovingY);
		m_pBack_Window_Header->Move_UI(fMovingX, fMovingY);
		m_pBack_Window->Move_UI(fMovingX, fMovingY);
	}

	return true;
}



HRESULT CUIPage_Option::Ready_Components()
{


	return S_OK;
}

CUIPage_Option* CUIPage_Option::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Option* pInstance = new CUIPage_Option(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Option"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Option::Clone(void* pArg)
{
	CUIPage_Option* pInstance = new CUIPage_Option(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Option"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Option::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
