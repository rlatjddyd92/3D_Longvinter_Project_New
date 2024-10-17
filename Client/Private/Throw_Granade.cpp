#include "stdafx.h"
#include "Throw_Granade.h"
#include "ClientInstance.h"

CThrow_Granade::CThrow_Granade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CThrow_Granade::CThrow_Granade(const CThrow_Granade& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CThrow_Granade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CThrow_Granade::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fSpec_Extent = { 0.1f,0.1f,0.1f };
	m_fSpec_Scale = 1.f;
	m_fSpec_PushedPower = 20.f;
	m_fSpec_PushedPower_Decrease = 0.5f;
	m_iColliderType = _int(CCollider::TYPE_OBB);
	m_fSpec_Time = 2.5f;

	return S_OK;
}

void CThrow_Granade::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_fShowTime > 0.f)
	{
		m_fShowTime -= fTimeDelta;
		if (m_fShowTime < 0.f)
		{
			m_fShowTime = 0.f;
		}
	}
}

void CThrow_Granade::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	for (auto& iter : m_Actionlist)
	{
		CPhysicsManager::P_RESULT tResult = {};

		_vector vOrigin = iter->pTransform->Get_State(CTransform::STATE_POSITION);

		tResult = GET_INSTANCE->Bounce_Physics(*iter->pTransform, *iter->pCollider, true, fTimeDelta);
		GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

		_vector vNow = iter->pTransform->Get_State(CTransform::STATE_POSITION);

	




		iter->fTime -= fTimeDelta;

		if (iter->fTime < 0.f)
		{
			GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_EXPLOSION_NORMAL, iter->pHost, iter->pCollider->GetBoundingCenter(), { 0.f,0.f,0.f });
			iter->bDead = true;
		}
	}
}

void CThrow_Granade::Late_Update(_float fTimeDelta)
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

HRESULT CThrow_Granade::Render()
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
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
				return E_FAIL;

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
void CThrow_Granade::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CThrow_Granade::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CThrow_Granade::Ready_Components()
{


	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Throw_Granade_Unpinned"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CThrow_Granade::Ready_PartObjects()
{
	return S_OK;
}

CThrow_Granade* CThrow_Granade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CThrow_Granade* pInstance = new CThrow_Granade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CThrow_Granade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThrow_Granade::Clone(void* pArg)
{
	CThrow_Granade* pInstance = new CThrow_Granade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CThrow_Granade"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThrow_Granade::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
