#include "stdafx.h"
#include "Missile.h"
#include "ClientInstance.h"

CMissile::CMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInterAction{ pDevice, pContext }
{
}

CMissile::CMissile(const CMissile& Prototype)
	: CInterAction{ Prototype }
{
}

HRESULT CMissile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissile::Initialize(void* pArg)
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

void CMissile::Priority_Update(_float fTimeDelta)
{
}

void CMissile::Update(_float fTimeDelta)
{
}

void CMissile::Late_Update(_float fTimeDelta)
{
}

HRESULT CMissile::Render()
{
	__super::Render();

	return S_OK;
}
void CMissile::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex)
{
}
void CMissile::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}
//
//void CMissile::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CMissile::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CMissile::Ready_Components()
{
	return S_OK;
}

HRESULT CMissile::Ready_PartObjects()
{
	return S_OK;
}

CMissile* CMissile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMissile* pInstance = new CMissile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMissile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMissile::Clone(void* pArg)
{
	CMissile* pInstance = new CMissile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMissile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissile::Free()
{
	__super::Free();
}
