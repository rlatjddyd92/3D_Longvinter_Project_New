#include "stdafx.h"
#include "..\Public\Container_Player.h"

#include "Body_Human.h"
#include "Body.h"
#include "ClientInstance.h"
#include "Tool.h"
#include "Tool_Head.h"

CContainer_Player::CContainer_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLongvinter_Container{ pDevice, pContext }
{
}

CContainer_Player::CContainer_Player(const CContainer_Player& Prototype)
	: CLongvinter_Container{ Prototype }
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

	m_iState = STATE_IDEL;

	eContainerType = CONTAINER::CONTAINER_PLAYER;


	GET_INSTANCE->Set_Player_Pointer(this);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());
	m_pTransformCom->Set_Pushed_PowerDecrease(1.f);
	m_pTransformCom->Set_Scaled(0.95f, 0.95f, 0.95f);

	GET_INSTANCE->Input_ContainerColliderPointer(eContainerType, static_cast<CLongvinter_Container*>(this), m_pColliderCom);

	m_fHp = 1000.f;
	m_fHp_Max = 1000.f;

	//CTool_Head::HEAD_DESC ToolDesc{};
	//ToolDesc.eType = CTool_Head::TYPE_NORMAL_1;
	//ToolDesc.pParentState = &m_iState;
	//ToolDesc.pParentWorldMatrix = &m_pTransformCom->Get_WorldMatrix();
	//ToolDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("head");

	//if (FAILED(__super::Add_PartObject(PART_HEAD, TEXT("Prototype_GameObject_Tool_Head"), &ToolDesc)))
	//	return E_FAIL;

	

	return S_OK;
}

void CContainer_Player::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	for (auto& pPartObject : m_Parts)
		pPartObject->Priority_Update(fTimeDelta);

}

void CContainer_Player::Update(_float fTimeDelta)
{
	
	__super::Update(fTimeDelta);
	
	Camera_Control(fTimeDelta);
	
	Moving_Control(fTimeDelta);
	
	Weapon_Control(fTimeDelta);
	
	Test_Control(fTimeDelta);
	
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());

	CPhysicsManager::P_RESULT tResult = {};
	
	tResult = GET_INSTANCE->Total_Physics(*m_pTransformCom, *m_pColliderCom, true, true, true, fTimeDelta);
	GET_INSTANCE->Update_By_P_Result(m_pTransformCom, m_pColliderCom, tResult);
	
	for (auto& pPartObject : m_Parts)
		pPartObject->Update(fTimeDelta);
	
}

void CContainer_Player::Late_Update(_float fTimeDelta)
{
	
	__super::Late_Update(fTimeDelta);
	
	if (m_bNonLoopAnimReset)
		static_cast<CBody*>(m_Parts[PART_BODY])->Start_NonLoopAnim();

	for (auto& pPartObject : m_Parts)
		pPartObject->Late_Update(fTimeDelta);

	if (bMove == false)
	{
		if (static_cast<CBody*>(m_Parts[PART_BODY])->GetEnd())
		{
			if (m_iState == STATE_GRANADE)
				m_iState = STATE_THROW_WAIT;
			else if ((m_iState == STATE_HANDGUN) || (m_iState == STATE_GUN))
				m_iState = STATE_AIM;
			else if (m_iState == STATE_CHAINSAW)
				m_iState = STATE_CHAINSAW;
			else if (m_iState == STATE_HIT)
				m_iState = STATE_IDEL;
		}

	}
	
	m_pTransformCom->Save_BeforePosition();
	
	const _float4x4* fSocket = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("Hand_Right");
	
	if (m_eWeaponType == WEAPON_MAIN)
		GET_INSTANCE->InputRenderlist(GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_MAINWEAPON).eIndex, &m_iState, fSocket, m_pTransformCom->Get_WorldMatrix());
	else if (m_eWeaponType == WEAPON_SUB)
		GET_INSTANCE->InputRenderlist(GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_SUBWEAPON).eIndex, &m_iState, fSocket, m_pTransformCom->Get_WorldMatrix());
	else if (m_eWeaponType == WEAPON_THROW)
		GET_INSTANCE->InputRenderlist(GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_THROW).eIndex, &m_iState, fSocket, m_pTransformCom->Get_WorldMatrix());
	
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	
	
	


}

HRESULT CContainer_Player::Render()
{
	//m_Parts[PART_HEAD]->Render();
	m_pColliderCom->Render();

	return S_OK;
}

void CContainer_Player::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
}

void CContainer_Player::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
}

void CContainer_Player::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}

void CContainer_Player::DeadAction()
{
	__super::DeadAction();
}


