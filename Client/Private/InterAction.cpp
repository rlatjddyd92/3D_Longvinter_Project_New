#include "stdafx.h"
#include "InterAction.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

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
	if (m_fShowTime > 0.f)
	{
		m_fShowTime -= fTimeDelta;
		if (m_fShowTime < 0.f)
		{
			m_fShowTime = 0.f;
		}
	}


}

void CInterAction::Update(_float fTimeDelta)
{
}

void CInterAction::Late_Update(_float fTimeDelta)
{
	



	for (list<INTERACTION_INFO*>::iterator iter = m_Actionlist.begin(); iter != m_Actionlist.end(); )
	{
		_vector fPosition = (*iter)->pTransform->Get_State(CTransform::STATE_POSITION);

		if ((fPosition.m128_f32[0] < 0.f) + (fPosition.m128_f32[1] < 0.f) + (fPosition.m128_f32[2] < 0.f) > 0)
			(*iter)->bDead = true;

		if ((fPosition.m128_f32[0] > LMAX_X * LCUBESIZE) + (fPosition.m128_f32[1] > LMAX_Y * LCUBESIZE) + (fPosition.m128_f32[2] > LMAX_Z * LCUBESIZE) > 0)
			(*iter)->bDead = true;



		if ((*iter)->bDead)
		{
			Safe_Release((*iter)->pHost);
			Safe_Release((*iter)->pTransform);
			Safe_Release((*iter)->pCollider);
			Safe_Delete((*iter));

			iter = m_Actionlist.erase(iter);
		}
		else
			++iter;
	}
		

}

HRESULT CInterAction::Render()
{
	return S_OK;
}

void CInterAction::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
}

void CInterAction::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
}



void CInterAction::Add_InterActionObject(CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType, TERRAIN_ACTION eAction)
{
	INTERACTION_INFO* pNew = new INTERACTION_INFO;

	if (pHost)
	{
		pNew->pHost = pHost;
		Safe_AddRef(pNew->pHost);
	}
	

	pNew->pTransform = CTransform::Create(m_pDevice, m_pContext, nullptr);
	//Safe_AddRef(pNew->pTransform);
	pNew->pTransform->Set_State(CTransform::STATE_POSITION, { fPosition.x, fPosition.y,fPosition.z });

	pNew->pTransform->Set_Pushed_Power(fPushedDirec, fPushedPower);
	pNew->pTransform->Set_Pushed_PowerDecrease(fDecreasePushedPower);

	CBounding::BOUNDING_DESC* pBoundingDesc{};

	if (eColliderType == CCollider::TYPE_AABB)
	{
		CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(fExtent, fExtent, fExtent);
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB, &ColliderDesc);
	}
	else if (eColliderType == CCollider::TYPE_OBB)
	{
		CBounding_OBB::BOUNDING_OBB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(fExtent, fExtent, fExtent);
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB, &ColliderDesc);
	}
	else if (eColliderType == CCollider::TYPE_SPHERE)
	{
		CBounding_Sphere::BOUNDING_SPHERE_DESC			ColliderDesc{};
		ColliderDesc.fRadius = fExtent;
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE, &ColliderDesc);
	}
	

	//Safe_AddRef(pNew->pCollider);
	pNew->pCollider->Update(pNew->pTransform->Get_WorldMatrix_Ptr());

	pNew->eAction = eAction;

	m_Actionlist.push_back(pNew);
}

void CInterAction::Add_InterActionObject_BySpec(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec)
{
	INTERACTION_INFO* pNew = new INTERACTION_INFO;

	if (pHost)
	{
		pNew->pHost = pHost;
		Safe_AddRef(pNew->pHost);
	}

	pNew->pTransform = CTransform::Create(m_pDevice, m_pContext, nullptr);
	//Safe_AddRef(pNew->pTransform);
	pNew->pTransform->Set_State(CTransform::STATE_POSITION, { fPosition.x, fPosition.y,fPosition.z });
	pNew->pTransform->Set_Scaled(m_fSpec_Scale, m_fSpec_Scale, m_fSpec_Scale);
	pNew->pTransform->Set_Pushed_Power(fPushedDirec, m_fSpec_PushedPower);
	pNew->pTransform->Set_Pushed_PowerDecrease(m_fSpec_PushedPower_Decrease);

	CBounding::BOUNDING_DESC* pBoundingDesc{};
	CCollider::TYPE eColliderType = CCollider::TYPE(m_iColliderType);

	if (eColliderType == CCollider::TYPE_AABB)
	{
		CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(m_fSpec_Extent.x, m_fSpec_Extent.y, m_fSpec_Extent.z);
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB, &ColliderDesc);
	}
	else if (eColliderType == CCollider::TYPE_OBB)
	{
		CBounding_OBB::BOUNDING_OBB_DESC			ColliderDesc{};
		ColliderDesc.vExtents = _float3(m_fSpec_Extent.x, m_fSpec_Extent.y, m_fSpec_Extent.z);
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB, &ColliderDesc);
	}
	else if (eColliderType == CCollider::TYPE_SPHERE)
	{
		CBounding_Sphere::BOUNDING_SPHERE_DESC			ColliderDesc{};
		ColliderDesc.fRadius = m_fSpec_Extent.x;
		ColliderDesc.vCenter = _float3(0.0f, 0.0f, 0.0f);
		pNew->pCollider = CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE, &ColliderDesc);
	}


	//Safe_AddRef(pNew->pCollider);
	pNew->pCollider->Update(pNew->pTransform->Get_WorldMatrix_Ptr());
	pNew->fTime = m_fSpec_Time;

	m_Actionlist.push_back(pNew);
}

HRESULT CInterAction::Bind_WorldMatrix(CShader* pShader, const _char* pContantName)
{
	return pShader->Bind_Matrix(pContantName, &m_WorldMatrix); 
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
		Safe_Release(iter->pTrace);
		Safe_Release(iter->pTransform);
		Safe_Release(iter->pCollider);
		Safe_Delete(iter);
	}


	m_Actionlist.clear();
}
