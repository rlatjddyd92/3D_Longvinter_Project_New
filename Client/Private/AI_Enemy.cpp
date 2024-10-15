#include "stdafx.h"
#include "AI_Enemy.h"
#include "ClientInstance.h"

CAI_Enemy::CAI_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainer_Enemy{ pDevice, pContext }
{
}

CAI_Enemy::CAI_Enemy(const CAI_Enemy& Prototype)
	: CContainer_Enemy{ Prototype }
{
}

HRESULT CAI_Enemy::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CAI_Enemy::Initialize(void* pArg)
{
	AI_Enemy_Desc* pTemp = static_cast<AI_Enemy_Desc*>(pArg);

	eContainerType = CONTAINER::CONTAINER_ENEMY;



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	
	
	m_iBody = _int(HUMAN_BODY::BODY_YELLOW);

	GET_INSTANCE->MakeEnemyHpBar(this);
	GET_INSTANCE->MakeSymbol(this);

	_int iWeapon = _int(m_pGameInstance->Get_Random_Normal() * 1000) % 3;

	
	m_fDetective_Length = 20.f;

	if (iWeapon == 0)
	{
		m_fClosuerLimit_Length = 5.f;
		m_fAttack_Length = 15.f;
		m_eWeapon = ITEMINDEX::ITEM_MACHINEGUN;
	}
		
	else if (iWeapon == 1)
	{
		m_fClosuerLimit_Length = 1.f;
		m_fAttack_Length = 3.f;
		m_eWeapon = ITEMINDEX::ITEM_SHOTGUN;
	}
		
	else if (iWeapon == 2)
	{
		m_fClosuerLimit_Length = 0.f;
		m_fAttack_Length = 1.f;
		m_eWeapon = ITEMINDEX::ITEM_MACHETE;
	}
		


	


	return S_OK;
}

void CAI_Enemy::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
	{
		__super::Add_Hp(-10.f);
	}



	if (m_fActionTimer == 0.f)
		{
			m_iFace = _int(HUMAN_FACE::FACE_NORMAL);
			static_cast<CBody_Human*>(m_Parts[PART_BODY])->Set_Human_Face(HUMAN_FACE(m_iFace));
		}
			
	Set_AI_Status(fTimeDelta);
}

void CAI_Enemy::Update(_float fTimeDelta)
{
	
	Moving_Control(fTimeDelta);
	

	__super::Update(fTimeDelta);

}

void CAI_Enemy::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAI_Enemy::Render()
{
	__super::Render();


	return S_OK;
}

void CAI_Enemy::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, tOpponent);

	CONTAINER eType = CONTAINER::CONTAINER_END;

	if (pPoint)
	{
		CLongvinter_Container* pOpponent = static_cast<CLongvinter_Container*>(pPoint);
		eType = pOpponent->GetContainerType();
	}

	if (eIndex == INTERACTION::INTER_FIRE)
		if (!m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
			__super::Burning();

	if (eType == CONTAINER::CONTAINER_PLAYER)
	{
		m_fActionTimer = 0.5f;

		m_iFace = _int(HUMAN_FACE::FACE_SAD);
		static_cast<CBody_Human*>(m_Parts[PART_BODY])->Set_Human_Face(HUMAN_FACE(m_iFace));


		if (eIndex == INTERACTION::INTER_BULLET_MACHINEGUN)
		{
			__super::Add_Hp(-100.f);
		}
		else if (eIndex == INTERACTION::INTER_EXPLOSION_NORMAL)
		{
			__super::Add_Hp(-100.f);
			_vector vDirec = XMLoadFloat3(&m_pColliderCom->GetBoundingCenter()) - XMLoadFloat3(&tOpponent.pCollider->GetBoundingCenter()) + _vector{0.f, 0.2f, 0.f, 0.f};
			_float3 fDirec{};
			XMStoreFloat3(&fDirec, vDirec);
			m_pTransformCom->Set_Pushed_Power(fDirec, GRAVITY_ACCELE * 4.f);

			if (!m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
				__super::Burning();
		}
		else if (eIndex == INTERACTION::INTER_MELEE_SHOTGUN)
		{
			if (pPoint == nullptr)
				return;



			__super::Add_Hp(-100.f);

			_float3 fDirec = tOpponent.pTransform->Get_Pushed_Dir();
			fDirec.y = 0.1f;
			
			m_pTransformCom->Set_Pushed_Power(fDirec, GRAVITY_ACCELE * 3.f);
		}
			
	}











}

void CAI_Enemy::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	__super::Collision_Reaction_MadeInterAction(pPoint, eIndex);
}

