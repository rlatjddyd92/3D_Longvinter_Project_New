#include "stdafx.h"
#include "..\Public\Container_Turret.h"

#include "Body_Human.h"
#include "ClientInstance.h"
#include "Tool.h"

CContainer_Turret::CContainer_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLongvinter_Container{ pDevice, pContext }
{
}

CContainer_Turret::CContainer_Turret(const CContainer_Turret& Prototype)
	: CLongvinter_Container{ Prototype }
{
}

HRESULT CContainer_Turret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CContainer_Turret::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	eContainerType = CONTAINER::CONTAINER_TURRET;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	GET_INSTANCE->MakeEnemyHpBar(this);
	GET_INSTANCE->MakeSymbol(this);

	m_pTransformCom->Set_Pushed_PowerDecrease(1.f);
	//m_pTransformCom->Set_Scaled(1.5f, 1.5f, 1.5f);

	m_fDetective_Length = 7.f;
	
	m_fRotate = 0.4f;
	
	return S_OK;
}

void CContainer_Turret::Priority_Update(_float fTimeDelta)
{

	/*if (m_bRotate)
	{
		m_pTransformCom->Rotation({ 0.f,1.f,0.f }, -m_fRotate);
		m_bRotate = false;
	}
		*/

	__super::Priority_Update(fTimeDelta);

	if (m_bHack[0])
	{
		m_vecCrowdControl[_int(CROWDCONTROL::CC_STRN)] = true;
		m_vecCrowdControl_Time[_int(CROWDCONTROL::CC_STRN)] = 1000.f;
		m_eAI_Status = AI_STATUS::AI_OFF;
	}
	if (m_bHack[1])
	{
		m_vecCrowdControl[_int(CROWDCONTROL::CC_STRN)] = false;
		m_vecCrowdControl_Time[_int(CROWDCONTROL::CC_STRN)] = 0.f;
		m_eAI_Status = AI_STATUS::AI_ATTACK;
	}

	if (m_bHack[2])
	{
		

		m_bExplosionActive = true;
	}



	for (auto& pPartObject : m_Parts)
		pPartObject->Priority_Update(fTimeDelta);

	Set_AI_Status(fTimeDelta);
}

void CContainer_Turret::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_bHack[2])
	{
		m_fExplosionWaitTime -= fTimeDelta;
		if (m_fExplosionWaitTime < 0.f)
		{
			GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_EXPLOSION_NORMAL, nullptr, m_pColliderCom->GetBoundingCenter(), { 0.f,0.f,0.f });
			__super::SetDead();
		}
		else if (m_fMakeEffect >= 0.05f)
		{
			for (_int i = 0; i < 5; ++i)
				GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FIRE, m_pColliderCom->GetBoundingCenter());
		}
	}

	if (m_bExplosionActive)
		GET_INSTANCE->SetChannelVolume(SOUND_NAME::SOUND_FIRE, SOUND_CHANNEL::CH_INGAME_FIRE, 10.f, m_pColliderCom->GetBoundingCenter());


	Moving_Control(fTimeDelta);



	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());

	CPhysicsManager::P_RESULT tResult = {};

	tResult = GET_INSTANCE->Total_Physics(*m_pTransformCom, *m_pColliderCom, true, true, true, fTimeDelta);
	GET_INSTANCE->Update_By_P_Result(m_pTransformCom, m_pColliderCom, tResult);


	if (m_vecCrowdControl[_int(CROWDCONTROL::CC_BURN)])
		if (m_fMakeEffect >= 0.05f)
		{
			for (_int i = 0; i < 5; ++i)
				GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FIRE, m_pColliderCom->GetBoundingCenter());
		}

	m_fMakeEffect -= fTimeDelta;
	if (m_fMakeEffect < 0.f)
		m_fMakeEffect = 0.05f;

	for (auto& pPartObject : m_Parts)
		pPartObject->Update(fTimeDelta);
}

void CContainer_Turret::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& pPartObject : m_Parts)
		pPartObject->Late_Update(fTimeDelta);



	m_pTransformCom->Save_BeforePosition();
}

HRESULT CContainer_Turret::Render()
{
	m_pColliderCom->Render();

	return S_OK;
}

void CContainer_Turret::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, tOpponent);

	

	CONTAINER eType = CONTAINER::CONTAINER_END;

	if (pPoint)
	{
		CLongvinter_Container* pOpponent = static_cast<CLongvinter_Container*>(pPoint);
		eType = pOpponent->GetContainerType();
	}

	else if (eIndex == INTERACTION::INTER_EXPLOSION_NORMAL)
	{
		__super::SetDead();
	}

	if ((eType == CONTAINER::CONTAINER_PLAYER) || (eType == CONTAINER::CONTAINER_ENEMY))
	{
		for (_int i = 0; i < 10; ++i)
			GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_DEBRIS, m_pColliderCom->GetBoundingCenter());

		if (eIndex == INTERACTION::INTER_BULLET_MACHINEGUN)
		{
			__super::Add_Hp(-10.f);
		}
		else if (eIndex == INTERACTION::INTER_MELEE_SHOTGUN)
		{
			if (pPoint == nullptr)
				return;

			__super::Add_Hp(-30.f);

		}
	}

	
}

void CContainer_Turret::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	__super::Collision_Reaction_MadeInterAction(pPoint, eIndex);
}

void CContainer_Turret::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	__super::Collision_Reaction_Container(pPoint, eIndex);
}

