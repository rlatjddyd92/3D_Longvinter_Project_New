#include "stdafx.h"
#include "Effect_Particle.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& Prototype)
	: CEffect{ Prototype }
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* pArg)
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

void CEffect_Particle::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);

}

void CEffect_Particle::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}

void CEffect_Particle::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);




}

HRESULT CEffect_Particle::Render()
{
	__super::Render();


	return S_OK;
}

HRESULT CEffect_Particle::Bind_WorldMatrix(CShader* pShader, const _char* pContantName)
{
	return pShader->Bind_Matrix(pContantName, &m_WorldMatrix);
}

HRESULT CEffect_Particle::Ready_Components()
{
	return S_OK;
}

HRESULT CEffect_Particle::Ready_PartObjects()
{
	return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Particle* pInstance = new CEffect_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Particle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* pArg)
{
	CEffect_Particle* pInstance = new CEffect_Particle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Particle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Particle::Free()
{
	__super::Free();

}