void CContainer_Player::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);
	
	if (GET_INSTANCE->GetCameraMode() == CAMERAMODE::CAMERA_FIRST)
	{
		

		// 이동 조작 
		if (m_pGameInstance->Get_DIKeyState(DIK_W, true) & 0x80)
		{
			m_pTransformCom->Go_Straight(fTimeDelta, true);
			m_iState = STATE_WALK;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S, true) & 0x80)
		{
			m_pTransformCom->Go_Backward(fTimeDelta, true);
			m_iState = STATE_WALK;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_A, true) & 0x80)
		{
			m_pTransformCom->Go_Left(fTimeDelta, true);
			m_iState = STATE_WALK_L;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D, true) & 0x80)
		{
			m_pTransformCom->Go_Right(fTimeDelta, true);
			m_iState = STATE_WALK_R;
			bMove = true;
		}

		if (GET_INSTANCE->Check_OnGround(m_pColliderCom->GetBoundingCenter(), m_pColliderCom->GetBoundingExtents()))
		{
			if (m_bJump)
			{
				m_bJump = false;
				GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_JUMP, SOUND_CHANNEL::CH_PLAYER_ACT, 10.f, m_pColliderCom->GetBoundingCenter());
			}

			if (m_pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
			{
				m_pTransformCom->Set_Pushed_Power(_float3(0.f, 1.f, 0.f), GRAVITY_ACCELE * 2.f);
				m_bJump = true;
			}
		}
	
		

	
		
		
		_long		MouseMove = { 0 };

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.1f * fTimeDelta * MouseMove);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			//m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove);
		}

		
	}
	else if (GET_INSTANCE->GetCameraMode() == CAMERAMODE::CAMERA_THIRD)
	{
		

		// 이동 조작 
		if (m_pGameInstance->Get_DIKeyState(DIK_W, true) & 0x80)
		{
			m_pTransformCom->Go_Straight(fTimeDelta, true);
			m_iState = STATE_WALK;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S, true) & 0x80)
		{
			m_pTransformCom->Go_Backward(fTimeDelta, true);
			m_iState = STATE_WALK;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_A, true) & 0x80)
		{
			m_pTransformCom->Go_Left(fTimeDelta, true);
			m_iState = STATE_WALK_L;
			bMove = true;
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D, true) & 0x80)
		{
			m_pTransformCom->Go_Right(fTimeDelta, true);
			m_iState = STATE_WALK_R;
			bMove = true;
		}
		
		if (GET_INSTANCE->Check_OnGround(m_pColliderCom->GetBoundingCenter(), m_pColliderCom->GetBoundingExtents()))
		{
			if (m_bJump)
			{
				m_bJump = false;
				GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_JUMP, SOUND_CHANNEL::CH_PLAYER_ACT, 10.f, m_pColliderCom->GetBoundingCenter());
			}

			if (m_pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
			{
				m_pTransformCom->Set_Pushed_Power(_float3(0.f, 1.f, 0.f), GRAVITY_ACCELE * 2.f);
				m_bJump = true;
			}
		}
		
		_float3 fPoint = GET_INSTANCE->CheckPicking();
		fPoint.x -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
		fPoint.z -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2];

		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		_float fDot = XMVector3Dot({ fPoint.x, 0.f, fPoint.z, 0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2], 0.f }).m128_f32[0];

		_float fCos = (sqrt(pow(fPoint.x, 2) + pow(fPoint.z, 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));

		_float fAngle = acos(fDot / fCos);

		if (isnan(fAngle))
			fAngle = 0.f;

		_bool bResult = GET_INSTANCE->Check_CCW_XZ(fPoint, { 0.f,0.f,0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2] });
		
		if (bResult)
			fAngle *= -1;

		_long		MouseMove = { 0 };
		
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fAngle);
	}

	
}

