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
	m_vecConInterlist.resize(_int(CONTAINER::CONTAINER_END));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_Straight"), TEXT("Prototype_Inter_Bullet_Straight"));
	m_vecInterAction[_int(INTERACTION::INTER_BULLET_STRAIGHT)] = static_cast<CBullet_Straight*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_Straight"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), TEXT("Prototype_Inter_Bullet_MachineGun"));
	m_vecInterAction[_int(INTERACTION::INTER_BULLET_MACHINEGUN)] = static_cast<CBullet_MachineGun*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Throw_Mine"), TEXT("Prototype_Inter_Throw_Mine"));
	m_vecInterAction[_int(INTERACTION::INTER_THORW_MINE)] = static_cast<CThrow_Mine*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Throw_Mine"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Throw_Granade"), TEXT("Prototype_Inter_Throw_Granade"));
	m_vecInterAction[_int(INTERACTION::INTER_THORW_GRANADE)] = static_cast<CThrow_Granade*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Throw_Granade"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Explosion_Normal"), TEXT("Prototype_Inter_Explosion_Normal"));
	m_vecInterAction[_int(INTERACTION::INTER_EXPLOSION_NORMAL)] = static_cast<CExplosion_Normal*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Explosion_Normal"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Fire"), TEXT("Prototype_Inter_Fire"));
	m_vecInterAction[_int(INTERACTION::INTER_FIRE)] = static_cast<CFire*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Fire"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Melee_ShotGun"), TEXT("Prototype_Inter_Melee_ShotGun"));
	m_vecInterAction[_int(INTERACTION::INTER_MELEE_SHOTGUN)] = static_cast<CMelee_ShotGun*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Melee_ShotGun"), -1));

	//m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), TEXT("Prototype_Inter_Bullet_MachineGun"));
	//m_vecInterAction[_int(INTERACTION::INTER_BULLET_MACHINEGUN)] = static_cast<CBullet_MachineGun*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), -1));

	//m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), TEXT("Prototype_Inter_Bullet_MachineGun"));
	//m_vecInterAction[_int(INTERACTION::INTER_BULLET_MACHINEGUN)] = static_cast<CBullet_MachineGun*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), -1));

	//m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), TEXT("Prototype_Inter_Bullet_MachineGun"));
	//m_vecInterAction[_int(INTERACTION::INTER_BULLET_MACHINEGUN)] = static_cast<CBullet_MachineGun*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_Bullet_MachineGun"), -1));


	// LANDOBJECT
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_AppleTree"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_APPLETREE)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_AppleTree"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_APPLETREE)])->SetLandObject(INTERACTION::INTER_APPLETREE);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Bush"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_BUSH)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Bush"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_BUSH)])->SetLandObject(INTERACTION::INTER_BUSH);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Rock"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_ROCK)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Rock"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_ROCK)])->SetLandObject(INTERACTION::INTER_ROCK);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Tree"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_TREE)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Tree"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_TREE)])->SetLandObject(INTERACTION::INTER_TREE);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Item"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_ITEM)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Item"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_ITEM)])->SetLandObject(INTERACTION::INTER_ITEM);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Door"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_DOOR)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_Door"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_DOOR)])->SetLandObject(INTERACTION::INTER_DOOR);

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_MonsterMaker"), TEXT("Prototype_Inter_LandObject_NonAnim"));
	m_vecInterAction[_int(INTERACTION::INTER_MONSTERMAKER)] = static_cast<CLandObject_NonAnim*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Inter_LandObject_MonsterMaker"), -1));
	static_cast<CLandObject_NonAnim*>(m_vecInterAction[_int(INTERACTION::INTER_MONSTERMAKER)])->SetLandObject(INTERACTION::INTER_MONSTERMAKER);

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
	Check_Collision_InterAction_Container(INTERACTION::INTER_BULLET_MACHINEGUN, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_InterAction_Container(INTERACTION::INTER_EXPLOSION_NORMAL, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_InterAction_Container(INTERACTION::INTER_FIRE, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_InterAction_Container(INTERACTION::INTER_THORW_MINE, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_InterAction_Container(INTERACTION::INTER_MELEE_SHOTGUN, CONTAINER::CONTAINER_ENEMY);
	Check_Collision_InterAction(INTERACTION::INTER_EXPLOSION_NORMAL, INTERACTION::INTER_APPLETREE);
	Check_Collision_InterAction(INTERACTION::INTER_EXPLOSION_NORMAL, INTERACTION::INTER_BUSH);
	Check_Collision_InterAction(INTERACTION::INTER_EXPLOSION_NORMAL, INTERACTION::INTER_ROCK);
	Check_Collision_InterAction(INTERACTION::INTER_EXPLOSION_NORMAL, INTERACTION::INTER_TREE);
	Check_Collision_InterAction(INTERACTION::INTER_FIRE, INTERACTION::INTER_APPLETREE);
	Check_Collision_InterAction(INTERACTION::INTER_FIRE, INTERACTION::INTER_BUSH);
	Check_Collision_InterAction(INTERACTION::INTER_FIRE, INTERACTION::INTER_ROCK);
	Check_Collision_InterAction(INTERACTION::INTER_FIRE, INTERACTION::INTER_TREE);


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

void CInterActionManager::Add_InterActionObject(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType, CInterAction::TERRAIN_ACTION eAction, _float fAngle)
{
	m_vecInterAction[_int(eInterType)]->Add_InterActionObject(pHost, fPosition, fPushedDirec, fPushedPower, fExtent, fDecreasePushedPower, eColliderType, eAction, fAngle);
}

void CInterActionManager::Input_ContainerColliderPointer(CONTAINER eContanerType, CLongvinter_Container* pHost, CCollider* pCollider)
{
	CON_INTER_INFO* pNew = new CON_INTER_INFO;

	pNew->eType = eContanerType;
	pNew->pPoint = pHost;
	pNew->pCollider = pCollider;

	Safe_AddRef(pNew->pPoint);
	Safe_AddRef(pNew->pCollider);

	m_vecConInterlist[_int(eContanerType)].push_back(pNew);
}

void CInterActionManager::Add_InterActionObject_BySpec(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fAngle)
{
	m_vecInterAction[_int(eInterType)]->Add_InterActionObject_BySpec(eInterType, pHost, fPosition, fPushedDirec, fAngle);
}

void CInterActionManager::Delete_LastInterAction(INTERACTION eInterType)
{
	m_vecInterAction[_int(eInterType)]->Delete_LastInterAction();
}

void CInterActionManager::Check_Collision_InterAction(INTERACTION eFirst, INTERACTION eSecond)
{
	for (list<CInterAction::INTERACTION_INFO*>::iterator iterA = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->begin(); iterA != m_vecInterAction[_int(eFirst)]->Get_Actionlist()->end();)
	{
		if (((*iterA)->pCollider == nullptr) + ((*iterA)->pTransform == nullptr) > 0)
		{
			Safe_Release((*iterA)->pCollider);
			Safe_Release((*iterA)->pHost);
			Safe_Release((*iterA)->pTransform);
			Safe_Delete(*iterA);
			iterA = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->erase(iterA);
			continue;
		}

		if ((*iterA)->bActive == false)
		{
			++iterA;
			continue;
		}


		for (list<CInterAction::INTERACTION_INFO*>::iterator iterB = m_vecInterAction[_int(eSecond)]->Get_Actionlist()->begin(); iterB != m_vecInterAction[_int(eSecond)]->Get_Actionlist()->end();)
		{
			if (((*iterB)->pCollider == nullptr) + ((*iterB)->pTransform == nullptr) > 0)
			{
				Safe_Release((*iterB)->pCollider);
				Safe_Release((*iterB)->pHost);
				Safe_Release((*iterB)->pTransform);
				Safe_Delete(*iterB);
				iterB = m_vecInterAction[_int(eFirst)]->Get_Actionlist()->erase(iterB);
				continue;
			}

			if((*iterB)->bActive == false)
			{
				++iterB;
				continue;
			}


			//CCollider::TYPE eType = CCollider::TYPE(m_vecInterAction[_int(eFirst)]->Get_ColliderType());
			CCollider::TYPE eType = CCollider::TYPE::TYPE_AABB;

			if ((*iterA)->pCollider->GetCollision(eType, (*iterB)->pCollider))
			{
				m_vecInterAction[_int(eFirst)]->Collision_Reaction_InterAction((*iterB)->pHost, eSecond, *iterA);
				m_vecInterAction[_int(eSecond)]->Collision_Reaction_InterAction((*iterA)->pHost, eFirst, *iterB);
			}
			else if (m_vecInterAction[_int(eFirst)]->Get_Sensor_Range() > 0.f)
			{
				_float3 fInter = (*iterA)->pCollider->GetBoundingCenter();
				_float3 fContainer = (*iterB)->pCollider->GetBoundingCenter();

				_float fDistance = sqrt(pow((fInter.x - fContainer.x), 2) + pow((fInter.y - fContainer.y), 2) + pow((fInter.z - fContainer.z), 2));

				if (m_vecInterAction[_int(eFirst)]->Get_Sensor_Range() + (*iterB)->pCollider->GetBoundingExtents().x > fDistance)
				{
					m_vecInterAction[_int(eFirst)]->Collision_Reaction_InterAction((*iterB)->pHost, eSecond, *iterA);
					m_vecInterAction[_int(eSecond)]->Collision_Reaction_InterAction((*iterA)->pHost, eFirst, *iterB);
				}
			}
			else if (m_vecInterAction[_int(eSecond)]->Get_Sensor_Range() > 0.f)
			{
				_float3 fInter = (*iterA)->pCollider->GetBoundingCenter();
				_float3 fContainer = (*iterB)->pCollider->GetBoundingCenter();

				_float fDistance = sqrt(pow((fInter.x - fContainer.x), 2) + pow((fInter.y - fContainer.y), 2) + pow((fInter.z - fContainer.z), 2));

				if (m_vecInterAction[_int(eSecond)]->Get_Sensor_Range() + (*iterB)->pCollider->GetBoundingExtents().x > fDistance)
				{
					m_vecInterAction[_int(eSecond)]->Collision_Reaction_InterAction((*iterB)->pHost, eFirst, *iterA);
					m_vecInterAction[_int(eFirst)]->Collision_Reaction_InterAction((*iterA)->pHost, eSecond, *iterB);
				}
			}

			++iterB;
		}

		++iterA;
	}
		
}

void CInterActionManager::Check_Collision_InterAction_Container(INTERACTION eInter, CONTAINER eContainer)
{
	for (list<CInterAction::INTERACTION_INFO*>::iterator iterA = m_vecInterAction[_int(eInter)]->Get_Actionlist()->begin(); iterA != m_vecInterAction[_int(eInter)]->Get_Actionlist()->end();)
	{
		if (((*iterA)->pCollider == nullptr) + ((*iterA)->pTransform == nullptr) > 0)
		{
			Safe_Release((*iterA)->pCollider);
			Safe_Release((*iterA)->pHost);
			Safe_Release((*iterA)->pTransform);
			Safe_Delete(*iterA);
			iterA = m_vecInterAction[_int(eInter)]->Get_Actionlist()->erase(iterA);
			continue;
		}

		if ((*iterA)->bActive == false)
		{
			++iterA;
			continue;
		}


		for (list<CON_INTER_INFO*>::iterator iterB = m_vecConInterlist[_int(eContainer)].begin(); iterB != m_vecConInterlist[_int(eContainer)].end();)
		{
			if (((*iterB)->pPoint == nullptr) + ((*iterB)->pPoint->GetDead()) > 0)
			{
				Safe_Release((*iterB)->pCollider);
				Safe_Release((*iterB)->pPoint);
				Safe_Delete(*iterB);
				iterB = m_vecConInterlist[_int(eContainer)].erase(iterB);
				continue;
			}

			if ((*iterB)->pPoint->GetOff())
			{
				++iterB;
				continue;
			}
				

			CCollider::TYPE eType = CCollider::TYPE::TYPE_AABB;
			// 추후 상대방 콘테이너 객체에 따라 값을 변경하는 코드 추가 필요 

			if ((*iterA)->pCollider->GetCollision(eType, (*iterB)->pCollider))
			{
				m_vecInterAction[_int(eInter)]->Collision_Reaction_Container((*iterB)->pPoint, eContainer, *iterA);
				(*iterB)->pPoint->Collision_Reaction_InterAction((*iterA)->pHost, eInter, *(*iterA));
			}
			else if (m_vecInterAction[_int(eInter)]->Get_Sensor_Range() > 0.f)
			{
				_float3 fInter = (*iterA)->pCollider->GetBoundingCenter();
				_float3 fContainer = (*iterB)->pCollider->GetBoundingCenter();
				
				_float fDistance = sqrt(pow((fInter.x - fContainer.x), 2) + pow((fInter.y - fContainer.y), 2) + pow((fInter.z - fContainer.z), 2));

				if (m_vecInterAction[_int(eInter)]->Get_Sensor_Range() + (*iterB)->pCollider->GetBoundingExtents().x > fDistance)
				{
					m_vecInterAction[_int(eInter)]->Collision_Reaction_Container((*iterB)->pPoint, eContainer, *iterA);
					(*iterB)->pPoint->Collision_Reaction_InterAction((*iterA)->pHost, eInter, *(*iterA));
				}
			}

			++iterB;
		}

		++iterA;
	}
}

void CInterActionManager::Check_Collision_Container(CONTAINER eFirst, CONTAINER eSecond)
{
	for (list<CON_INTER_INFO*>::iterator iterA = m_vecConInterlist[_int(eFirst)].begin(); iterA != m_vecConInterlist[_int(eFirst)].end();)
	{
		if (((*iterA)->pPoint == nullptr) + ((*iterA)->pPoint->GetDead()) > 0)
		{
			Safe_Release((*iterA)->pCollider);
			Safe_Release((*iterA)->pPoint);
			Safe_Delete(*iterA);
			iterA = m_vecConInterlist[_int(eFirst)].erase(iterA);
			continue;
		}

		if ((*iterA)->pPoint->GetOff())
		{
			++iterA;
			continue;
		}


		for (list<CON_INTER_INFO*>::iterator iterB = m_vecConInterlist[_int(eSecond)].begin(); iterB != m_vecConInterlist[_int(eSecond)].end();)
		{
			if (((*iterB)->pPoint == nullptr) + ((*iterB)->pPoint->GetDead()) > 0)
			{
				Safe_Release((*iterB)->pCollider);
				Safe_Release((*iterB)->pPoint);
				Safe_Delete(*iterB);
				iterB = m_vecConInterlist[_int(eSecond)].erase(iterB);
				continue;
			}

			if ((*iterB)->pPoint->GetOff())
			{
				++iterB;
				continue;
			}


			CCollider::TYPE eType = CCollider::TYPE::TYPE_AABB;
			// 추후 상대방 콘테이너 객체에 따라 값을 변경하는 코드 추가 필요 

			if ((*iterA)->pCollider->GetCollision(eType, (*iterB)->pCollider))
			{
				(*iterA)->pPoint->Collision_Reaction_Container((*iterB)->pPoint, eSecond);
				(*iterB)->pPoint->Collision_Reaction_Container((*iterA)->pPoint, eFirst);
			}

			++iterB;
		}

		++iterA;
	}


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

