#include "stdafx.h"
#include "Explosion_Normal.h"
#include "ClientInstance.h"

CExplosion_Normal::CExplosion_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CExplosion{ pDevice, pContext }
{
}

CExplosion_Normal::CExplosion_Normal(const CExplosion_Normal& Prototype)
	: CExplosion{ Prototype }
{
}

HRESULT CExplosion_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion_Normal::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_Pushed_PowerDecrease(0.f); // <- 속도 감소 없음 
	m_fSpec_Extent = { 0.2f,0.2f,0.2f };
	m_fSpec_Scale = 5.f;
	m_fSpec_PushedPower = 0.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = _int(CCollider::TYPE_SPHERE);

	return S_OK;
}

void CExplosion_Normal::Priority_Update(_float fTimeDelta)
{
}

void CExplosion_Normal::Update(_float fTimeDelta)
{
}

void CExplosion_Normal::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CExplosion_Normal::Render()
{
	return S_OK;
}

void CExplosion_Normal::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CExplosion_Normal::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CExplosion_Normal::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bullet_Normal"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CExplosion_Normal::Ready_PartObjects()
{
	return S_OK;
}

CExplosion_Normal* CExplosion_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion_Normal* pInstance = new CExplosion_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CExplosion_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion_Normal::Clone(void* pArg)
{
	CExplosion_Normal* pInstance = new CExplosion_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CExplosion_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion_Normal::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
