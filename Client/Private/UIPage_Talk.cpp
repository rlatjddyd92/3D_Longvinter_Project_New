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

	m_fX = 300.f;
	m_fY = 300.f;

	m_fSizeX = (m_fGap * 2.f) + m_fFunc_X;
	m_fSizeY = (m_fGap * 4.f) + 30.f + m_fFunc_Y + m_fFunc_Y;

	__super::SetOff(true);

	Ready_UIPart();

	Ready_Script();

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
	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
		return;


	int a = 10;
}

void CUIPage_Talk::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */


	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Talk::Render()
{
	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
		return S_OK;

	_float fX = 0.f;
	_float fY = 0.f;

	m_pBack_Window->Get_UIPosition(&fX, &fY);

	 fX -= (m_fSizeX * 0.4f) + m_fGap;
	 fY -= 30.f;

	_int iNPC = _int(m_pHost->Get_NPC_Type());
	_int iScript = m_pHost->Get_ScriptNum();

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), m_vecScript[iNPC][iScript]->pName, {fX , fY, 0.f,0.f}, 0.7f, false, {0.f ,0.f, 0.f,1.f});

	m_pGameInstance->Render_Text(TEXT("Font_Test3"), m_vecScript[iNPC][iScript]->pScript, { fX , fY + 25.f, 0.f,0.f }, 0.35f, false, { 0.f ,0.f, 0.f,1.f });

	m_pBack_Select->Get_UIPosition(&fX, &fY);
	fX -= (m_fSizeX * 0.4f) + m_fGap;
	fY -= 10.f;

	m_pGameInstance->Render_Text(TEXT("Font_Test3"), m_vecScript[iNPC][iScript]->pSelect, { fX , fY, 0.f,0.f }, 0.35f, false, { 1.f ,1.f, 1.f,1.f });

	return S_OK;
}

void CUIPage_Talk::AddRender_UIPage()
{




	m_pBack_Window->AddRender_UIPart();
	m_pBack_Window_Header->AddRender_UIPart();
	m_pButton_Close->AddRender_UIPart();

	m_pBack_Select->AddRender_UIPart();

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPage_Talk::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeX / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

	_float fStartY = (m_fY + (m_fSizeY * 0.5)) - m_fGap - (m_fFunc_Y * 0.5f);
 
	m_pBack_Select = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_SELECT, m_fX, fStartY, m_fFunc_X, m_fFunc_Y);

}

_bool CUIPage_Talk::Key_Action()
{

	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);

	_int Check = 0;

	Check += m_pButton_Close->IsPushed();
	Check += m_pBack_Window->IsPushed();
	Check += m_pBack_Window_Header->IsPushed();

	if ((m_pBack_Select->IsPushed()) && (!m_pBack_Select->IsPressing()))
	{
		++Check;

		if (m_pHost != nullptr)
			m_pHost->Add_ScriptNum(1);

		if (m_pHost->Get_ScriptNum() >= m_vecScript[_int(m_pHost->Get_NPC_Type())].size())
			__super::SetOff(true);
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
		m_pBack_Select->Move_UI(fMovingX, fMovingY);
	
	}

	return true;
}



HRESULT CUIPage_Talk::Ready_Components()
{


	return S_OK;
}

void CUIPage_Talk::Ready_Script()
{
	m_vecScript.resize(_int(NPC_TYPE::NPC_END));
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)].resize(4);

	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pName = TEXT("???");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pScript = TEXT("안녕하세요! 무료 나눔을 부탁해요.");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][0]->pSelect = TEXT("무엇을 나누어 드릴까요?");


	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pName = TEXT("도적단 인턴");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pScript = TEXT("니가 가진 모든 거요");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][1]->pSelect = TEXT("혹시 강도이신가요?");

	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pName = TEXT("도적단 인턴");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pScript = TEXT("네! 강도단 인턴이에요.");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][2]->pSelect = TEXT("저도 총은 있어요!");

	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pName = TEXT("도적단 인턴");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pScript = TEXT("괜찮아요 곧 선배님들이 무료나눔을 받으러 오실 거에요");
	m_vecScript[_int(NPC_TYPE::NPC_INTERN)][3]->pSelect = TEXT("[닫기]");



	m_vecScript[_int(NPC_TYPE::NPC_SHOP)].resize(4);

	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pName = TEXT("상점 주인");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pScript = TEXT("물건 좀 전달해줄래요?");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][0]->pSelect = TEXT("무엇을 전달할까요?");

	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pName = TEXT("상점 주인");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pScript = TEXT("이 주소로 고기를 전달해주세요");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][1]->pSelect = TEXT("그럼 뭘 줄거에요?");

	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pName = TEXT("상점 주인");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pScript = TEXT("꼭 뭘 줘야 해요?");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][2]->pSelect = TEXT("님 클래스 CMonster로 바꿀래요?");

	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pName = TEXT("상점 주인");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pScript = TEXT("전달 받은 사람이 10,000원을 줄 거에요");
	m_vecScript[_int(NPC_TYPE::NPC_SHOP)][3]->pSelect = TEXT("[퀘스트 수락] 알겠어요!");

	


	m_vecScript[_int(NPC_TYPE::NPC_LAST)].resize(5);

	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pName = TEXT("육식주의자");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pScript = TEXT("주문했던 고기네요! 고마워요");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][0]->pSelect = TEXT("Mission을 완수했으니 Reward를 주세요");

	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pName = TEXT("육식주의자");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pScript = TEXT("흑흑....ㅠㅠㅠ");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][1]->pSelect = TEXT("????");

	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pName = TEXT("육식주의자");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pScript = TEXT("흑흑흑흑....ㅠㅠㅠㅠㅠㅠㅠㅠ");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][2]->pSelect = TEXT("갑자기 왜 울어요?");

	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pName = TEXT("진상");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pScript = TEXT("흑흑....맛있었다. 오늘 밥은.");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][3]->pSelect = TEXT("너의 수명은 오늘까지인가봐요!");

	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4] = new SCRIPT;
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pName = new _tchar[10];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pScript = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pSelect = new _tchar[100];
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pName = TEXT("진상");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pScript = TEXT("함께 폭사해요!");
	m_vecScript[_int(NPC_TYPE::NPC_LAST)][4]->pSelect = TEXT("!!!!!!!");


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
	Safe_Release(m_pBack_Select);

	for (auto& iter : m_vecScript)
	{
		for (auto& iterScript : iter)
		{
			Safe_Delete_Array(iterScript->pName);
			Safe_Delete_Array(iterScript->pScript);
			Safe_Delete_Array(iterScript->pSelect);
			Safe_Delete(iterScript);
		}
		
		iter.clear();
	}

	m_vecScript.clear();

}
