#include "stdafx.h"
#include "..\Public\Container_Player.h"

#include "Body_Human.h"
#include "ClientInstance.h"
#include "Tool.h"

CContainer_Player::CContainer_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CContainer_Player::CContainer_Player(const CContainer_Player& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CContainer_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainer_Player::Initialize(void* pArg)
{
	GAMEOBJECT_DESC*		pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_iState = 13; // <-IDLE


	GET_INSTANCE->Set_Player_Pointer(this);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());
	m_pTransformCom->Set_Pushed_PowerDecrease(0.01f);
	return S_OK;
}

void CContainer_Player::Priority_Update(_float fTimeDelta)
{
	

	
	for (auto& pPartObject : m_Parts)
		pPartObject->Priority_Update(fTimeDelta);

	
}

void CContainer_Player::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_DOWN, true) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);

		/*if (m_iState ^ 24)
		{
			m_iState = STATE_RESET;
		}
			

		m_iState = 24;*/
	}
	else if (m_pGameInstance->Get_DIKeyState(DIK_UP, true) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);

		/*if (m_iState ^ 24)
		{
			m_iState ^= STATE_RESET;
		}
			

		m_iState = 24;*/
	}
	else
	{
		/*m_iState = STATE_RESET;
		m_iState |= 13;*/
	}


	if (m_pGameInstance->Get_DIKeyState(DIK_LEFT, true) & 0x80)
		m_pTransformCom->Turn(false, true, false, fTimeDelta * -1.f);

	if (m_pGameInstance->Get_DIKeyState(DIK_RIGHT, true) & 0x80)
		m_pTransformCom->Turn(false, true, false, fTimeDelta);

	if (m_pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
		m_pTransformCom->Set_Pushed_Power(_float3(0.f, 1.f, 0.f), GRAVITY_ACCELE * 2.f);

	if (m_pGameInstance->Get_DIKeyState(DIK_K, true) & 0x80)
		m_pTransformCom->Go_Up(fTimeDelta);

	if (m_pGameInstance->Get_DIKeyState(DIK_M, true) & 0x80)
		m_pTransformCom->Go_Down(fTimeDelta);

	

	if (m_pGameInstance->Get_DIKeyState(DIK_5) & 0x80)
		++m_iState;

	if (m_pGameInstance->Get_DIKeyState(DIK_6) & 0x80)
		m_iState = 0;

	if (m_pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
		GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERA_FIRST);
	if (m_pGameInstance->Get_DIKeyState(DIK_E) & 0x80)
		GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERA_EDITOR);
	if (m_pGameInstance->Get_DIKeyState(DIK_T) & 0x80)
		GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERA_THIRD);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());

	CPhysicsManager::P_RESULT tResult = {};

	tResult = GET_INSTANCE->Total_Physics(*m_pTransformCom, *m_pColliderCom, true, true, true, fTimeDelta);
	GET_INSTANCE->Update_By_P_Result(m_pTransformCom, m_pColliderCom, tResult);

	for (auto& pPartObject : m_Parts)
		pPartObject->Update(fTimeDelta);
}

void CContainer_Player::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_Parts)
		pPartObject->Late_Update(fTimeDelta);

	m_pTransformCom->Save_BeforePosition();
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CContainer_Player::Render()
{
	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CContainer_Player::Ready_Components()
{
	

	return S_OK;
}

HRESULT CContainer_Player::Ready_PartObjects()
{
	/* 실제 추가하고 싶은 파트오브젝트의 갯수만큼 밸류를 셋팅해놓자. */
	m_Parts.resize(PART_END);

	CBody_Human::BODY_DESC		BodyDesc{};
	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	if (FAILED(__super::Add_PartObject(PART_BODY, TEXT("Prototype_GameObject_Body_Human"), &BodyDesc)))
		return E_FAIL;

	CTool::TOOL_DESC		ToolDesc{};
	ToolDesc.pParentState = &m_iState;
	ToolDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();


	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("BackpackSocket");

	if (FAILED(__super::Add_PartObject(PART_BACKPACK, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("MCH-upper_arm_parent_socket_L");

	if (FAILED(__super::Add_PartObject(PART_ARM_LEFT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("MCH-upper_arm_parent_socket_R");

	if (FAILED(__super::Add_PartObject(PART_ARM_RIGHT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("MCH-thigh_parent_socket_L");

	if (FAILED(__super::Add_PartObject(PART_LEG_LEFT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("MCH-thigh_parent_socket_R");

	if (FAILED(__super::Add_PartObject(PART_LEG_RIGHT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("Hand_Left_end");

	if (FAILED(__super::Add_PartObject(PART_HAND_LEFT, TEXT("Prototype_GameObject_Tool_Empty"), &ToolDesc)))
		return E_FAIL;

	ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("rightHand");

	if (FAILED(__super::Add_PartObject(PART_HAND_RIGHT, TEXT("Prototype_GameObject_Tool_ShotGun"), &ToolDesc)))
		return E_FAIL;



	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(0.5f, 1.0f, 0.5f);
	ColliderDesc.vCenter = _float3(0.0f, ColliderDesc.vExtents.y, 0.0f);

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_SINGLE), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CContainer_Player* CContainer_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CContainer_Player* pInstance = new CContainer_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CContainer_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CContainer_Player::Clone(void* pArg)
{
	CContainer_Player* pInstance = new CContainer_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CContainer_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CContainer_Player::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