void CAI_Enemy::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	__super::Collision_Reaction_Container(pPoint, eIndex);
}



void CAI_Enemy::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);

	m_iState = STATE_IDEL;

	if (m_eAI_Status == AI_STATUS::AI_SERACH)
	{
		if (m_fMove_Angle == 0.f)
			if (_int(m_fSearch_Time_Now / m_fSearch_Interval) > m_iSearch_Count)
			{

				m_fMove_Angle = m_pGameInstance->Get_Random(2.f, 4.f);

				if (m_pGameInstance->Get_Random(0.f, 2.f) > 1.f)
					m_fMove_Angle *= -1;


				++m_iSearch_Count;
			}

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fMove_Angle);

		if (m_fMove_Angle < 0.f)
		{
			m_fMove_Angle += fTimeDelta;
			if (m_fMove_Angle > 0.f)
				m_fMove_Angle = 0.f;
		}
		else if (m_fMove_Angle > 0.f)
		{
			m_fMove_Angle -= fTimeDelta;
			if (m_fMove_Angle < 0.f)
				m_fMove_Angle = 0.f;
		}

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
		}
			

		bMove = true;
		m_pTransformCom->Go_Straight(fTimeDelta * 0.5f, true);
		m_iState = STATE_WALK;

	}
	else if (m_eAI_Status == AI_STATUS::AI_ATTACK)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fMove_Angle);

		if (m_fDistance_from_Player < m_fClosuerLimit_Length) 
		{
			m_pTransformCom->Go_Backward(fTimeDelta, true);
			m_iState = STATE_WALK;
		}
		else if (m_fDistance_from_Player < m_fAttack_Length)
		{
			Weapon_Control(fTimeDelta);
		}
		else if (m_fDistance_from_Player < m_fDetective_Length)
		{
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
			}
			bMove = true;
			m_pTransformCom->Go_Straight(fTimeDelta, true);
			m_iState = STATE_WALK;
		}
	}
	else if (m_eAI_Status == AI_STATUS::AI_PANIC)
	{
		if (m_fMove_Angle == 0.f)
			if (_int(m_fSearch_Time_Now / m_fSearch_Interval) > m_iSearch_Count)
			{

				m_fMove_Angle = m_pGameInstance->Get_Random(2.f,4.f);

				if (m_pGameInstance->Get_Random(0.f, 2.f) > 1.f)
					m_fMove_Angle *= -1;

				
				++m_iSearch_Count;
			}

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f),m_fMove_Angle);

	
			

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
}

void CAI_Enemy::Weapon_Control(_float fTimeDelta)
{
	__super::Weapon_Control(fTimeDelta);
	

}

void CAI_Enemy::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);
}

void CAI_Enemy::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);
}

void CAI_Enemy::Set_AI_Status(_float fTimeDelta)
{
	__super::Set_AI_Status(fTimeDelta);

	m_fMove_Angle = 0.f;

	if (m_vecCrowdControl[_int(CROWDCONTROL::CC_STRN)] == true)
	{
		m_eAI_Status = AI_STATUS::AI_OFF;
		return;
	}
	else if (m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)] == true)
	{
		m_eAI_Status = AI_STATUS::AI_PANIC;
		return;
	}

	__super::Get_Sound(&m_fSoundPosition, &m_fVolume, &m_fMove_Angle);

	_float3 fPlayerPosition{};
	_bool bCanSee = false;

	__super::Look_Player(&fPlayerPosition, &bCanSee, &m_fMove_Angle);

	if (bCanSee)
	{
		m_fDestination = fPlayerPosition;
		m_eAI_Status = AI_STATUS::AI_ATTACK;
	}
	else
	{
		if (m_eAI_Status == AI_STATUS::AI_ATTACK)
		{
			__super::Start_Serach();
		}
		else if (m_eAI_Status == AI_STATUS::AI_SERACH)
		{

		}
	}
}


HRESULT CAI_Enemy::Ready_Components()
{
	
	return S_OK;
}

HRESULT CAI_Enemy::Ready_PartObjects()
{
	
	return S_OK;
}

CAI_Enemy* CAI_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAI_Enemy* pInstance = new CAI_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAI_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAI_Enemy::Clone(void* pArg)
{
	CAI_Enemy* pInstance = new CAI_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAI_Enemy"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Enemy::Free()
{
	__super::Free();
}
