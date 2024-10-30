#include "stdafx.h"
#include "EffectManager.h"
#include "ClientInstance.h"

CEffectManager::CEffectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CEffectManager::CEffectManager(const CEffectManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CEffectManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectManager::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	
	m_vecEffect.resize(_int(EFFECT_TYPE::EFFECT_END));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Fire"), TEXT("Prototype_Effect_Particle_Fire"));
	m_vecEffect[_int(EFFECT_TYPE::EFFECT_PARTICLE_FIRE)] = static_cast<CParticle_Fire*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Fire"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Debris"), TEXT("Prototype_Effect_Particle_Debris"));
	m_vecEffect[_int(EFFECT_TYPE::EFFECT_PARTICLE_DEBRIS)] = static_cast<CParticle_Debris*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Debris"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Trail"), TEXT("Prototype_Effect_Particle_Trail"));
	m_vecEffect[_int(EFFECT_TYPE::EFFECT_PARTICLE_TRAIL)] = static_cast<CParticle_Trail*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Trail"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Hit"), TEXT("Prototype_Effect_Particle_Hit"));
	m_vecEffect[_int(EFFECT_TYPE::EFFECT_PARTICLE_HIT)] = static_cast<CParticle_Fire*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Hit"), -1));

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Flame"), TEXT("Prototype_Effect_Particle_Flame"));
	m_vecEffect[_int(EFFECT_TYPE::EFFECT_PARTICLE_FLAME)] = static_cast<CParticle_Flame*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Effect_Particle_Flame"), -1));
	

	return S_OK;
}

void CEffectManager::Priority_Update(_float fTimeDelta)
{

}

void CEffectManager::Update(_float fTimeDelta)
{

}

void CEffectManager::Late_Update(_float fTimeDelta)
{

}

HRESULT CEffectManager::Render()
{
	return S_OK;
}

void CEffectManager::MakeEffect(EFFECT_TYPE eType, _float3 fPosition, _float3 fDirec, _float fTime)
{
	m_vecEffect[_int(eType)]->AddEffectBySpec(fPosition, fDirec, fTime);
}

HRESULT CEffectManager::Ready_Components()
{
	return S_OK;
}

HRESULT CEffectManager::Ready_PartObjects()
{
	return S_OK;
}




CEffectManager* CEffectManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffectManager* pInstance = new CEffectManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffectManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffectManager::Clone(void* pArg)
{
	CEffectManager* pInstance = new CEffectManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffectManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectManager::Free()
{
	__super::Free();

	for (auto& iter : m_vecEffect)
		Safe_Release(iter);

	m_vecEffect.clear();

}

