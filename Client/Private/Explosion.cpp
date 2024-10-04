#include "stdafx.h"
#include "Explosion.h"
#include "ClientInstance.h"

CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInterAction{ pDevice, pContext }
{
}

CExplosion::CExplosion(const CExplosion& Prototype)
	: CInterAction{ Prototype }
{
}

HRESULT CExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
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

void CExplosion::Priority_Update(_float fTimeDelta)
{
}

void CExplosion::Update(_float fTimeDelta)
{
}

void CExplosion::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);



	m_bDemege = false;
}

HRESULT CExplosion::Render()
{




	return S_OK;
}

void CExplosion::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	if (!m_bDemege)
		return;

	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CExplosion::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	if (!m_bDemege)
		return;

	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}

//void CExplosion::Collision_Reaction_InterAction(CInterAction* pPoint)
//{
//}
//
//void CExplosion::Collision_Reaction_Container(CLongvinter_Container* pPoint)
//{
//}

HRESULT CExplosion::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CExplosion::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CExplosion::Clone(void* pArg)
{
	return nullptr;
}

void CExplosion::Free()
{
}
