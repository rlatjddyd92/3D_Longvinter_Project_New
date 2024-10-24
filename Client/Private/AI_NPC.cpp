#include "stdafx.h"
#include "AI_NPC.h"
#include "ClientInstance.h"

CAI_NPC::CAI_NPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainer_NPC{ pDevice, pContext }
{
}

CAI_NPC::CAI_NPC(const CAI_NPC& Prototype)
	: CContainer_NPC{ Prototype }
{
}

HRESULT CAI_NPC::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CAI_NPC::Initialize(void* pArg)
{
	AI_NPC_Desc* pTemp = static_cast<AI_NPC_Desc*>(pArg);

	eContainerType = CONTAINER::CONTAINER_NPC;
	m_eNPC_Type = pTemp->eType;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	m_iBody = _int(HUMAN_BODY::BODY_YELLOW);

	GET_INSTANCE->MakeEnemyHpBar(this);
	GET_INSTANCE->MakeSymbol(this);

	m_fDetective_Length = 20.f;

	m_fHp = 100.f;
	m_fHp_Max = 100.f;


	return S_OK;
}

void CAI_NPC::Priority_Update(_float fTimeDelta)
{



	__super::Priority_Update(fTimeDelta);

	

	Set_AI_Status(fTimeDelta);
}

void CAI_NPC::Update(_float fTimeDelta)
{
	if (m_fHp <= 0.f)
	{
			DeadAction();

	}


	if (m_fDistance_from_Player < m_fInterActionRange)
	{
		if (m_eNPC_Type == NPC_TYPE::NPC_SHOP)
		{
			_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("상인"), TEXT("E키 : 퀘스트"), TEXT("F키 : 거래"), TEXT("없음"));

			if (bActive)
			{
				if (m_pGameInstance->Get_DIKeyState(DIK_E))
				{
					GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
				else if (m_pGameInstance->Get_DIKeyState(DIK_F))
					GET_INSTANCE->ShowInformMessage(TEXT("[TEST] 구매 기능 작동"));
			}

		}
		else if (m_eNPC_Type == NPC_TYPE::NPC_INTERN)
		{
			if (m_iScriptNum == 0)
			{
				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("???"), TEXT("E키 : 대화하기"), TEXT("없음"), TEXT("없음"));

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
			}
			else
			{
				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("황건적 인턴"), TEXT("E키 : 대화하기"), TEXT("없음"), TEXT("없음"));

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
			}
			

		}
		if (m_eNPC_Type == NPC_TYPE::NPC_LAST)
		{
			if (m_iScriptNum < 3)
			{
				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("육식주의자"), TEXT("E키 : 대화하기"), TEXT("없음"), TEXT("없음"));

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
			}
			else if (m_iScriptNum == 3)
			{
				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("진상"), TEXT("E키 : 대화하기"), TEXT("없음"), TEXT("없음"));

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
			}
			else if ((m_iScriptNum == 4) && (m_eAI_Status == AI_STATUS::AI_DEAD))
			{
				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("진상"), TEXT("E키 : 유언듣기"), TEXT("없음"), TEXT("없음"));

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->OpenTalkPage(static_cast<CContainer_NPC*>(this));
				}
			}

		}

	}

	if (m_eNPC_Type == NPC_TYPE::NPC_INTERN)
		if (m_iScriptNum == 4)
		{
			m_eAI_Status = AI_STATUS::AI_SERACH;
			GET_INSTANCE->SetMakeMonster(true);
			m_iScriptNum = 5;
		}

	if (m_eNPC_Type == NPC_TYPE::NPC_LAST)
	{
		if (m_iScriptNum == 3)
		{

		}
		if (m_iScriptNum == 4)
		{
			// 도망 기능 

			if (m_eAI_Status == AI_STATUS::AI_IDLE)
				m_eAI_Status = AI_STATUS::AI_SERACH;


			if (m_eAI_Status != AI_STATUS::AI_DEAD)
				if (GET_INSTANCE->GetOpenTalk())
					GET_INSTANCE->OpenTalkPage(nullptr);
		}
		else if (m_iScriptNum == 5)
		{
			m_fLastExplosion -= fTimeDelta;
			if (m_fLastExplosion < 0.f)
			{
				__super::SetDead();
				GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_EXPLOSION_NORMAL, nullptr, m_pColliderCom->GetBoundingCenter(), { 0.f,0.f,0.f }, 0.f, 10.f);
			}
		}
	}


	if (m_eAI_Status == AI_STATUS::AI_DEAD)
	{
		m_pTransformCom->Turn({ -1.f,0.f,0.f }, fTimeDelta * m_fMove_Angle);


		_float3 fExtent = m_pColliderCom->GetBoundingExtents();

		fExtent.y -= ((fTimeDelta * m_fMove_Angle / _float(PI_DEFINE)) * fExtent.y);

		if (fExtent.y < 0.5f)
			fExtent.y = 0.5f;

		m_pColliderCom->SetBoundingExtents({ fExtent.x, fExtent.y - ((fTimeDelta * m_fMove_Angle / _float(PI_DEFINE)) * fExtent.y), fExtent.z });


		m_fMove_Angle -= fTimeDelta * m_fMove_Angle;
	}
	else
		Moving_Control(fTimeDelta);




	__super::Update(fTimeDelta);

}

