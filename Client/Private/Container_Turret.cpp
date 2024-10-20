#include "stdafx.h"
#include "..\Public\Container_Turret.h"

#include "Body_Human.h"
#include "ClientInstance.h"
#include "Tool.h"

CCContainer_Turret::CCContainer_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLongvinter_Container{ pDevice, pContext }
{
}

CCContainer_Turret::CCContainer_Turret(const CCContainer_Turret& Prototype)
	: CLongvinter_Container{ Prototype }
{
}

HRESULT CCContainer_Turret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCContainer_Turret::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	eContainerType = CONTAINER::CONTAINER_TURRET;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	
	m_pTransformCom->Set_Pushed_PowerDecrease(1.f);
	m_pTransformCom->Set_Scaled(0.95f, 0.95f, 0.95f);
	
	m_fRotate = 0.4f;

	return S_OK;
}

void CCContainer_Turret::Priority_Update(_float fTimeDelta)
{

	/*if (m_bRotate)
	{
		m_pTransformCom->Rotation({ 0.f,1.f,0.f }, -m_fRotate);
		m_bRotate = false;
	}
		*/

	__super::Priority_Update(fTimeDelta);




	for (auto& pPartObject : m_Parts)
		pPartObject->Priority_Update(fTimeDelta);

	Set_AI_Status(fTimeDelta);
}

void CCContainer_Turret::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


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

void CCContainer_Turret::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& pPartObject : m_Parts)
		pPartObject->Late_Update(fTimeDelta);



	m_pTransformCom->Save_BeforePosition();
}

HRESULT CCContainer_Turret::Render()
{
	m_pColliderCom->Render();

	return S_OK;
}

void CCContainer_Turret::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, tOpponent);

}

void CCContainer_Turret::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	__super::Collision_Reaction_MadeInterAction(pPoint, eIndex);
}

void CCContainer_Turret::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	__super::Collision_Reaction_Container(pPoint, eIndex);
}

void CCContainer_Turret::DeadAction()
{
	__super::DeadAction();
}


void CCContainer_Turret::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);

	if (m_eAI_Status == AI_STATUS::AI_IDLE)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 1.f);


	}
	else if (m_eAI_Status == AI_STATUS::AI_ATTACK)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fMove_Angle);

		Weapon_Control(fTimeDelta);
	}
}

void CCContainer_Turret::Weapon_Control(_float fTimeDelta)
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

void CCContainer_Turret::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);
}

void CCContainer_Turret::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);
}

void CCContainer_Turret::Set_AI_Status(_float fTimeDelta)
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

	
	__super::Look_Player(&fPlayerPosition, &bCanSee, &m_fMove_Angle);

	if (bCanSee)
	{
		
		m_fDestination = fPlayerPosition;
		m_eAI_Status = AI_STATUS::AI_ATTACK;

	
		
	}
	
}

void CCContainer_Turret::Burning()
{
	
}



HRESULT CCContainer_Turret::Ready_Components()
{


	return S_OK;
}

HRESULT CCContainer_Turret::Ready_PartObjects()
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

CCContainer_Turret* CCContainer_Turret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCContainer_Turret* pInstance = new CCContainer_Turret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCContainer_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CCContainer_Turret::Clone(void* pArg)
{
	CCContainer_Turret* pInstance = new CCContainer_Turret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCContainer_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCContainer_Turret::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
