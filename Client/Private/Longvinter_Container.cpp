#include "stdafx.h"
#include "..\Public\Longvinter_Container.h"

#include "Body_Human.h"
#include "Tool.h"

#include "ClientInstance.h"

CLongvinter_Container::CLongvinter_Container(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CLongvinter_Container::CLongvinter_Container(const CLongvinter_Container& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CLongvinter_Container::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLongvinter_Container::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_vecCrowdControl.resize(_int(CROWDCONTROL::CC_END));
	m_vecCrowdControl_Time.resize(_int(CROWDCONTROL::CC_END));

	GET_INSTANCE->MakeScript(this);

	return S_OK;
}

void CLongvinter_Container::Priority_Update(_float fTimeDelta)
{
	m_bNonLoopAnimReset = false;
	bMove = false;

	if (m_fActionTimer > 0.f)
	{
		m_fActionTimer -= fTimeDelta;
		if (m_fActionTimer < 0.f)
			m_fActionTimer = 0.f;
	}

	if (m_fAttackDelay > 0.f)
	{
		m_fAttackDelay -= fTimeDelta;
		if (m_fAttackDelay < 0.f)
			m_fAttackDelay = 0.f;
	}

	if (m_fDeamegeDelay > 0.f)
	{
		m_fDeamegeDelay -= fTimeDelta;
		if (m_fDeamegeDelay < 0.f)
			m_fDeamegeDelay = 0.f;
	}

	if (m_fChangeScript_Now < m_fChangeScript_Interval)
	{
		m_fChangeScript_Now += fTimeDelta;
		if (m_fChangeScript_Now > m_fChangeScript_Interval)
			m_fChangeScript_Now = m_fChangeScript_Interval;
	}




	if ((m_eAI_Status == AI_STATUS::AI_SERACH) || (m_eAI_Status == AI_STATUS::AI_PANIC))
	if (m_fSearch_Time_Now < m_fSearch_Time)
	{
		m_fSearch_Time_Now += fTimeDelta;
		if (m_fSearch_Time_Now > m_fSearch_Time)
		{
			if (m_eAI_Status == AI_STATUS::AI_SERACH)
				End_Search();
		}	
	}

	
	
	for (_int i = 0; i < _int(CROWDCONTROL::CC_END); ++ i)
	{
		m_vecCrowdControl_Time[i] -= fTimeDelta;
		if (m_vecCrowdControl_Time[i] < 0.f)
		{
			if ((i == _int(CROWDCONTROL::CC_BURN)) && (m_vecCrowdControl[i]))
				End_Panic();

			m_vecCrowdControl_Time[i] = 0.f;
			m_vecCrowdControl[i] = false;
		}
	}

	
		

}

void CLongvinter_Container::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CLongvinter_Container::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CLongvinter_Container::Render()
{

	return S_OK;
}

void CLongvinter_Container::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	

}

void CLongvinter_Container::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	
}

void CLongvinter_Container::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	
}

void CLongvinter_Container::DeadAction()
{
	

}



void CLongvinter_Container::Moving_Control(_float fTimeDelta)
{

}

void CLongvinter_Container::Weapon_Control(_float fTimeDelta)
{
	
}

void CLongvinter_Container::Camera_Control(_float fTimeDelta)
{
}

void CLongvinter_Container::Test_Control(_float fTimeDelta)
{
}

void CLongvinter_Container::Update_Distance_From_Player()
{
	_vector vPlayer = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
	_vector vThis = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_fDistance_from_Player = sqrt(pow(vPlayer.m128_f32[0] - vThis.m128_f32[0], 2) + pow(vPlayer.m128_f32[1] - vThis.m128_f32[1], 2) + pow(vPlayer.m128_f32[2] - vThis.m128_f32[2], 2));
}

void CLongvinter_Container::Look_Player(_float3* fPlayerPosition, _bool* bCanSee, _float* fTurnAngle)
{
	Update_Distance_From_Player();
	XMStoreFloat3(fPlayerPosition, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION));

	if (!m_bMonsterMake)
	{
		if (m_fDistance_from_Player > m_fDetective_Length)
		{
			*bCanSee = false;
			//*fTurnAngle = 0.f;
			return;
		}

		_float3 fPoint = {};
		XMStoreFloat3(&fPoint, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION));
		fPoint.x -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
		fPoint.y -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1];
		fPoint.z -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2];

		


		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		_float fDot = XMVector3Dot({ fPoint.x,fPoint.y, fPoint.z, 0.f }, { vLook.m128_f32[0], vLook.m128_f32[1], vLook.m128_f32[2], 0.f }).m128_f32[0];
		_float fCos = (sqrt(pow(fPoint.x, 2) + pow(fPoint.y, 2) + pow(fPoint.z, 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[1], 2) + pow(vLook.m128_f32[2], 2)));
		_float fAngle = acos(fDot / fCos);

		if ((isnan(fAngle)) || (abs(fAngle) > m_fLook_Angle))
		{
			*bCanSee = false;
			//*fTurnAngle = 0.f;
			return;
		}

		*bCanSee = true;

		fDot = XMVector3Dot({ fPoint.x, 0.f, fPoint.z, 0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2], 0.f }).m128_f32[0];
		fCos = (sqrt(pow(fPoint.x, 2) + pow(fPoint.z, 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));
		*fTurnAngle = acos(fDot / fCos);

		if (isnan(*fTurnAngle))
			*fTurnAngle = 0.f;

		_bool bResult = GET_INSTANCE->Check_CCW_XZ(fPoint, { 0.f,0.f,0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2] });

		if (bResult)
			*fTurnAngle *= -1;
	}
	else
	{
		_float3 fPoint = {};
		XMStoreFloat3(&fPoint, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION));
		fPoint.x -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
		fPoint.y -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1];
		fPoint.z -= m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2];

		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		*bCanSee = true;

		_float fDot = XMVector3Dot({ fPoint.x, 0.f, fPoint.z, 0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2], 0.f }).m128_f32[0];
		_float fCos = (sqrt(pow(fPoint.x, 2) + pow(fPoint.z, 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));
		*fTurnAngle = acos(fDot / fCos);

		if (isnan(*fTurnAngle))
			*fTurnAngle = 0.f;

		_bool bResult = GET_INSTANCE->Check_CCW_XZ(fPoint, { 0.f,0.f,0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2] });

		if (bResult)
			*fTurnAngle *= -1;
	}

}

