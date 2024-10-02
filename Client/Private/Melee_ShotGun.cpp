#include "stdafx.h"
#include "Melee_ShotGun.h"
#include "ClientInstance.h"

CMelee_ShotGun::CMelee_ShotGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_ShotGun::CMelee_ShotGun(const CMelee_ShotGun& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_ShotGun::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMelee_ShotGun::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CMelee_ShotGun::Priority_Update(_float fTimeDelta)
{
}

void CMelee_ShotGun::Update(_float fTimeDelta)
{
}

void CMelee_ShotGun::Late_Update(_float fTimeDelta)
{
}

HRESULT CMelee_ShotGun::Render()
{
	return E_NOTIMPL;
}

HRESULT CMelee_ShotGun::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CMelee_ShotGun::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CMelee_ShotGun* CMelee_ShotGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CMelee_ShotGun::Clone(void* pArg)
{
	return nullptr;
}

void CMelee_ShotGun::Free()
{
}
