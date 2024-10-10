#include "stdafx.h"
#include "Melee_ChainSaw.h"
#include "ClientInstance.h"

CMelee_ChainSaw::CMelee_ChainSaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_ChainSaw::CMelee_ChainSaw(const CMelee_ChainSaw& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_ChainSaw::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMelee_ChainSaw::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CMelee_ChainSaw::Priority_Update(_float fTimeDelta)
{
}

void CMelee_ChainSaw::Update(_float fTimeDelta)
{
}

void CMelee_ChainSaw::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMelee_ChainSaw::Render()
{
	return E_NOTIMPL;
}
void CMelee_ChainSaw::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CMelee_ChainSaw::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}
//
//void CMelee_ChainSaw::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CMelee_ChainSaw::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CMelee_ChainSaw::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CMelee_ChainSaw::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CMelee_ChainSaw* CMelee_ChainSaw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CMelee_ChainSaw::Clone(void* pArg)
{
	return nullptr;
}

void CMelee_ChainSaw::Free()
{
}
