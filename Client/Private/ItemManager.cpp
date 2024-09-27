#include "stdafx.h"
#include "ItemManager.h"
#include "ClientInstance.h"

CItemManager::CItemManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CItemManager::CItemManager(const CItemManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CItemManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemManager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Setting_ItemInfo();

	return S_OK;
}

void CItemManager::Priority_Update(_float fTimeDelta)
{
}

void CItemManager::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIMK_RBUTTON))
		CancelPick();
}

void CItemManager::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CItemManager::Render()
{
	while (!m_Render_Model_list.empty())
	{
		RENDERINFO tTemp = m_Render_Model_list.front();
		m_Render_Model_list.pop_front();

		if (tTemp.eIndex == ITEMINDEX::ITEM_END)
			continue;

		if (m_vecTool[_int(tTemp.eIndex)] == nullptr)
			continue;

		m_vecTool[_int(tTemp.eIndex)]->SetParentState(tTemp.pParentState);
		m_vecTool[_int(tTemp.eIndex)]->SetSocketMatrix(tTemp.pSocketMatrix);
		m_vecTool[_int(tTemp.eIndex)]->SetRenderMatrix(tTemp.pParentMatrix);

		m_vecTool[_int(tTemp.eIndex)]->Render();
	}

	m_Render_Model_list.clear();

	return S_OK;
}

HRESULT CItemManager::Setting_ItemInfo()
{
	m_vecItemInfo.resize(_int(ITEMINDEX::ITEM_END));
	m_vecInvenInfo.resize(INVEN_COL * INVEN_ROW);
	m_vecEquipInfo.resize(SLOT_END);
	m_vecItemInvenTexture.resize(_int(ITEMINDEX::ITEM_END));
	m_vecTool.resize(_int(ITEMINDEX::ITEM_END));


	_uint iTemp = 0;
	CTool::TOOL_DESC		ToolDesc{};
	ToolDesc.pParentState = &iTemp;
	ToolDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	// 탄환
	m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)] =
	{ ITEMINDEX::ITEM_AMMO, TEXT("Gun_Ammo"), TEXT("Ammo"), false,  ITEMTYPE::ITEM_TYPE_AMMO, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Ammo"),
		TEXT("Com_Texture_0"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_AMMO)]))))
		return E_FAIL;
	
	// 샷건 키트
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)] =
	{ ITEMINDEX::ITEM_SHOTGUN_KIT, TEXT("ShotGun_Kit"), TEXT("Kit"), false, ITEMTYPE::ITEM_TYPE_ETC, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun_Kit"),
		TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)]))))
		return E_FAIL;
	
	// 샷건
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)] =
	{ ITEMINDEX::ITEM_SHOTGUN, TEXT("ShotGun"), TEXT("Weapon"), true, ITEMTYPE::ITEM_TYPE_WEAPON, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun"),
		TEXT("Com_Texture_2"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN)]))))
		return E_FAIL;
	
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_ShotGun"), TEXT("Prototype_GameObject_Tool_ShotGun"), &ToolDesc);
	m_vecTool[_int(ITEMINDEX::ITEM_SHOTGUN)] = static_cast<CTool*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_ShotGun"), -1));
	
	
	// 인벤 초기 세팅 

	m_vecInvenInfo[0] = m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)];
	m_vecInvenInfo[1] = m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)];
	m_vecInvenInfo[2] = m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)];

	return S_OK;
}

const CItemManager::TINFO CItemManager::PickItem(ITEMARRAY eArray, _int iIndex)
{
	TINFO tFail;

	if ((eArray == ITEMARRAY::ARRAY_INFO) && (iIndex >= _int(ITEMINDEX::ITEM_END)))
		return tFail;
	if ((eArray == ITEMARRAY::ARRAY_INVEN) && (iIndex >= _int(INVEN_COL * INVEN_ROW)))
		return tFail;
	if ((eArray == ITEMARRAY::ARRAY_EQUIP) && (iIndex >= _int(SLOT_END)))
		return tFail;

	m_eBeforeArray = eArray;
	m_iBeforeIndex = iIndex;
	
	if (eArray == ITEMARRAY::ARRAY_INFO)
	{
		m_vecItemInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecItemInfo[iIndex];
		return m_vecItemInfo[iIndex];
	}
		
	if (eArray == ITEMARRAY::ARRAY_INVEN)
	{
		m_vecInvenInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecInvenInfo[iIndex];
		return m_vecInvenInfo[iIndex];
	}
		
	if (eArray == ITEMARRAY::ARRAY_EQUIP)
	{
		m_vecEquipInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecEquipInfo[iIndex];
		return m_vecEquipInfo[iIndex];
	}
		

	// 상점 관련 사항 추가 필요 
}

