#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "InterAction.h"



BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CLongvinter_Container : public CContainerObject
{

protected:
	CLongvinter_Container(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLongvinter_Container(const CLongvinter_Container& Prototype);
	virtual ~CLongvinter_Container() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent);
	virtual void Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex);
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex);

	void Rotation(_float3 fAxis, _float fAngle) { m_pTransformCom->Rotation(XMLoadFloat3(&fAxis), XMConvertToRadians(fAngle)); }

	CONTAINER GetContainerType() { return eContainerType; }

	_float Get_Hp() { return m_fHp; }
	_float Get_HpMax() { return m_fHp_Max; }
	
	_bool Get_CC(CROWDCONTROL eType) { return m_vecCrowdControl[_int(eType)]; }
	AI_STATUS Get_AI_Status() { return m_eAI_Status; }

	void SetMonsterMake(_bool bIsMonsterMaker) 
	{ 
		m_bMonsterMake = bIsMonsterMaker; 
	}


protected:
	CONTAINER eContainerType = CONTAINER::CONTAINER_END;

	virtual void Moving_Control(_float fTimeDelta);
	virtual void Weapon_Control(_float fTimeDelta);
	virtual void Camera_Control(_float fTimeDelta);
	virtual void Test_Control(_float fTimeDelta);

	virtual void DeadAction();

	void Update_Distance_From_Player();
	

	void Look_Player(_float3* fPlayerPosition, _bool* bCanSee, _float* fTurnAngle);

	void Get_Sound(_float3* fSoundPosition, _float* fVolume, _float* fTurnAngle);

	void UsingWeapon(ITEMINDEX eWeapon, _float3 fPosition, _float3 fDirec);
	void AimWeapon_Anim(ITEMINDEX eWeapon);
	void FireWeapon_Anim(ITEMINDEX eWeapon);

	void Add_Hp(_float fAdd)
	{
		m_fHp += fAdd;
		if (m_fHp > m_fHp_Max)
			m_fHp = m_fHp_Max;
		else if (m_fHp < 0.f)
			m_fHp = 0.f;
	}

	
	

protected: // AI 관련 함수 
	virtual void Set_AI_Status(_float fTimeDelta);
	void Start_Serach();
	void End_Search();

	void Start_Panic();
	void End_Panic();



protected: // <- 상태 제어용
	_float m_fActionTimer = 0.f;
	_float m_fPreAttackDelay = 0.f;
	_float m_fAttackDelay = 0.f;
	_float m_fDeamegeDelay = 0.f;

	_bool m_bMakeInterAction = false;

	_bool m_bRotate = false;
	_float m_fRotate = 0.f;

protected: // <- 스탯
	_float m_fHp = 1000.f;
	_float m_fHp_Max = 1000.f;

protected: // <- 상태
	vector<_bool> m_vecCrowdControl;
	vector<_float> m_vecCrowdControl_Time;

	_bool m_bJump = false;

protected:
	_float m_fDetective_Length = 10.f;
	_float m_fAttack_Length = 10.f;
	_float m_fClosuerLimit_Length = 10.f;

	_float m_fLook_Angle = 0.4f * PI_DEFINE;

	_float m_fDistance_from_Player = 0.f;

	_float3 m_fDestination = { 0.f,0.f,0.f };

	AI_STATUS m_eAI_Status = AI_STATUS::AI_IDLE;

	_float3 m_fSoundPosition = { 0.f,0.f,0.f };
	_float m_fVolume = 0.f;

	_float m_fMove_Angle = 0.f;

	_float m_fMakeEffect = 0.1f;
	_bool m_bNonLoopAnimReset = false;

	_bool bMove = false;

	// serach
	_float m_fSearch_Time_Now = 0.f;
	_float m_fSearch_Time = 20.f;
	_float m_fSearch_Interval = 5.f;
	_int m_iSearch_Count = 0;

	_bool m_bMonsterMake = false;

public:
	static CLongvinter_Container* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END