#include "stdafx.h"
#include "InterActionManager.h"
#include "ClientInstance.h"

CInterActionManager::CInterActionManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CInterActionManager::CInterActionManager(const CInterActionManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInterActionManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInterActionManager::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_vecInterAction.resize(_int(INTERACTION::INTER_END));
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_Straight"), TEXT("Prototype_Inter_Bullet_Straight"));

	m_vecInterAction[_int(INTERACTION::INTER_BULLET_STRAIGHT)] = static_cast<CBullet_Straight*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_Straight"), -1));



	return S_OK;
}

void CInterActionManager::Priority_Update(_float fTimeDelta)
{

}

void CInterActionManager::Update(_float fTimeDelta)
{

}

void CInterActionManager::Late_Update(_float fTimeDelta)
{
	// InterActionManager는 모든 InterAction객체보다 먼저 




}

HRESULT CInterActionManager::Render()
{
	return S_OK;
}

HRESULT CInterActionManager::Ready_Components()
{
	return S_OK;
}

HRESULT CInterActionManager::Ready_PartObjects()
{
	return S_OK;
}

void CInterActionManager::Input_ActionInfo(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType, CInterAction::TERRAIN_ACTION eAction)
{
	m_vecInterAction[_int(eInterType)]->Add_ActionInfo(pHost, fPosition, fPushedDirec, fPushedPower, fExtent, fDecreasePushedPower, eColliderType, eAction);
}

CInterActionManager* CInterActionManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInterActionManager* pInstance = new CInterActionManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CInterActionManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInterActionManager::Clone(void* pArg)
{
	CInterActionManager* pInstance = new CInterActionManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CInterActionManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInterActionManager::Free()
{
	__super::Free();

	for (auto& iter : m_vecInterAction)
		Safe_Release(iter);

	m_vecInterAction.clear();
}