void CItemManager::CancelPick()
{
	if (m_tPickedItem.eIndex != ITEMINDEX::ITEM_END)
	{
		if (m_eBeforeArray == ITEMARRAY::ARRAY_INFO)
		{
			m_vecItemInfo[m_iBeforeIndex].bPicked = false;
		}

		if (m_eBeforeArray == ITEMARRAY::ARRAY_INVEN)
		{
			m_vecInvenInfo[m_iBeforeIndex].bPicked = false;
		}

		if (m_eBeforeArray == ITEMARRAY::ARRAY_EQUIP)
		{
			m_vecEquipInfo[m_iBeforeIndex].bPicked = false;
		}

		m_tPickedItem.eIndex = ITEMINDEX::ITEM_END;
		m_eBeforeArray = ITEMARRAY::ARRAY_END;
		m_iBeforeIndex = -1;
	}
}

HRESULT CItemManager::PutInItem(ITEMARRAY eArray, _int iIndex)
{
	if (m_tPickedItem.eIndex == ITEMINDEX::ITEM_END)
		return E_FAIL;

	if ((m_eBeforeArray == eArray) && (m_iBeforeIndex == iIndex))
	{
		CancelPick();
		return E_FAIL;
	}
		

	if (eArray == ITEMARRAY::ARRAY_INVEN)
	{
		_bool bPlus = false;
		if (m_tPickedItem.bStack)
		{
			for (auto& iter : m_vecInvenInfo)
				if (iter.eIndex == m_tPickedItem.eIndex)
				{
					iter.iCount += m_tPickedItem.iCount;
					CancelPick();
					bPlus = true;
					break;
				}
		}
			
		if ((!m_tPickedItem.bStack) || (!bPlus))
		{
			TINFO tTemp = m_tPickedItem;
			_int iBefore = m_iBeforeIndex;
			ITEMARRAY eBefore = m_eBeforeArray;
			PickItem(eArray, iIndex);

			if (eBefore == ITEMARRAY::ARRAY_INVEN)
				if (m_vecInvenInfo[iBefore].bPicked)
					m_vecInvenInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

			if (eBefore == ITEMARRAY::ARRAY_EQUIP)
				if (m_vecEquipInfo[iBefore].bPicked)
					m_vecEquipInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

			m_vecInvenInfo[iIndex] = tTemp;
			m_vecInvenInfo[iIndex].bPicked = false;
		}
	}
	if (eArray == ITEMARRAY::ARRAY_EQUIP)
	{
		if (!m_tPickedItem.bIsTool)
		{
			CancelPick();
			return E_FAIL;
		}
			

		TINFO tTemp = m_tPickedItem;
		_int iBefore = m_iBeforeIndex;
		ITEMARRAY eBefore = m_eBeforeArray;
		PickItem(eArray, iIndex);

		if (eBefore == ITEMARRAY::ARRAY_INVEN)
			if (m_vecInvenInfo[iBefore].bPicked)
				m_vecInvenInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

		if (eBefore == ITEMARRAY::ARRAY_EQUIP)
			if (m_vecEquipInfo[iBefore].bPicked)
				m_vecEquipInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;
		m_vecEquipInfo[iIndex] = tTemp;
		m_vecEquipInfo[iIndex].bPicked = false;
	}


	return S_OK;
}

CItemManager* CItemManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemManager* pInstance = new CItemManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItemManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemManager::Clone(void* pArg)
{
	CItemManager* pInstance = new CItemManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItemManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemManager::Free()
{
	__super::Free();

	m_vecItemInfo.clear();
	m_vecInvenInfo.clear();

	m_vecInvenSelectBuffer.clear();
	m_vecEquipInfo.clear();

	m_vecTool.clear();

	for (auto& iter : m_vecItemInvenTexture)
		Safe_Release(iter);
	m_vecItemInvenTexture.clear();

	for (auto& iter : m_Render_Model_list)
	{
		Safe_Delete(iter.pParentState);
		Safe_Delete(iter.pSocketMatrix);
	}
		
	m_Render_Model_list.clear();
	
}
