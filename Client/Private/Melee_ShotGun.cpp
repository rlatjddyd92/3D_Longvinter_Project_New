#include "stdafx.h"
#include "Melee_ShotGun.h"
#include "ClientInstance.h"

CMelee_ShotGun::CMelee_ShotGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_ShotGun::CMelee_ShotGun(const CMelee_ShotGun& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_ShotGun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee_ShotGun::Initialize(void* pArg)
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
	m_fSpec_PushedPower = 20.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = _int(CCollider::TYPE_SPHERE);
	m_fSpec_Time = 0.1f;
	m_bTransParent = true;
	m_fAlpah = 0.5f;

	return S_OK;
}

void CMelee_ShotGun::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CMelee_ShotGun::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
	{
		iter->fTime -= fTimeDelta;

		if (iter->fTime <= 0.f)
		{
			iter->bDead = true;
			continue;
		}

		/*if (GET_INSTANCE->Check_Terrain_Collision(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
		{
			iter->bDead = true;
			continue;
		}*/


		CPhysicsManager::P_RESULT tResult = {};

		tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, false, false, false, fTimeDelta);
		GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

		_float fScale = iter->pTransform->Get_Scaled().x * 1.2f;
		iter->pTransform->Set_Scaled(fScale, fScale, fScale);
		iter->pCollider->Update(iter->pTransform->Get_WorldMatrix_Ptr());
	}
}

void CMelee_ShotGun::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMelee_ShotGun::Render()
{
	__super::Render();

	for (auto& iter : m_Actionlist)
	{
		_float3 fPosition{};
		XMStoreFloat3(&fPosition, iter->pTransform->Get_State(CTransform::STATE_POSITION));

		if (!GET_INSTANCE->GetIsLender(fPosition))
			continue;




		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &iter->pTransform->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;



		if (FAILED(m_pShaderCom->Bind_ChangeAlpah("g_Istransparency", "g_TransAlpah", &m_bTransParent, &m_fAlpah)))
			return E_FAIL;

		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
				return E_FAIL;*/

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
#ifdef _DEBUG
		iter->pCollider->Render();
#endif
	}

	return S_OK;
}

void CMelee_ShotGun::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CMelee_ShotGun::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}




HRESULT CMelee_ShotGun::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture_Alpah"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Explosion"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CMelee_ShotGun::Ready_PartObjects()
{
	return S_OK;
}

CMelee_ShotGun* CMelee_ShotGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee_ShotGun* pInstance = new CMelee_ShotGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee_ShotGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee_ShotGun::Clone(void* pArg)
{
	CMelee_ShotGun* pInstance = new CMelee_ShotGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee_ShotGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee_ShotGun::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
