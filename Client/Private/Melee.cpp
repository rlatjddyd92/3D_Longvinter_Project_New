#include "stdafx.h"
#include "Melee.h"
#include "ClientInstance.h"

CMelee::CMelee(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInterAction{ pDevice, pContext }
{
}

CMelee::CMelee(const CMelee& Prototype)
	: CInterAction{ Prototype }
{
}

HRESULT CMelee::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CMelee::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMelee::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CMelee::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMelee::Render()
{
	__super::Render();

	return S_OK;
}
void CMelee::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CMelee::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CMelee::Ready_Components()
{
	return S_OK;
}

HRESULT CMelee::Ready_PartObjects()
{
	return S_OK;
}

CMelee* CMelee::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee* pInstance = new CMelee(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee::Clone(void* pArg)
{
	CMelee* pInstance = new CMelee(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee::Free()
{
	__super::Free();
}
