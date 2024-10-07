#include "stdafx.h"
#include "SoundManager.h"
#include "ClientInstance.h"

CSoundManager::CSoundManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CSoundManager::CSoundManager(const CSoundManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSoundManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSoundManager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CSoundManager::Priority_Update(_float fTimeDelta)
{
}

void CSoundManager::Update(_float fTimeDelta)
{
	
}

void CSoundManager::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CSoundManager::Render()
{
	
	return S_OK;
}


CSoundManager* CSoundManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSoundManager* pInstance = new CSoundManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSoundManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSoundManager::Clone(void* pArg)
{
	CSoundManager* pInstance = new CSoundManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSoundManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoundManager::Free()
{
	__super::Free();

	
}
