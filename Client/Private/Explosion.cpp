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
	return E_NOTIMPL;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CExplosion::Priority_Update(_float fTimeDelta)
{
}

void CExplosion::Update(_float fTimeDelta)
{
}

void CExplosion::Late_Update(_float fTimeDelta)
{
}

HRESULT CExplosion::Render()
{
	return E_NOTIMPL;
}

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