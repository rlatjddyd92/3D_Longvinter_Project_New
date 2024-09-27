#include "stdafx.h"
#include "InterActionManager.h"
#include "ClientInstance.h"

CInterActionManager::CInterActionManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CInterActionManager::CInterActionManager(const CInterActionManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInterActionManager::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CInterActionManager::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CInterActionManager::Priority_Update(_float fTimeDelta)
{
}

void CInterActionManager::Update(_float fTimeDelta)
{
}

void CInterActionManager::Late_Update(_float fTimeDelta)
{
}

HRESULT CInterActionManager::Render()
{
	return E_NOTIMPL;
}

HRESULT CInterActionManager::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CInterActionManager::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CInterActionManager* CInterActionManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CInterActionManager::Clone(void* pArg)
{
	return nullptr;
}

void CInterActionManager::Free()
{
}