void CContainer_Turret::SetHack(_int iFunc)
{
	if ((iFunc == 2) && (m_bHack[iFunc] == false))
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_FIRE, SOUND_CHANNEL::CH_INGAME_FIRE, 10.f, m_pColliderCom->GetBoundingCenter());

	m_bHack[iFunc] = true;
}

void CContainer_Turret::DeadAction()
{
	__super::DeadAction();
}


void CContainer_Turret::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);

	if (m_eAI_Status == AI_STATUS::AI_IDLE)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 1.f);


	}
	else if (m_eAI_Status == AI_STATUS::AI_ATTACK)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.5f * m_fMove_Angle);

		Weapon_Control(fTimeDelta);
	}
}

void CContainer_Turret::Weapon_Control(_float fTimeDelta)
{
	__super::Weapon_Control(fTimeDelta);

	if (m_fAttackDelay == 0.f)
	if (abs(m_fMove_Angle) < 0.5f)
	{
		_float3 fStartPostion{};
		_float3 fPushedDirec{};

		_vector vStartPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, 0.f, 0.f };
		XMStoreFloat3(&fStartPostion, vStartPosition);

		_vector vPushedDirec = (GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION) + _vector{ 0.f, 1.f, 0.f, 0.f }) - vStartPosition;
		XMStoreFloat3(&fPushedDirec, vPushedDirec);

		XMStoreFloat3(&fPushedDirec, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		__super::UsingWeapon(ITEMINDEX::ITEM_MACHINEGUN, fStartPostion, fPushedDirec);
	}
}

void CContainer_Turret::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);
}

void CContainer_Turret::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);
}

void CContainer_Turret::Set_AI_Status(_float fTimeDelta)
{
	__super::Set_AI_Status(fTimeDelta);

	m_eAI_Status = AI_STATUS::AI_IDLE;

	if (m_eAI_Status == AI_STATUS::AI_DEAD)
		return;

	m_fMove_Angle = 0.f;

	if (m_vecCrowdControl[_int(CROWDCONTROL::CC_STRN)] == true)
	{
		m_eAI_Status = AI_STATUS::AI_OFF;
		return;
	}


	_float3 fPlayerPosition{};
	_bool bCanSee = false;

	if (!m_bHack[1])
		__super::Look_Player(&fPlayerPosition, &bCanSee, &m_fMove_Angle);
	else
	{
		_int iSize = m_pGameInstance->GetLayerSize(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"));

		for (_int i = 0; i < iSize; ++i)
		{
			CLongvinter_Container* pTarget = static_cast<CLongvinter_Container*>(m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), i));

			_vector vDistance = pTarget->GetTransform(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			_float fDistance = sqrt(pow(vDistance.m128_f32[0], 2) + pow(vDistance.m128_f32[1], 2) + pow(vDistance.m128_f32[2], 2));

			if ((fDistance < 10.f) && (_int(m_pGameInstance->Get_Random(0.f,1.f)) % 2 == 0))
			{
				_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

				bCanSee = true;

				_float fDot = XMVector3Dot({ vDistance.m128_f32[0], 0.f, vDistance.m128_f32[2], 0.f}, {vLook.m128_f32[0], 0.f, vLook.m128_f32[2], 0.f}).m128_f32[0];
				_float fCos = (sqrt(pow(vDistance.m128_f32[0], 2) + pow(vDistance.m128_f32[2], 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));
				m_fMove_Angle = acos(fDot / fCos);

				if (isnan(m_fMove_Angle))
					m_fMove_Angle = 0.f;

				_bool bResult = GET_INSTANCE->Check_CCW_XZ({ vDistance.m128_f32[0], 0.f, vDistance.m128_f32[2] }, {0.f,0.f,0.f}, {vLook.m128_f32[0], 0.f, vLook.m128_f32[2]});

				if (bResult)
					m_fMove_Angle *= -1;
			}
		}
		

	}

	if (bCanSee)
	{
		
		m_fDestination = fPlayerPosition;
		m_eAI_Status = AI_STATUS::AI_ATTACK;
		GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_BEEP, SOUND_CHANNEL::CH_SYSTEM_UI, 10.f);
	
		
	}
	
}

void CContainer_Turret::Burning()
{
	
}



HRESULT CContainer_Turret::Ready_Components()
{


	return S_OK;
}

HRESULT CContainer_Turret::Ready_PartObjects()
{
	/* 실제 추가하고 싶은 파트오브젝트의 갯수만큼 밸류를 셋팅해놓자. */
	m_Parts.resize(1);

	CBody::BODY_DESC		BodyDesc{};
	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	if (FAILED(__super::Add_PartObject(0, TEXT("Prototype_GameObject_Body_Turret"), &BodyDesc)))
		return E_FAIL;

	


	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(0.5f, 1.0f, 0.5f);
	ColliderDesc.vCenter = _float3(0.0f, ColliderDesc.vExtents.y, 0.0f);

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;


	GET_INSTANCE->Input_ContainerColliderPointer(eContainerType, static_cast<CLongvinter_Container*>(this), m_pColliderCom);

	return S_OK;
}

CContainer_Turret* CContainer_Turret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CContainer_Turret* pInstance = new CContainer_Turret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCContainer_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CContainer_Turret::Clone(void* pArg)
{
	CContainer_Turret* pInstance = new CContainer_Turret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCContainer_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CContainer_Turret::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
