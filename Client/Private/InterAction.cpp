#include "stdafx.h"
#include "InterAction.h"
#include "ClientInstance.h"

CInterAction::CInterAction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CInterAction::CInterAction(const CInterAction& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CInterAction::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CInterAction::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CInterAction::Priority_Update(_float fTimeDelta)
{
}

void CInterAction::Update(_float fTimeDelta)
{
}

void CInterAction::Late_Update(_float fTimeDelta)
{
}

HRESULT CInterAction::Render()
{
	return E_NOTIMPL;
}

HRESULT CInterAction::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CInterAction::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CInterAction* CInterAction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CInterAction::Clone(void* pArg)
{
	return nullptr;
}

void CInterAction::Free()
{
}
