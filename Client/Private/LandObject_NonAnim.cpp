#include "stdafx.h"
#include "LandObject_NonAnim.h"
#include "ClientInstance.h"


CLandObject_NonAnim::CLandObject_NonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{
}

CLandObject_NonAnim::CLandObject_NonAnim(const CLandObject_NonAnim& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CLandObject_NonAnim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject_NonAnim::Initialize(void* pArg)
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

void CLandObject_NonAnim::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


}

void CLandObject_NonAnim::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
	{
		if ((!m_bSettingComplete) || (m_eType == INTERACTION::INTER_ITEM))
		{
			CPhysicsManager::P_RESULT tResult = {};

			tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, true, true, false, fTimeDelta);
			GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

			if (GET_INSTANCE->Check_OnGround(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
				m_bSettingComplete = true;
		}
		else
		{
			if (!GET_INSTANCE->Check_OnGround(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
				DestroyAction(iter);
		}


	}
}

void CLandObject_NonAnim::Late_Update(_float fTimeDelta)
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

HRESULT CLandObject_NonAnim::Render()
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

		_uint		iNumMeshes = m_vecModelCom[iter->iIndex]->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (m_bTexture)
				m_vecModelCom[iter->iIndex]->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i);


			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_vecModelCom[iter->iIndex]->Render(i)))
				return E_FAIL;
		}
#ifdef _DEBUG
		iter->pCollider->Render();
#endif
	}

	return S_OK;
}

void CLandObject_NonAnim::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);

	if (eIndex == INTERACTION::INTER_EXPLOSION_NORMAL)
	{
		DestroyAction(pAction);
	}





}

void CLandObject_NonAnim::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);

	if (eIndex == CONTAINER::CONTAINER_PLAYER)
	{
		
	}
}

HRESULT CLandObject_NonAnim::SetLandObject(INTERACTION eIndex)
{
	m_fSpec_PushedPower = 0.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = CCollider::TYPE_AABB;
	m_fSpec_Time = 0.f;
	m_fSpec_Sensor = 1.f;
	m_eType = eIndex;

	if (eIndex == INTERACTION::INTER_TREE)
	{
		// 나무
		m_vecModelCom.resize(2);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Tree_Fine"), TEXT("Com_Model_0"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Tree_Normal"), TEXT("Com_Model_1"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,1.f,0.2f };
		m_fSpec_Scale = 1.f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_BUSH)
	{
		// 덤불
		m_vecModelCom.resize(3);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_1"), TEXT("Com_Model_2"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_2"), TEXT("Com_Model_3"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_3"), TEXT("Com_Model_4"), reinterpret_cast<CComponent**>(&m_vecModelCom[2]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.5f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_ROCK)
	{
		// 바위
		m_vecModelCom.resize(8);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_1"), TEXT("Com_Model_5"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_2"), TEXT("Com_Model_6"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_3"), TEXT("Com_Model_7"), reinterpret_cast<CComponent**>(&m_vecModelCom[2]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_4"), TEXT("Com_Model_8"), reinterpret_cast<CComponent**>(&m_vecModelCom[3]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_5"), TEXT("Com_Model_9"), reinterpret_cast<CComponent**>(&m_vecModelCom[4]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_6"), TEXT("Com_Model_10"), reinterpret_cast<CComponent**>(&m_vecModelCom[5]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_7"), TEXT("Com_Model_11"), reinterpret_cast<CComponent**>(&m_vecModelCom[6]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_8"), TEXT("Com_Model_12"), reinterpret_cast<CComponent**>(&m_vecModelCom[7]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.2f;
		m_bTexture = true;
	}
	else if (eIndex == INTERACTION::INTER_APPLETREE)
	{
		// 사과나무
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_AppleTree"), TEXT("Com_Model_13"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,1.f,0.2f };
		m_fSpec_Scale = 1.f;
		m_bTexture = true;
	}
	else if (eIndex == INTERACTION::INTER_ITEM)
	{
		// 아이템
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_ItemPocket"), TEXT("Com_Model_14"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.2f;
		m_bTexture = false;
	}















	

	






	



}

void CLandObject_NonAnim::DestroyAction(INTER_INFO* pAction)
{
	//pAction->bDead = true; 
}

HRESULT CLandObject_NonAnim::Ready_Components()
{

	


	return S_OK;
}

HRESULT CLandObject_NonAnim::Ready_PartObjects()
{
	return S_OK;
}

CLandObject_NonAnim* CLandObject_NonAnim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLandObject_NonAnim* pInstance = new CLandObject_NonAnim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandObject_NonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandObject_NonAnim::Clone(void* pArg)
{
	CLandObject_NonAnim* pInstance = new CLandObject_NonAnim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLandObject_NonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandObject_NonAnim::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	for (auto& iter : m_vecModelCom)
		Safe_Release(iter);

	m_vecModelCom.clear();
}