void CLongvinter_Container::Get_Sound(_float3* fSoundPosition, _float* fVolume, _float* fTurnAngle)
{
}

void CLongvinter_Container::UsingWeapon(ITEMINDEX eWeapon, _float3 fPosition, _float3 fDirec)
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition.m128_f32[1] += 0.5f;

	if (eWeapon == ITEMINDEX::ITEM_MACHINEGUN)
	{
		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_BULLET_MACHINEGUN, this, fPosition, fDirec);
		GET_INSTANCE->MakeGunFireLight({ vPosition.m128_f32[0], vPosition.m128_f32[1], vPosition.m128_f32[2], 1.f });

		for (_int i = 0; i < 5; ++i)
			GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FLAME, fPosition, fDirec, 0.1f);
		
		m_fAttackDelay = 0.1f;
	}
	else if (eWeapon == ITEMINDEX::ITEM_LANDMINE)
	{
		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_THORW_MINE, this, fPosition, fDirec);
		
		m_fAttackDelay = 1.f;
	}
	else if (eWeapon == ITEMINDEX::ITEM_GRANADE)
	{
		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_THORW_GRANADE, this, fPosition, fDirec);
		
		m_fAttackDelay = 1.f;
	}
	else if (eWeapon == ITEMINDEX::ITEM_MACHETE)
	{
		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_MELEE_NORMAL, this, fPosition, fDirec);
		
		m_fAttackDelay = 0.2f;
	}
	else if (eWeapon == ITEMINDEX::ITEM_SHOTGUN)
	{
		GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_MELEE_SHOTGUN, this, fPosition, fDirec);
		GET_INSTANCE->MakeGunFireLight({ vPosition.m128_f32[0], vPosition.m128_f32[1], vPosition.m128_f32[2], 1.f });

		for (_int i = 0; i < 20; ++i)
			GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FLAME, fPosition, fDirec);
		
		m_fAttackDelay = 1.f;
	}
	

}

void CLongvinter_Container::AimWeapon_Anim(ITEMINDEX eWeapon)
{
}

void CLongvinter_Container::FireWeapon_Anim(ITEMINDEX eWeapon)
{
}

void CLongvinter_Container::Set_AI_Status(_float fTimeDelta)
{




}

void CLongvinter_Container::Start_Serach()
{
	m_eAI_Status = AI_STATUS::AI_SERACH;
	m_fSearch_Time_Now = 0.f;
	m_fSearch_Time = 20.f;
	m_fSearch_Interval = 5.f;
	m_iSearch_Count = 0;
}

void CLongvinter_Container::End_Search()
{
	m_eAI_Status = AI_STATUS::AI_IDLE;
	m_fSearch_Time_Now = 0.f;
	m_fSearch_Time = 20.f;
	m_fSearch_Interval = 5.f;
	m_iSearch_Count = 0;
}

void CLongvinter_Container::Start_Panic()
{
	m_eAI_Status = AI_STATUS::AI_PANIC;
	m_fSearch_Time_Now = 0.f;
	m_fSearch_Time = 1000.f;
	m_fSearch_Interval = 0.5f;
	m_iSearch_Count = 0;
}

void CLongvinter_Container::End_Panic()
{
	m_eAI_Status = AI_STATUS::AI_IDLE;
	m_fSearch_Time_Now = 0.f;
	m_fSearch_Time = 20.f;
	m_fSearch_Interval = 5.f;
	m_iSearch_Count = 0;
}

CLongvinter_Container* CLongvinter_Container::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLongvinter_Container* pInstance = new CLongvinter_Container(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLongvinter_Container"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CLongvinter_Container::Clone(void* pArg)
{
	CLongvinter_Container* pInstance = new CLongvinter_Container(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLongvinter_Container"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLongvinter_Container::Free()
{
	__super::Free();
	m_vecCrowdControl.clear();
	m_vecCrowdControl_Time.clear();
}