void CContainer_Player::Weapon_Control(_float fTimeDelta)
{
	__super::Weapon_Control(fTimeDelta);
	// 무기 조작 

	if (m_pGameInstance->Get_DIKeyState(DIK_1) & 0x80)
	{
		m_eWeaponType = WEAPON_MAIN;
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_WEAPONSLOT, SOUND_CHANNEL::CH_SYSTEM_UI, 10.f);
		GET_INSTANCE->ShowInformMessage(TEXT("주 무기로 변경"));
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_2) & 0x80)
	{
		m_eWeaponType = WEAPON_SUB;
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_WEAPONSLOT, SOUND_CHANNEL::CH_SYSTEM_UI, 10.f);
		GET_INSTANCE->ShowInformMessage(TEXT("보조 무기로 변경"));
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_3) & 0x80)
	{
		m_eWeaponType = WEAPON_THROW;
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_WEAPONSLOT, SOUND_CHANNEL::CH_SYSTEM_UI, 10.f);
		GET_INSTANCE->ShowInformMessage(TEXT("투척 무기로 변경"));
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_4) & 0x80)
	{
		m_eWeaponType = WEAPON_END;
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_WEAPONSLOT, SOUND_CHANNEL::CH_SYSTEM_UI, 10.f);
		GET_INSTANCE->ShowInformMessage(TEXT("무기 해제"));
	}

	ITEMINDEX eNowType = ITEMINDEX::ITEM_END;

	if (m_eWeaponType == WEAPON_MAIN)
		eNowType = GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_MAINWEAPON).eIndex;
	if (m_eWeaponType == WEAPON_SUB)
		eNowType = GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_SUBWEAPON).eIndex;
	if (m_eWeaponType == WEAPON_THROW)
		eNowType = GET_INSTANCE->GetEquipInfo(EQUIPSLOT::SLOT_THROW).eIndex;

	

	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON, true) & 0x80) 
	{
		if (eNowType == ITEMINDEX::ITEM_CHAINSAW)
			m_iState = STATE_CHAINSAW;
		else if (eNowType == ITEMINDEX::ITEM_ARROW)
			m_iState = STATE_HANDGUN;
		else if (eNowType == ITEMINDEX::ITEM_MACHINEGUN)
			m_iState = STATE_GUN;
		else if (eNowType == ITEMINDEX::ITEM_SHOTGUN)
			m_iState = STATE_GUN;
		else if (eNowType == ITEMINDEX::ITEM_FIRETHROWER)
			m_iState = STATE_AIM;
		else if (eNowType == ITEMINDEX::ITEM_GRANADE)
			m_iState = STATE_GRANADE;
		else if (eNowType == ITEMINDEX::ITEM_MACHETE)
			m_iState = STATE_HIT;

		if (m_fAttackDelay == 0.f)
		{
			m_bNonLoopAnimReset = true;

			if (m_iState == STATE_GRANADE)
			{
				m_fPreAttackDelay = 1.3f;
			}
			else if (m_iState == STATE_HIT)
			{
				m_fAttackDelay = 0.5f;
				m_fPreAttackDelay = 0.5f;
			}
			else 
			{
				_float3 fStartPostion{};
				_float3 fPushedDirec{};
				_vector vStartPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 1.2f + _vector{ 0.f, 1.f, 0.f, 0.f };
				XMStoreFloat3(&fStartPostion, vStartPosition);

				if (GET_INSTANCE->GetCameraMode() == CAMERAMODE::CAMERA_FIRST)
					XMStoreFloat3(&fPushedDirec, GET_INSTANCE->GetCameraLook());
				else
					XMStoreFloat3(&fPushedDirec, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

				if (eNowType == ITEMINDEX::ITEM_MACHINEGUN)
					GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_SHOT_MACHINEGUN, SOUND_CHANNEL::CH_PLAYER_WEAPON, 10.f, m_pColliderCom->GetBoundingCenter());
				else if (eNowType == ITEMINDEX::ITEM_LANDMINE)
					GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_THROW_GRANADE, SOUND_CHANNEL::CH_PLAYER_WEAPON, 10.f, m_pColliderCom->GetBoundingCenter());
				else if (eNowType == ITEMINDEX::ITEM_MACHETE)
					GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_MELEE, SOUND_CHANNEL::CH_PLAYER_WEAPON, 10.f, m_pColliderCom->GetBoundingCenter());
				else if (eNowType == ITEMINDEX::ITEM_SHOTGUN)
					GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_SHOT_SHOTGUN, SOUND_CHANNEL::CH_PLAYER_WEAPON, 10.f, m_pColliderCom->GetBoundingCenter());

				__super::UsingWeapon(eNowType, fStartPostion, fPushedDirec);
			}
		}
	}

	if (m_fPreAttackDelay > 0.f)
	{
		m_fPreAttackDelay -= fTimeDelta;

		if (m_fPreAttackDelay <= 0.f)
		{
			_float3 fStartPostion{};
			_float3 fPushedDirec{};
			_vector vStartPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 2.f + _vector{ 0.f, 1.f, 0.f, 0.f };
			XMStoreFloat3(&fStartPostion, vStartPosition);

			if (GET_INSTANCE->GetCameraMode() == CAMERAMODE::CAMERA_FIRST)
				XMStoreFloat3(&fPushedDirec, GET_INSTANCE->GetCameraLook());
			else
				XMStoreFloat3(&fPushedDirec, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			if (eNowType == ITEMINDEX::ITEM_GRANADE)
				GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_THROW_GRANADE, SOUND_CHANNEL::CH_PLAYER_WEAPON, 10.f, m_pColliderCom->GetBoundingCenter());

			__super::UsingWeapon(eNowType, fStartPostion, fPushedDirec);

			m_fPreAttackDelay = 0.f;
		}

		
	}






	if (bMove == false)
		if ((m_iState != STATE_GRANADE) && (m_iState != STATE_HANDGUN))
			if ((m_iState != STATE_GUN) && (m_iState != STATE_HIT))
	{
		if (eNowType == ITEMINDEX::ITEM_CHAINSAW)
			m_iState = STATE_CHAINSAW;
		else if (eNowType == ITEMINDEX::ITEM_ARROW)
			m_iState = STATE_AIM;
		else if (eNowType == ITEMINDEX::ITEM_MACHINEGUN)
			m_iState = STATE_AIM;
		else if (eNowType == ITEMINDEX::ITEM_SHOTGUN)
			m_iState = STATE_AIM;
		else if (eNowType == ITEMINDEX::ITEM_FIRETHROWER)
			m_iState = STATE_AIM;
		else if (eNowType == ITEMINDEX::ITEM_GRANADE)
			m_iState = STATE_THROW_WAIT;
		else if (eNowType == ITEMINDEX::ITEM_GRANADE)
			m_iState = STATE_THROW_WAIT;
		else 
			m_iState = STATE_IDEL;
	}
}

