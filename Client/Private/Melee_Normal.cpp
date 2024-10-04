#include "stdafx.h"
#include "Melee_Normal.h"
#include "ClientInstance.h"

CMelee_Normal::CMelee_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_Normal::CMelee_Normal(const CMelee_Normal& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_Normal::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMelee_Normal::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CMelee_Normal::Priority_Update(_float fTimeDelta)
{
}

void CMelee_Normal::Update(_float fTimeDelta)
{
}

void CMelee_Normal::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMelee_Normal::Render()
{
	return E_NOTIMPL;
}
void CMelee_Normal::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CMelee_Normal::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}
//
//void CMelee_Normal::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CMelee_Normal::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CMelee_Normal::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CMelee_Normal::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CMelee_Normal* CMelee_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CMelee_Normal::Clone(void* pArg)
{
	return nullptr;
}

void CMelee_Normal::Free()
{
}
