#include "stdafx.h"
#include "..\Public\Container_Enemy.h"

#include "Body_Human.h"
#include "ClientInstance.h"
#include "Tool.h"

CContainer_Enemy::CContainer_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLongvinter_Container{ pDevice, pContext }
{
}

CContainer_Enemy::CContainer_Enemy(const CContainer_Enemy& Prototype)
	: CLongvinter_Container{ Prototype }
{
}

HRESULT CContainer_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainer_Enemy::Initialize(void* pArg)
{
	AI_Enemy_Desc* pTemp = static_cast<AI_Enemy_Desc*>(pArg);

	m_eEnemy_Type = pTemp->eType;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_iState = 13; // <-IDLE

	return S_OK;
}

void CContainer_Enemy::Priority_Update(_float fTimeDelta)
{



	for (auto& pPartObject : m_Parts)
		pPartObject->Priority_Update(fTimeDelta);


}

void CContainer_Enemy::Update(_float fTimeDelta)
{
	






	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());

	CPhysicsManager::P_RESULT tResult = {};

	tResult = GET_INSTANCE->Total_Physics(*m_pTransformCom, *m_pColliderCom, true, true, true, fTimeDelta);
	GET_INSTANCE->Update_By_P_Result(m_pTransformCom, m_pColliderCom, tResult);

	for (auto& pPartObject : m_Parts)
		pPartObject->Update(fTimeDelta);
}

void CContainer_Enemy::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_Parts)
		pPartObject->Late_Update(fTimeDelta);

	m_pTransformCom->Save_BeforePosition();




	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CContainer_Enemy::Render()
{
	m_pColliderCom->Render();

	return S_OK;
}

void CContainer_Enemy::Collision_Reaction_InterAction(CGameObject* pPoint,INTERACTION eIndex)
{
}

void CContainer_Enemy::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}


HRESULT CContainer_Enemy::Ready_Components()
{


	return S_OK;
}

HRESULT CContainer_Enemy::Ready_PartObjects()
{
	/* ���� �߰��ϰ� ���� ��Ʈ������Ʈ�� ������ŭ ����� �����س���. */
	m_Parts.resize(PART_END);

	CBody_Human::BODY_DESC		BodyDesc{};
	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	if (FAILED(__super::Add_PartObject(PART_BODY, TEXT("Prototype_GameObject_Body_Human"), &BodyDesc)))
		return E_FAIL;

	CTool::TOOL_DESC		ToolDesc{};
	ToolDesc.pParentState = &m_iState;
	ToolDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();





	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("head");

	if (FAILED(__super::Add_PartObject(PART_HEAD, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;



	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("spine_01");

	if (FAILED(__super::Add_PartObject(PART_BACKPACK, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("ik_hand_gun");

	if (FAILED(__super::Add_PartObject(PART_GUN, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("Hand_Left");

	if (FAILED(__super::Add_PartObject(PART_ARM_LEFT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("Hand_Right");

	if (FAILED(__super::Add_PartObject(PART_ARM_RIGHT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("ik_foot_l");

	if (FAILED(__super::Add_PartObject(PART_LEG_LEFT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("ik_foot_r");

	if (FAILED(__super::Add_PartObject(PART_LEG_RIGHT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;





	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(0.5f, 1.0f, 0.5f);
	ColliderDesc.vCenter = _float3(0.0f, ColliderDesc.vExtents.y, 0.0f);

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	static_cast<CBody_Human*>(m_Parts[PART_BODY])->Set_Human_Body(HUMAN_BODY(m_iBody));
	static_cast<CBody_Human*>(m_Parts[PART_BODY])->Set_Human_Face(HUMAN_FACE(m_iFace));

	return S_OK;
}

CContainer_Enemy* CContainer_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CContainer_Enemy* pInstance = new CContainer_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CContainer_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CContainer_Enemy::Clone(void* pArg)
{
	CContainer_Enemy* pInstance = new CContainer_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CContainer_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CContainer_Enemy::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}