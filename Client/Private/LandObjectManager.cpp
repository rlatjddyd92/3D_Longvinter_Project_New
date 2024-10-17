#include "stdafx.h"
#include "LandObjectManager.h"
#include "ClientInstance.h"

CLandObjectManager::CLandObjectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CLandObjectManager::CLandObjectManager(const CLandObjectManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CLandObjectManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObjectManager::Initialize(void* pArg)
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

void CLandObjectManager::Priority_Update(_float fTimeDelta)
{

}

void CLandObjectManager::Update(_float fTimeDelta)
{

}

void CLandObjectManager::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CLandObjectManager::Render()
{
	return S_OK;
}

HRESULT CLandObjectManager::Ready_Components()
{
	return S_OK;
}

HRESULT CLandObjectManager::Ready_PartObjects()
{
	return S_OK;
}



CLandObjectManager* CLandObjectManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLandObjectManager* pInstance = new CLandObjectManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandObjectManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandObjectManager::Clone(void* pArg)
{
	CLandObjectManager* pInstance = new CLandObjectManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLandObjectManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandObjectManager::Free()
{
	__super::Free();

	
}

