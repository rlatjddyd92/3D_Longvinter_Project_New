#include "stdafx.h"
#include "Throw_Dinamite.h"
#include "ClientInstance.h"

CThrow_Dinamite::CThrow_Dinamite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CThrow_Dinamite::CThrow_Dinamite(const CThrow_Dinamite& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CThrow_Dinamite::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThrow_Dinamite::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fSpec_Extent = { 0.2f,0.2f,0.2f };
	m_fSpec_Scale = 1.f;
	m_fSpec_PushedPower = 10.f;
	m_fSpec_PushedPower_Decrease = 1.f;
	m_iColliderType = _int(CCollider::TYPE_AABB);

	return S_OK;
}

void CThrow_Dinamite::Priority_Update(_float fTimeDelta)
{
}

void CThrow_Dinamite::Update(_float fTimeDelta)
{

	for (auto& iter : m_Actionlist)
	{
		CPhysicsManager::P_RESULT tResult = {};

		_vector vOrigin = iter->pTransform->Get_State(CTransform::STATE_POSITION);

		tResult = GET_INSTANCE->Bounce_Physics(*iter->pTransform, *iter->pCollider, true, fTimeDelta);
		GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

		_vector vNow = iter->pTransform->Get_State(CTransform::STATE_POSITION);

		if (vOrigin.m128_f32[0] == vNow.m128_f32[0])
			if (vOrigin.m128_f32[1] == vNow.m128_f32[1])
				if (vOrigin.m128_f32[1] == vNow.m128_f32[1])
					m_bDinamiteActive = true;

	}
}

void CThrow_Dinamite::Late_Update(_float fTimeDelta)
{
	for (list<INTER_INFO*>::iterator iter = m_Actionlist.begin(); iter != m_Actionlist.end();)
	{
		if (*iter == nullptr)
			iter = m_Actionlist.erase(iter);
		else
			++iter;
	}

	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
		iter->pTransform->Save_BeforePosition();




	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CThrow_Dinamite::Render()
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
void CThrow_Dinamite::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CThrow_Dinamite::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CThrow_Dinamite::Ready_Components()
{


	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Throw_LandDinamite"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CThrow_Dinamite::Ready_PartObjects()
{
	return S_OK;
}

CThrow_Dinamite* CThrow_Dinamite::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CThrow_Dinamite* pInstance = new CThrow_Dinamite(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CThrow_Dinamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThrow_Dinamite::Clone(void* pArg)
{
	CThrow_Dinamite* pInstance = new CThrow_Dinamite(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CThrow_Dinamite"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThrow_Dinamite::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
