#include "stdafx.h"
#include "Bullet_Straight.h"
#include "ClientInstance.h"

CBullet_Straight::CBullet_Straight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CBullet_Straight::CBullet_Straight(const CBullet_Straight& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CBullet_Straight::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Straight::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	m_pTransformCom->Set_Pushed_PowerDecrease(0.f); // <- 속도 감소 없음 

	return S_OK;
}

void CBullet_Straight::Priority_Update(_float fTimeDelta)
{
}

void CBullet_Straight::Update(_float fTimeDelta)
{

	for (auto& iter : m_Actionlist)
	{
		iter->pCollider->Update(iter->pTransform->Get_WorldMatrix_Ptr());

		LCUBEDIRECION eDirec = LCUBEDIRECION::LDIREC_END;
		_float3 fAdjust = GET_INSTANCE->Check_Terrain_Collision(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents(), iter->pTransform->Get_AdjustVector(), &eDirec);

		if ((fAdjust.x != -1) || (fAdjust.y != -1) || (fAdjust.z != -1))
		{
			Safe_Release(iter->pHost);
			Safe_Release(iter->pTransform);
			Safe_Release(iter->pCollider);
			Safe_Delete(iter);
			GET_INSTANCE->Destroy_Terrain_Explosion(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents().x);
		}
		else
		{
			CPhysicsManager::P_RESULT tResult = {};

			tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, false, false, false, fTimeDelta);
			GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);
		}
	}
}

void CBullet_Straight::Late_Update(_float fTimeDelta)
{
	for (list<INTER_INFO*>::iterator iter = m_Actionlist.begin(); iter != m_Actionlist.end();)
	{
		if (*iter == nullptr)
			iter = m_Actionlist.erase(iter);
		else
			++iter;
	}

	for (auto& iter : m_Actionlist)
		iter->pTransform->Save_BeforePosition();




	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBullet_Straight::Render()
{
	

	return S_OK;
}

HRESULT CBullet_Straight::Ready_Components()
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

HRESULT CBullet_Straight::Ready_PartObjects()
{
	return S_OK;
}

CBullet_Straight* CBullet_Straight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_Straight* pInstance = new CBullet_Straight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBullet_Straight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_Straight::Clone(void* pArg)
{
	CBullet_Straight* pInstance = new CBullet_Straight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBullet_Straight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_Straight::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