void CAI_NPC::Late_Update(_float fTimeDelta)
{



	__super::Late_Update(fTimeDelta);
}

HRESULT CAI_NPC::Render()
{
	__super::Render();


	return S_OK;
}

void CAI_NPC::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	if (m_eAI_Status != AI_STATUS::AI_SERACH) return;

	__super::Collision_Reaction_InterAction(pPoint, eIndex, tOpponent);

	CONTAINER eType = CONTAINER::CONTAINER_END;

	if (pPoint)
	{
		CLongvinter_Container* pOpponent = static_cast<CLongvinter_Container*>(pPoint);
		eType = pOpponent->GetContainerType();
	}

	if (eIndex == INTERACTION::INTER_FIRE)
	{
		
		if (!m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
			__super::Burning();
	}

	if (eIndex == INTERACTION::INTER_EXPLOSION_NORMAL)
	{
		
		__super::Add_Hp(-10.f);
		_vector vDirec = XMLoadFloat3(&m_pColliderCom->GetBoundingCenter()) - XMLoadFloat3(&tOpponent.pCollider->GetBoundingCenter()) + _vector{ 0.f, 0.2f, 0.f, 0.f };
		_float3 fDirec{};
		XMStoreFloat3(&fDirec, vDirec);
		m_pTransformCom->Set_Pushed_Power(fDirec, GRAVITY_ACCELE * 4.f);

		if (!m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
			__super::Burning();
	}

	if (eType == CONTAINER::CONTAINER_PLAYER)
	{
		m_fActionTimer = 0.5f;

		m_iFace = _int(HUMAN_FACE::FACE_SAD);
		static_cast<CBody_Human*>(m_Parts[PART_BODY])->Set_Human_Face(HUMAN_FACE(m_iFace));

		for (_int i = 0; i < 10; ++i)
			GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_HIT, m_pColliderCom->GetBoundingCenter());

		if (eIndex == INTERACTION::INTER_BULLET_MACHINEGUN)
		{
			__super::Add_Hp(-10.f);


			_float3 fDirec = tOpponent.pTransform->Get_Pushed_Dir();
			fDirec.y = 0.7f;

			m_pTransformCom->Set_Pushed_Power(fDirec, GRAVITY_ACCELE * 0.5f);
		}
		else if (eIndex == INTERACTION::INTER_MELEE_SHOTGUN)
		{
			if (pPoint == nullptr)
				return;



			__super::Add_Hp(-30.f);

			_float3 fDirec = tOpponent.pTransform->Get_Pushed_Dir();
			fDirec.y = 0.1f;

			m_pTransformCom->Set_Pushed_Power(fDirec, GRAVITY_ACCELE * 3.f);
		}

	}










}

void CAI_NPC::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	//if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	__super::Collision_Reaction_MadeInterAction(pPoint, eIndex);
}

void CAI_NPC::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	//if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	__super::Collision_Reaction_Container(pPoint, eIndex);






}

void CAI_NPC::DeadAction()
{
	__super::DeadAction();

	if (m_eAI_Status == AI_STATUS::AI_DEAD)
		return;

	m_eAI_Status = AI_STATUS::AI_DEAD;
	m_fActionTimer = 3.f;
	m_fMove_Angle = PI_DEFINE * 0.15f;
	m_iState = STATE_IDEL;
}



