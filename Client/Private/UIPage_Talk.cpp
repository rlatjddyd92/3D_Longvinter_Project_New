#include "stdafx.h"
#include "..\Public\UIPage_Talk.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Talk::CUIPage_Talk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Talk::CUIPage_Talk(const CUIPage_Talk& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Talk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Talk::Initialize(void* pArg)
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

	m_fX = 800.f;
	m_fY = 400.f;

	m_fSizeX = (m_fGap * 2.f) + m_fFunc_X;
	m_fSizeY = (m_fGap * 5.f) + 30.f + (m_fFunc_Y * 3.f);

	__super::SetOff(true);

	Ready_UIPart();

	return S_OK;
}

void CUIPage_Talk::Priority_Update(_float fTimeDelta)
{
	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
		__super::SetOff(true);
	else
		__super::SetOn();

	int a = 10;
}

void CUIPage_Talk::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_Talk::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */


	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Talk::Render()
{







	return S_OK;
}

void CUIPage_Talk::AddRender_UIPage()
{




	m_pBack_Window->AddRender_UIPart();
	m_pBack_Window_Header->AddRender_UIPart();
	m_pButton_Close->AddRender_UIPart();

	m_pBack_Upper->AddRender_UIPart();
	m_pBack_Lower->AddRender_UIPart();

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPage_Talk::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeX / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

	_float fStartY = (m_fY - (m_fSizeY * 0.5)) + 30.f + (m_fGap * 2) + (m_fFunc_Y * 0.5);

	//m_pBack_Upper = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_SELECT, m_fX, fStartY + ((m_fFunc_Y + m_fGap) * i), m_fFunc_X, m_fFunc_Y);
	//m_pBack_Lower = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_SELECT, m_fX, fStartY + ((m_fFunc_Y + m_fGap) * i), m_fFunc_X, m_fFunc_Y);


}

_bool CUIPage_Talk::Key_Action()
{

	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);

	_int Check = 0;

	Check += m_pButton_Close->IsPushed();
	Check += m_pBack_Window->IsPushed();
	Check += m_pBack_Window_Header->IsPushed();

	for (_int i = 0; i < 3; ++i)
	{
		
	}


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



HRESULT CUIPage_Talk::Ready_Components()
{


	return S_OK;
}

CUIPage_Talk* CUIPage_Talk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Talk* pInstance = new CUIPage_Talk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Talk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Talk::Clone(void* pArg)
{
	CUIPage_Talk* pInstance = new CUIPage_Talk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Talk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Talk::Free()
{
	__super::Free();
	Safe_Release(m_pButton_Close);
	Safe_Release(m_pBack_Window);
	Safe_Release(m_pBack_Window_Header);
	Safe_Release(m_pBack_Upper);
	Safe_Release(m_pBack_Lower);

	

}
