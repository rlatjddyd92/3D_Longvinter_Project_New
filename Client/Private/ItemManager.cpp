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
}

void CItemManager::Late_Update(_float fTimeDelta)
{
}

HRESULT CItemManager::Render()
{
	return S_OK;
}

HRESULT CItemManager::Setting_ItemInfo()
{
	m_vecItemInfo.resize(_int(ITEMINDEX::ITEM_END));
	m_vecInvenInfo.resize(INVEN_COL * INVEN_ROW);
	m_vecItemInvenTexture.resize(_int(ITEMINDEX::ITEM_END));
	m_vecTool.resize(_int(ITEMINDEX::ITEM_END));



	// ÅºÈ¯
	m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)] =
	{ ITEMINDEX::ITEM_AMMO, TEXT("Gun_Ammo"), TEXT("Ammo"), false,  ITEMTYPE::ITEM_TYPE_AMMO, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Ammo"),
		TEXT("Com_Texture_0"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_AMMO)]))))
		return E_FAIL;
	
	// ¼¦°Ç Å°Æ®
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)] =
	{ ITEMINDEX::ITEM_SHOTGUN_KIT, TEXT("ShotGun_Kit"), TEXT("Kit"), false, ITEMTYPE::ITEM_TYPE_ETC, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun_Kit"),
		TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)]))))
		return E_FAIL;
	
	// ¼¦°Ç
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)] =
	{ ITEMINDEX::ITEM_SHOTGUN, TEXT("ShotGun"), TEXT("Weapon"), false, ITEMTYPE::ITEM_TYPE_WEAPON, false, 100.f, 100, 100.f, 1.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun"),
		TEXT("Com_Texture_2"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN)]))))
		return E_FAIL;
	


	// ÀÎº¥ ÃÊ±â ¼¼ÆÃ 

	m_vecInvenInfo[0] = m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)];
	m_vecInvenInfo[1] = m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)];
	m_vecInvenInfo[2] = m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)];

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
}