void CAI_NPC::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);

	

	if (m_eAI_Status == AI_STATUS::AI_IDLE)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fMove_Angle);
	}
	else if (m_eAI_Status == AI_STATUS::AI_PANIC)
	{
		if (m_fMove_Angle == 0.f)
			if (_int(m_fSearch_Time_Now / m_fSearch_Interval) > m_iSearch_Count)
			{

				m_fMove_Angle = m_pGameInstance->Get_Random(2.f, 4.f);

				if (m_pGameInstance->Get_Random(0.f, 2.f) > 1.f)
					m_fMove_Angle *= -1;


				++m_iSearch_Count;
			}

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMove_Angle);


		_float3 fLook{};
		XMStoreFloat3(&fLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		if (GET_INSTANCE->Check_OnGround(m_pColliderCom->GetBoundingCenter(), m_pColliderCom->GetBoundingExtents()))
		{
			m_bJump = false;

			if (GET_INSTANCE->Check_Wall(m_pColliderCom->GetBoundingCenter(), fLook, max(m_pColliderCom->GetBoundingExtents().x, m_pColliderCom->GetBoundingExtents().z) * 1.2f))
			{
				m_pTransformCom->Set_Pushed_Power(_float3(0.f, 1.f, 0.f), GRAVITY_ACCELE * 2.f);
				m_bJump = true;
			}
			else if (rand() % 10 == 0)
			{
				m_pTransformCom->Set_Pushed_Power(_float3(0.f, 1.f, 0.f), GRAVITY_ACCELE * 2.f);
				m_bJump = true;
			}
			bMove = true;
		}


		m_pTransformCom->Go_Straight(fTimeDelta * 1.5f, true);
		m_iState = STATE_WALK;
	}
	else if (m_eAI_Status == AI_STATUS::AI_SERACH)
	{

	}
}

void CAI_NPC::Weapon_Control(_float fTimeDelta)
{
	__super::Weapon_Control(fTimeDelta);


}

void CAI_NPC::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);
}

void CAI_NPC::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);
}

void CAI_NPC::Set_AI_Status(_float fTimeDelta)
{
	__super::Set_AI_Status(fTimeDelta);



	if (m_eAI_Status == AI_STATUS::AI_DEAD)
	{
		Update_Distance_From_Player();
		return;
	}

	if (m_eAI_Status == AI_STATUS::AI_SERACH)
	{
		Update_Distance_From_Player();
		return;
	}
		

	if (m_eAI_Status == AI_STATUS::AI_PANIC)
	{
		Update_Distance_From_Player();
		return;
	}

	m_fMove_Angle = 0.f;

	

	__super::Get_Sound(&m_fSoundPosition, &m_fVolume, &m_fMove_Angle);

	_float3 fPlayerPosition{};
	_bool bCanSee = false;

	__super::Look_Player(&fPlayerPosition, &bCanSee, &m_fMove_Angle);

	m_eAI_Status == AI_STATUS::AI_IDLE;

	if (bCanSee)
	{
		if (m_eNPC_Type == NPC_TYPE::NPC_SHOP)
			__super::Change_Script(TEXT("앗! 폭탄 신발보다 싸다!"));
		else if ((m_eNPC_Type == NPC_TYPE::NPC_INTERN) && (m_iScriptNum == 0))
			__super::Change_Script(TEXT("저기요! 잠깐만요!"));
		else if ((m_eNPC_Type == NPC_TYPE::NPC_LAST) && (m_iScriptNum == 4))
			__super::Change_Script(TEXT("잠깐 내 마지막 말을 들어줘요..."));
	}

}


HRESULT CAI_NPC::Ready_Components()
{

	return S_OK;
}

HRESULT CAI_NPC::Ready_PartObjects()
{

	return S_OK;
}

CAI_NPC* CAI_NPC::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAI_NPC* pInstance = new CAI_NPC(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAI_NPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAI_NPC::Clone(void* pArg)
{
	CAI_NPC* pInstance = new CAI_NPC(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAI_NPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_NPC::Free()
{
	__super::Free();
}