void CContainer_Player::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);

	if (m_pGameInstance->Get_DIKeyState(DIK_B) & 0x80)
	{
		GET_INSTANCE->SetCameraMode(CAMERAMODE::CAMERA_FIRST);
		GET_INSTANCE->ShowInformMessage(TEXT("B키 : 숄더뷰 모드"));
		GET_INSTANCE->SetLenderLength(100.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_N) & 0x80)
	{
		GET_INSTANCE->SetCameraMode(CAMERAMODE::CAMERA_EDITOR);
		GET_INSTANCE->ShowInformMessage(TEXT("N키 : 에디터 카메라 모드"));
		GET_INSTANCE->SetLenderLength(100.f);
	}
	if (m_pGameInstance->Get_DIKeyState(DIK_M) & 0x80)
	{
		GET_INSTANCE->SetCameraMode(CAMERAMODE::CAMERA_THIRD);
		GET_INSTANCE->ShowInformMessage(TEXT("M키 : 3인칭 모드"));
		GET_INSTANCE->SetLenderLength(100.f);
	}
}

void CContainer_Player::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);

	if (m_pGameInstance->Get_DIKeyState(DIK_5) & 0x80)
	{
		++m_iState;
		
		wstring strNum = to_wstring(m_iState);
		GET_INSTANCE->ShowInformMessage(strNum);
	}
		

	if (m_pGameInstance->Get_DIKeyState(DIK_6) & 0x80)
	{
		m_iState = 0;

		wstring strNum = to_wstring(m_iState);
		GET_INSTANCE->ShowInformMessage(strNum);
	}
		



	if (m_pGameInstance->Get_DIKeyState(DIK_0, false) & 0x80)
	{
		if (GET_INSTANCE->GetMonsterMake())
		{
			GET_INSTANCE->SetMakeMonster(false);
			GET_INSTANCE->ShowInformMessage(TEXT("몬스터 생성기 OFF"));
		}
		else 
		{
			GET_INSTANCE->SetMakeMonster(true);
			GET_INSTANCE->ShowInformMessage(TEXT("몬스터 생성기 ON"));
		}
	}
}

void CContainer_Player::Set_AI_Status(_float fTimeDelta)
{
	//__super::Set_AI_Status(fTimeDelta);
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

	CTool_Head::HEAD_DESC		HeadDesc{};
	HeadDesc.eType = CTool_Head::HEAD_TYPE::TYPE_NORMAL_1;
	HeadDesc.pParentState = &m_iState;
	HeadDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	HeadDesc.pSocketBoneMatrix = dynamic_cast<CBody_Human*>(m_Parts[PART_BODY])->Get_BoneMatrix_Ptr("head");

	if (FAILED(__super::Add_PartObject(PART_HEAD, TEXT("Prototype_GameObject_Tool_Head"), &HeadDesc)))
		return E_FAIL;

	CTool::TOOL_DESC		ToolDesc{};
	ToolDesc.pParentState = &m_iState;
	ToolDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

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
