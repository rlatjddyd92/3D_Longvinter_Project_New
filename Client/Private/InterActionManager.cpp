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
	m_vecConInterlist.resize(_int(CONTAINER::CONTAINER_END));


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
	// InterActionManager는 모든 InterAction객체보다 먼저 업데이트가 돌아간다 

	Check_Collision_InterAction_Container(INTERACTION::INTER_BULLET_STRAIGHT, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_Container(CONTAINER::CONTAINER_PLAYER, CONTAINER::CONTAINER_ENEMY);
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

void CInterActionManager::Add_InterActionObject(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType, CInterAction::TERRAIN_ACTION eAction)
{
	m_vecInterAction[_int(eInterType)]->Add_InterActionObject(pHost, fPosition, fPushedDirec, fPushedPower, fExtent, fDecreasePushedPower, eColliderType, eAction);
}

void CInterActionManager::Input_ContainerColliderPointer(CONTAINER eContanerType, CLongvinter_Container* pHost, CCollider* pCollider)
{
	CON_INTER_INFO* pNew = new CON_INTER_INFO;

	pNew->eType = eContanerType;
	pNew->pPoint = pHost;
	pNew->pCollider = pCollider;

	m_vecConInterlist[_int(eContanerType)].push_back(pNew);
}

void CInterActionManager::Check_Collision_InterAction(INTERACTION eFirst, INTERACTION eSecond)
{
	for (list<CInterAction::INTERACTION_INFO*>::iterator iterA = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->begin(); iterA != m_vecInterAction[_int(eFirst)]->Get_Actionlist()->end();)
	{
		if (((*iterA)->pCollider == nullptr) + ((*iterA)->pHost == nullptr) + ((*iterA)->pTransform == nullptr) + ((*iterA)->pHost->GetDead()) > 0)
		{
			Safe_Release((*iterA)->pCollider);
			Safe_Release((*iterA)->pHost);
			Safe_Release((*iterA)->pTransform);
			Safe_Delete(*iterA);
			iterA = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->erase(iterA);
			continue;
		}

		if ((*iterA)->pHost->GetOff())
			continue;

		for (list<CInterAction::INTERACTION_INFO*>::iterator iterB = m_vecInterAction[_int(eSecond)]->Get_Actionlist()->begin(); iterB != m_vecInterAction[_int(eSecond)]->Get_Actionlist()->end();)
		{
			if (((*iterB)->pCollider == nullptr) + ((*iterB)->pHost == nullptr) + ((*iterB)->pTransform == nullptr) + ((*iterB)->pHost->GetDead()) > 0)
			{
				Safe_Release((*iterB)->pCollider);
				Safe_Release((*iterB)->pHost);
				Safe_Release((*iterB)->pTransform);
				Safe_Delete(*iterB);
				iterB = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->erase(iterB);
				continue;
			}

			if ((*iterB)->pHost->GetOff())
				continue;







		}

		++iterA;
	}
		
}

void CInterActionManager::Check_Collision_InterAction_Container(INTERACTION eInter, CONTAINER eContainer)
{
	for (list<CInterAction::INTERACTION_INFO*>::iterator iterA = m_vecInterAction[_int(eInter)]->Get_Actionlist()->begin(); iterA != m_vecInterAction[_int(eInter)]->Get_Actionlist()->end();)
	{
		for (list<CON_INTER_INFO*>::iterator iterB = m_vecConInterlist[_int(eContainer)].begin(); iterB != m_vecConInterlist[_int(eContainer)].end();)
		{

		}
	}
		



}

void CInterActionManager::Check_Collision_Container(CONTAINER eFirst, CONTAINER eSecond)
{
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

	for (auto& iter : m_vecConInterlist)
	{
		for (auto& iterlist : iter)
		{
			Safe_Release(iterlist->pPoint);
			Safe_Release(iterlist->pCollider);
			Safe_Delete(iterlist);
		}
		iter.clear();
	}
		
	m_vecConInterlist.clear();
}

