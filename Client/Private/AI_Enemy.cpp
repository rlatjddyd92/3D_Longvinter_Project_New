#include "stdafx.h"
#include "AI_Enemy.h"
#include "ClientInstance.h"

CAI_Enemy::CAI_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainer_Enemy{ pDevice, pContext }
{
}

CAI_Enemy::CAI_Enemy(const CAI_Enemy& Prototype)
	: CContainer_Enemy{ Prototype }
{
}

HRESULT CAI_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAI_Enemy::Initialize(void* pArg)
{
	AI_Enemy_Desc* pTemp = static_cast<AI_Enemy_Desc*>(pArg);





	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	eContainerType = CONTAINER::CONTAINER_ENEMY;



	return S_OK;
}

void CAI_Enemy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CAI_Enemy::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CAI_Enemy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAI_Enemy::Render()
{
	__super::Render();


	return S_OK;
}

void CAI_Enemy::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex)
{
}

void CAI_Enemy::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}


HRESULT CAI_Enemy::Ready_Components()
{
	
	return S_OK;
}

HRESULT CAI_Enemy::Ready_PartObjects()
{
	
	return S_OK;
}

CAI_Enemy* CAI_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAI_Enemy* pInstance = new CAI_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAI_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAI_Enemy::Clone(void* pArg)
{
	CAI_Enemy* pInstance = new CAI_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAI_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Enemy::Free()
{
	__super::Free();
}
