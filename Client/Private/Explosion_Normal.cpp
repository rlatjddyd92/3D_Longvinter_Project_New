#include "stdafx.h"
#include "Explosion_Normal.h"
#include "ClientInstance.h"

CExplosion_Normal::CExplosion_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CExplosion{ pDevice, pContext }
{
}

CExplosion_Normal::CExplosion_Normal(const CExplosion_Normal& Prototype)
	: CExplosion{ Prototype }
{
}

HRESULT CExplosion_Normal::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CExplosion_Normal::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CExplosion_Normal::Priority_Update(_float fTimeDelta)
{
}

void CExplosion_Normal::Update(_float fTimeDelta)
{
}

void CExplosion_Normal::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CExplosion_Normal::Render()
{
	return E_NOTIMPL;
}

void CExplosion_Normal::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CExplosion_Normal::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}

//void CExplosion_Normal::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CExplosion_Normal::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CExplosion_Normal::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CExplosion_Normal::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CExplosion_Normal* CExplosion_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CExplosion_Normal::Clone(void* pArg)
{
	return nullptr;
}

void CExplosion_Normal::Free()
{
}
