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

void CItemManager::Setting_ItemInfo()
{
	m_vecItemInfo.resize(_int(ITEMINDEX::ITEM_END));

	// ¼¦°Ç
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)] =
	{ ITEMINDEX::ITEM_SHOTGUN, TEXT("ShotGun"), TEXT("Weapon"), false, TEXT(""), TEXT("Prototype_Component_Texture_ShotGun"), ITEMTYPE::ITEM_TYPE_WEAPON, false, 100.f, 100, 100.f, 1.f };
	
	// ¼¦°Ç Å°Æ®
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)] =
	{ ITEMINDEX::ITEM_SHOTGUN_KIT, TEXT("ShotGun_Kit"), TEXT("Kit"), false, TEXT(""), TEXT("Prototype_Component_Texture_ShotGun"), ITEMTYPE::ITEM_TYPE_ETC, false, 100.f, 100, 100.f, 1.f };
	
	// ÅºÈ¯
	m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)] =
	{ ITEMINDEX::ITEM_AMMO, TEXT("Gun_Ammo"), TEXT("Ammo"), false, TEXT(""), TEXT("Prototype_Component_Texture_ShotGun"), ITEMTYPE::ITEM_TYPE_AMMO, false, 100.f, 100, 100.f, 1.f };

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

	
}
