#include "stdafx.h"
#include "Melee_ChainSaw.h"
#include "ClientInstance.h"

CMelee_ChainSaw::CMelee_ChainSaw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMelee{ pDevice, pContext }
{
}

CMelee_ChainSaw::CMelee_ChainSaw(const CMelee_ChainSaw& Prototype)
	: CMelee{ Prototype }
{
}

HRESULT CMelee_ChainSaw::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMelee_ChainSaw::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_Pushed_PowerDecrease(0.f); // <- 속도 감소 없음 
	m_fSpec_Extent = { 1.f,1.f,1.f };
	m_fSpec_Scale = 2.f;
	m_fSpec_PushedPower = 0.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = _int(CCollider::TYPE_SPHERE);

	m_bTransParent = true;
	m_fAlpah = 0.5f;

	return S_OK;
}

void CMelee_ChainSaw::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CMelee_ChainSaw::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
	{
		_float fSize = iter->pTransform->Get_Scaled().x * 1.15f;
		iter->pCollider->SetBoundingRadius_Sphere(fSize);
		iter->pCollider->Update(iter->pTransform->Get_WorldMatrix_Ptr());

		_float3 fDirec = { _float((rand() % 1000) - (rand() % 1000)) / 1000.f, 0.5f, _float((rand() % 1000) - (rand() % 1000)) / 1000.f };



		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_FIRE, nullptr, iter->pCollider->GetBoundingCenter(), fDirec);
		GET_INSTANCE->Destroy_Terrain_Explosion(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingRadius_Sphere());
		iter->pTransform->Set_Scaled(fSize, fSize, fSize);
		if (fSize > 3.f)
			iter->bDead = true;

	}
}

void CMelee_ChainSaw::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMelee_ChainSaw::Render()
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

void CMelee_ChainSaw::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CMelee_ChainSaw::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}




HRESULT CMelee_ChainSaw::Ready_Components()
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

HRESULT CMelee_ChainSaw::Ready_PartObjects()
{
	return S_OK;
}

CMelee_ChainSaw* CMelee_ChainSaw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMelee_ChainSaw* pInstance = new CMelee_ChainSaw(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMelee_ChainSaw"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMelee_ChainSaw::Clone(void* pArg)
{
	CMelee_ChainSaw* pInstance = new CMelee_ChainSaw(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMelee_ChainSaw"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMelee_ChainSaw::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
