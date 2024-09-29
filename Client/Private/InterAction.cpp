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
	return S_OK;
}

HRESULT CInterAction::Initialize(void* pArg)
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
	return S_OK;
}

void CInterAction::Add_ActionInfo(CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType, TERRAIN_ACTION eAction)
{
	INTERACTION_INFO* pNew = new INTERACTION_INFO;

	pNew->pHost = pHost;
	Safe_AddRef(pNew->pHost);

	pNew->pTransform = CTransform::Create(m_pDevice, m_pContext, nullptr);
	Safe_AddRef(pNew->pTransform);
	pNew->pTransform->Set_State(CTransform::STATE_POSITION, { fPosition.x, fPosition.y,fPosition.z });

	pNew->pTransform->Set_Pushed_Power(fPushedDirec, fPushedPower);
	pNew->pTransform->Set_Pushed_PowerDecrease(fDecreasePushedPower);

	CBounding::BOUNDING_DESC* pBoundingDesc{};

	if (eColliderType == CCollider::TYPE_AABB)
	{
		CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(fExtent, fExtent, fExtent);
		ColliderDesc.vCenter = fPosition;
		__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&pNew->pCollider));
	}
	else if (eColliderType == CCollider::TYPE_OBB)
	{
		CBounding_OBB::BOUNDING_OBB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(fExtent, fExtent, fExtent);
		ColliderDesc.vCenter = fPosition;
		__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&pNew->pCollider));
	}
	else if (eColliderType == CCollider::TYPE_SPHERE)
	{
		CBounding_Sphere::BOUNDING_SPHERE_DESC			ColliderDesc{};
		ColliderDesc.fRadius = fExtent;
		ColliderDesc.vCenter = fPosition;
		__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&pNew->pCollider));
	}
	

	Safe_AddRef(pNew->pCollider);
	pNew->pCollider->Update(pNew->pTransform->Get_WorldMatrix_Ptr());

	pNew->eAction = eAction;

	m_Actionlist.push_back(pNew);
}

HRESULT CInterAction::Ready_Components()
{
	return S_OK;
}

HRESULT CInterAction::Ready_PartObjects()
{
	return S_OK;
}

CInterAction* CInterAction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInterAction* pInstance = new CInterAction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CInterAction"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInterAction::Clone(void* pArg)
{
	CInterAction* pInstance = new CInterAction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CInterAction"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInterAction::Free()
{
	__super::Free();

	for (auto& iter : m_Actionlist)
	{
		Safe_Release(iter->pHost);
		Safe_Release(iter->pTransform);
		Safe_Release(iter->pCollider);
		Safe_Delete(iter);
	}


	m_Actionlist.clear();
}
