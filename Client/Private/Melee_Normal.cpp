#include "stdafx.h"
#include "Melee_Normal.h"
#include "ClientInstance.h"

CMelee_Normal::CMelee_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_Normal::CMelee_Normal(const CMelee_Normal& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee_Normal::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_Pushed_PowerDecrease(0.f); // <- 속도 감소 없음 
	m_fSpec_Extent = { 0.5f,0.5f,0.5f };
	m_fSpec_Scale = 0.5f;
	m_fSpec_PushedPower = 0.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = _int(CCollider::TYPE_SPHERE);
	m_fSpec_Time = 0.1f;

	return S_OK;
}

void CMelee_Normal::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CMelee_Normal::Update(_float fTimeDelta)
{
	for (auto& iter : m_Actionlist)
	{
		iter->fTime -= fTimeDelta;

		if (iter->fTime <= 0.f)
		{
			iter->bDead = true;
			continue;
		}
	}
}

void CMelee_Normal::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CMelee_Normal::Render()
{
	return S_OK;
}
void CMelee_Normal::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CMelee_Normal::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CMelee_Normal::Ready_Components()
{
	return S_OK;
}

HRESULT CMelee_Normal::Ready_PartObjects()
{
	return S_OK;
}

CMelee_Normal* CMelee_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee_Normal* pInstance = new CMelee_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee_Normal::Clone(void* pArg)
{
	CMelee_Normal* pInstance = new CMelee_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee_Normal::Free()
{
}
