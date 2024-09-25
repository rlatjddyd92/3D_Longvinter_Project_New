#include "stdafx.h"
#include "..\Public\UIPage_Inven.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Inven::CUIPage_Inven(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Inven::CUIPage_Inven(const CUIPage_Inven& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Inven::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Inven::Initialize(void* pArg)
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

	m_fSizeX = ((m_fInvenCellSize * 1.0f) * _float(m_iInvenCol)) + 20.f;
	m_fSizeY = ((m_fInvenCellSize * 1.0f) * _float(m_iInvenRow)) + 60.f;

	__super::SetOff(true);

	Ready_UIPart();

	return S_OK;
}

void CUIPage_Inven::Priority_Update(_float fTimeDelta)
{



	int a = 10;
}

void CUIPage_Inven::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_Inven::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < INVEN_COL * INVEN_ROW; ++i)
	{
		CItemManager::TINFO tInfo = GET_INSTANCE->GetInvenInfo(i);

		if (tInfo.eIndex == ITEMINDEX::ITEM_END)
			m_vecInvenCell[i]->Empty_Cell();
		else
			m_vecInvenCell[i]->Input_Item(_int(tInfo.eIndex));

		m_vecInvenCell[i]->Set_Picked(tInfo.bPicked);
	}




	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Inven::Render()
{

	return S_OK;
}

void CUIPage_Inven::AddRender_UIPage()
{
	
	

	

	m_pBack_Window->AddRender_UIPart();

	for (auto& iter : m_vecInvenCell)
		iter->AddRender_UIPart();

	m_pBack_Window_Header->AddRender_UIPart();
	m_pButton_Close->AddRender_UIPart();
}

void CUIPage_Inven::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeX / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

	m_vecInvenCell.resize(m_iInvenCol * m_iInvenRow);

	_float fStartX = m_fX - (m_fSizeX / 2) + 10 + (m_fInvenCellSize * 0.5f);
	_float fStartY = m_fY - (m_fSizeY / 2) + 50 + (m_fInvenCellSize * 0.5f);

	for (_int i = 0; i < m_iInvenRow; ++i)
		for (_int j = 0; j < m_iInvenCol; ++j)
		{
			m_vecInvenCell[(i * m_iInvenCol)+j] = GET_INSTANCE->MakeUIPart_Cell(CUIPart_Cell::CELL_INVEN, fStartX + (m_fInvenCellSize * 1.f * j), fStartY + (m_fInvenCellSize * 1.f * i), m_fInvenCellSize, m_fInvenCellSize);
		}
}

_bool CUIPage_Inven::Key_Action()
{

	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);

	_bool Check = false;

	if ((m_pBack_Window_Header->IsPushed()) && (!m_pBack_Window_Header->IsPressing()))
	{
		Check = true;

		m_bMoving = true;
		POINT			ptMouse{};
		GetCursorPos(&ptMouse);

		m_fBeforeX = ptMouse.x;
		m_fBeforeY = ptMouse.y;
	}
	else if ((m_bMoving) && (m_pBack_Window_Header->IsPressing()))
	{
		Check = true;

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
		for (auto& iter : m_vecInvenCell)
			iter->Move_UI(fMovingX, fMovingY);
	}
	else
	{
		_bool bInput = false;

		if (GET_INSTANCE->GetPickedItemIndex() != ITEMINDEX::ITEM_END)
			bInput = true;


		for (_int i = 0; i < INVEN_COL * INVEN_ROW; ++i)
			if (m_vecInvenCell[i]->IsPushed())
			{
				Check = true;

				if (bInput)
				{
					GET_INSTANCE->PutInItem(CItemManager::ARRAY_INVEN, i);
				}
				else
				{
					GET_INSTANCE->PickItem(CItemManager::ARRAY_INVEN, i);
					m_vecInvenCell[i]->Set_Picked(true);
				}
				
				break;
			}
	}
	

	return Check;
}



HRESULT CUIPage_Inven::Ready_Components()
{


	return S_OK;
}

CUIPage_Inven* CUIPage_Inven::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Inven* pInstance = new CUIPage_Inven(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Inven"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Inven::Clone(void* pArg)
{
	CUIPage_Inven* pInstance = new CUIPage_Inven(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Inven"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Inven::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& iter : m_vecInvenCell)
		Safe_Release(iter);

	m_vecInvenCell.clear();
}
