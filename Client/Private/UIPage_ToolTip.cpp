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

	m_fBeforeX = m_fX;
	m_fBeforeY = m_fY;

	m_vecSize.resize(_int(TOOLTIP_TEXT::TEXT_END));

	m_fSizeX = 300.f;
	m_fSizeY = m_fGap_Top;

	m_vecSize[_int(TOOLTIP_TEXT::TEXT_ITEMNAME)] = { 20.f , 0.5f ,10.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_TYPE)] = { 15.f , 0.4f ,10.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_DESCRIPTION)] = { 10.f , 0.3f ,10.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_HP)] = { 10.f , 0.3f ,2.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_PRICE)] = { 10.f , 0.3f ,2.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_ATTACK)] = { 10.f , 0.3f ,2.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_DEFENCE)] = { 10.f , 0.3f ,2.f };
	m_vecSize[_int(TOOLTIP_TEXT::TEXT_HEAL)] = { 10.f , 0.3f ,10.f };

	for (_int i = _int(TOOLTIP_TEXT::TEXT_HEAL) + 1; i < _int(TOOLTIP_TEXT::TEXT_END); ++i)
		m_vecSize[i] = { 15.f , 0.3f ,2.f };

	for (_int i = 0; i < _int(TOOLTIP_TEXT::TEXT_TAG_1); ++i)
		m_fSizeY += m_vecSize[i].m_fSize + m_vecSize[i].m_fGap;

	//m_fSizeY -= m_vecSize[_int(TOOLTIP_TEXT::TEXT_TAG_1) - 1].m_fGap;

	m_fSizeY += m_fGap_Top;

	m_fSizeY_NoneTag = m_fSizeY;

	

	__super::SetOff(true);

	Ready_UIPart();

	return S_OK;
}

void CUIPage_ToolTip::Priority_Update(_float fTimeDelta)
{
	__super::SetOff(true);


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

	for (_int i = 0; i < _int(TOOLTIP_TEXT::TEXT_TAG_1); ++i)
		m_vecText[i]->AddRender_UIPart();

	for (_int i = _int(TOOLTIP_TEXT::TEXT_TAG_1); i < _int(TOOLTIP_TEXT::TEXT_TAG_1) + m_iTagNum; ++i)
		m_vecText[i]->AddRender_UIPart();
}

void CUIPage_ToolTip::Ready_UIPart()
{


	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_TOOLTIP, m_fX, m_fY, m_fSizeX, m_fSizeY);

	_float fStartY = (m_fY - m_fSizeY * 0.5f) + m_fGap_Top + m_vecSize[0].m_fSize * 0.5f;
	_float fSizeX = m_fSizeX - m_fGap_Left * 2.f;

	m_vecText.resize(_int(TOOLTIP_TEXT::TEXT_END));

	for (_int i = 0; i < _int(TOOLTIP_TEXT::TEXT_END); ++i)
	{
		m_vecText[i] = GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_NORMAL, m_fX, fStartY, fSizeX, m_vecSize[i].m_fSize, false);
		m_vecText[i]->SetSize(m_vecSize[i].m_fTextScale);


		if ((i == _int(TOOLTIP_TEXT::TEXT_ITEMNAME)) || (i == _int(TOOLTIP_TEXT::TEXT_TYPE)))
			m_vecText[i]->SetFont(TEXT("Font_Test1"));
		if ((i >= _int(TOOLTIP_TEXT::TEXT_TAG_1)) || (i <= _int(TOOLTIP_TEXT::TEXT_TAG_10)))
			m_vecText[i]->SetFont(TEXT("Font_Test3"));
		
		fStartY += m_vecSize[i].m_fSize + m_vecSize[i].m_fGap;
	}




}

_bool CUIPage_ToolTip::Key_Action()
{


	return true;
}

void CUIPage_ToolTip::ShowToolTip(_float fCellX, _float fCellY, ITEMARRAY eArray, _int iIndex)
{
	CItemManager::ItemInfo tInfo{};

	if (eArray == ITEMARRAY::ARRAY_INVEN)
		tInfo = GET_INSTANCE->GetInvenInfo(iIndex);
	if (eArray == ITEMARRAY::ARRAY_EQUIP)
		tInfo = GET_INSTANCE->GetEquipInfo(EQUIPSLOT(iIndex));

	_int iNow = _int(TOOLTIP_TEXT::TEXT_TAG_1);

	for (_int i = 0; i < _int(ITEMTAG::ITEM_TAG_END); ++i)
		if (GET_INSTANCE->Get_TagState(tInfo.eIndex, ITEMTAG(i)))
		{
			m_vecText[iNow]->SetText(GET_INSTANCE->Get_TagName(ITEMTAG(i)));
			++iNow;
			if (iNow >= m_vecText.size())
				break;
		}

	m_fSizeY = m_fSizeY_NoneTag;
	m_iTagNum = iNow - _int(TOOLTIP_TEXT::TEXT_TAG_1);

	for (_int i = _int(TOOLTIP_TEXT::TEXT_TAG_1); i < iNow; ++i)
	{
		m_fSizeY += m_vecSize[i - 1].m_fGap;
		m_fSizeY += m_vecSize[i].m_fSize;
	}

	m_pBack_Window->Set_UISize(m_fSizeX, m_fSizeY);



	fCellX += 3.f + (m_fSizeX * 0.5f) + INVEN_CELLSIZE;
	_float fBackY = fCellY + (m_fSizeY * 0.5f);
	fCellY += (m_fSizeY_NoneTag * 0.5f);
	

	_float fMovingX = fCellX - m_fX;
	_float fMovingY = fCellY - m_fY;
	_float fMovingY_Back = fBackY - m_fY;

	m_pBack_Window->Set_UIPosition(m_fX, m_fY);
	__super::Move_UI(fMovingX, fMovingY);
	m_pBack_Window->Move_UI(fMovingX, fMovingY_Back);

	for (auto& iter : m_vecText)
		iter->Move_UI(fMovingX, fMovingY);
		
	
	
	m_vecText[_int(TOOLTIP_TEXT::TEXT_ITEMNAME)]->SetText(tInfo.strItemName);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_TYPE)]->SetText(GET_INSTANCE->Get_TypeName(tInfo.eType));
	m_vecText[_int(TOOLTIP_TEXT::TEXT_DESCRIPTION)]->SetText(tInfo.strItemDescription);

	wstring strText = TEXT("내구도 | ") + to_wstring(tInfo.fHp);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_HP)]->SetText(strText);
	 strText = TEXT("판매가 | ") + to_wstring(tInfo.iPrice);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_PRICE)]->SetText(strText);
	 strText = TEXT("공격력 | ") + to_wstring(tInfo.fAttack);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_ATTACK)]->SetText(strText);
	 strText = TEXT("방어력 | ") + to_wstring(tInfo.fDefence);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_DEFENCE)]->SetText(strText);
	 strText = TEXT("회복력 | ") + to_wstring(tInfo.fHeal);
	m_vecText[_int(TOOLTIP_TEXT::TEXT_HEAL)]->SetText(strText);
	
	


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
	m_vecSize.clear();
}
