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
	return E_NOTIMPL;
}

HRESULT CMelee::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CMelee::Priority_Update(_float fTimeDelta)
{
}

void CMelee::Update(_float fTimeDelta)
{
}

void CMelee::Late_Update(_float fTimeDelta)
{
}

HRESULT CMelee::Render()
{
	return E_NOTIMPL;
}
void CMelee::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex)
{
}
void CMelee::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}
//
//void CMelee::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CMelee::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CMelee::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CMelee::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CMelee* CMelee::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CMelee::Clone(void* pArg)
{
	return nullptr;
}

void CMelee::Free()
{
}
