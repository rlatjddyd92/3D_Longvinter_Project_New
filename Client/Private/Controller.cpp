#include "stdafx.h"
#include "Controller.h"
#include "ClientInstance.h"

HRESULT CController::Ready_Components()
{
	return E_NOTIMPL;
}

HRESULT CController::Ready_PartObjects()
{
	return E_NOTIMPL;
}

CController* CController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CController::Clone(void* pArg)
{
	return nullptr;
}

void CController::Free()
{
}
