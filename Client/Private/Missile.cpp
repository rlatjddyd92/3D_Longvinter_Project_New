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
	return E_NOTIMPL;
}

HRESULT CMissile::Initialize(void* pArg)
{
	return E_NOTIMPL;
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
	return E_NOTIMPL;
}

HRESULT CMissile::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CMissile::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CMissile* CMissile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CMissile::Clone(void* pArg)
{
	return nullptr;
}

void CMissile::Free()
{
}
