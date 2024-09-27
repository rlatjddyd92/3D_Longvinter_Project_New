#include "stdafx.h"
#include "Bullet_Straight.h"
#include "ClientInstance.h"

CBullet_Straight::CBullet_Straight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CBullet_Straight::CBullet_Straight(const CBullet_Straight& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CBullet_Straight::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CBullet_Straight::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CBullet_Straight::Priority_Update(_float fTimeDelta)
{
}

void CBullet_Straight::Update(_float fTimeDelta)
{
}

void CBullet_Straight::Late_Update(_float fTimeDelta)
{
}

HRESULT CBullet_Straight::Render()
{
	return E_NOTIMPL;
}

HRESULT CBullet_Straight::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CBullet_Straight::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CBullet_Straight* CBullet_Straight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CBullet_Straight::Clone(void* pArg)
{
	return nullptr;
}

void CBullet_Straight::Free()
{
}
