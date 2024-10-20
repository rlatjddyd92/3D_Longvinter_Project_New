#include "stdafx.h"
#include "..\Public\UIPage_Hack.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Hack::CUIPage_Hack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Hack::CUIPage_Hack(const CUIPage_Hack& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Hack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Hack::Initialize(void* pArg)
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

void CUIPage_Hack::Priority_Update(_float fTimeDelta)
{
	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
		__super::SetOff(true);
	else
		__super::SetOn();

	int a = 10;
}

void CUIPage_Hack::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_Hack::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	

	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Hack::Render()
{
	for (_int i = 0; i < 3; ++i)
	{
		_float fX = 0.f;
		_float fY = 0.f;

		if (!m_pHost->GetHack(i))
		{
			m_vecBack_Func[i]->Get_UIPosition(&fX, &fY);
			fX -= m_fFunc_X * 0.5f;
			fY -= m_fFunc_Y * 0.5f;

			fX += 10.f;
			fY += 10.f;

			if (i == 0)
			{
				m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("While(true){int i = 0;} 추가하기"), { fX , fY, 0.f,0.f }, 0.8f);
				fX += 20;
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("무한반복문으로 터렛을 정지시킵니다."), { fX , fY + 30.f, 0.f,0.f }, 0.3f);
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("몬스터를 터렛으로 유인합니다.(제작 중)"), { fX , fY + 50.f, 0.f,0.f }, 0.3f, false, { 1.f ,0.f, 0.f,0.f });
			}
			if (i == 1)
			{
				m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("if문에 느낌표 추가하기"), { fX , fY, 0.f,0.f }, 0.8f);
				fX += 20;
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("분기문을 반대로 작동시킵니다."), { fX , fY + 30.f, 0.f,0.f }, 0.3f);
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("터렛이 주변의 몬스터를 공격합니다."), { fX , fY + 50.f, 0.f,0.f }, 0.3f, false, { 1.f ,0.f, 0.f,0.f });
			}
			if (i == 2)
			{
				m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("Release 함수 내용 삭제"), { fX , fY, 0.f,0.f }, 0.8f);
				fX += 20;
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("끔찍한 메모리 누수를 일으킵니다."), { fX , fY + 30.f, 0.f,0.f }, 0.3f);
				m_pGameInstance->Render_Text(TEXT("Font_Test3"), TEXT("터렛이 곧 폭발합니다."), { fX , fY + 50.f, 0.f,0.f }, 0.3f, false, { 1.f ,0.f, 0.f,0.f });
			}
		}
		else
		{
			m_vecBack_Func[i]->Get_UIPosition(&fX, &fY);
			m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("해킹 완료"), { fX , fY, 0.f,0.f }, 2.f, true, { 1.f ,0.f, 0.f,0.f });
		}
	}






	








	return S_OK;
}

void CUIPage_Hack::AddRender_UIPage()
{
	

	
	
	m_pBack_Window->AddRender_UIPart();
	m_pBack_Window_Header->AddRender_UIPart();
	m_pButton_Close->AddRender_UIPart();


	for (auto& iter : m_vecBack_Func)
		iter->AddRender_UIPart();

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPage_Hack::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeX / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

	m_vecBack_Func.resize(3);

	_float fStartY = (m_fY - (m_fSizeY * 0.5)) + 30.f + (m_fGap *2) + (m_fFunc_Y * 0.5);
	

	for (_int i = 0; i < 3; ++i)
		m_vecBack_Func[i] = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_SELECT, m_fX, fStartY + ((m_fFunc_Y + m_fGap) * i), m_fFunc_X, m_fFunc_Y);


	
}

_bool CUIPage_Hack::Key_Action()
{

	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);

	_int Check = 0;

	Check += m_pButton_Close->IsPushed();
	Check += m_pBack_Window->IsPushed();
	Check += m_pBack_Window_Header->IsPushed();

	for (_int i = 0; i < 3; ++i)
	{
		if (m_vecBack_Func[i]->IsPushed())
		{
			++Check;

			if (m_pHost != nullptr)
				if (!m_pHost->GetHack(i))
					m_pHost->SetHack(i);
		}
		
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



HRESULT CUIPage_Hack::Ready_Components()
{


	return S_OK;
}

CUIPage_Hack* CUIPage_Hack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Hack* pInstance = new CUIPage_Hack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Hack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Hack::Clone(void* pArg)
{
	CUIPage_Hack* pInstance = new CUIPage_Hack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Hack"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Hack::Free()
{
	__super::Free();
	Safe_Release(m_pButton_Close);
	Safe_Release(m_pBack_Window);
	Safe_Release(m_pBack_Window_Header);

	for (auto& iter : m_vecBack_Func)
		Safe_Release(iter);

	m_vecBack_Func.clear();

}
