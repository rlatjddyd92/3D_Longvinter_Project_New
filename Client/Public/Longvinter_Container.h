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

	CONTAINER GetContainerType() { return eContainerType; }

protected:
	CONTAINER eContainerType = CONTAINER::CONTAINER_END;

	virtual void Moving_Control(_float fTimeDelta);
	virtual void Weapon_Control(_float fTimeDelta);
	virtual void Camera_Control(_float fTimeDelta);
	virtual void Test_Control(_float fTimeDelta);

	void Update_Distance_From_Player();
	

	void Look_Player(_float3* fPlayerPosition, _bool* bCanSee, _float* fTurnAngle);

	void Get_Sound(_float3* fSoundPosition, _float* fVolume, _float* fTurnAngle);

	void UsingWeapon(ITEMINDEX eWeapon, _float3 fPosition, _float3 fDirec);

protected: // AI 관련 함수 
	virtual void Set_AI_Status(_float fTimeDelta);



protected: // <- 상태 제어용
	_float m_fActionTimer = 0.f;
	_float m_fAttackDelay = 0.f;
	_float m_fDeamegeDelay = 0.f;

protected: // <- 스탯
	_float m_fHp = 1000.f;


protected: // <- 상태
	vector<_bool> m_vecCrowdControl;
	vector<_float> m_vecCrowdControl_Time;

protected:
	_float m_fDetective_Length = 10.f;
	_float m_fAttack_Length = 10.f;
	_float m_fClosuerLimit_Length = 10.f;

	_float m_fLook_Angle = 0.5f;

	_float m_fDistance_from_Player = 0.f;

	_float3 m_fDestination = { 0.f,0.f,0.f };

	AI_STATUS m_eAI_Status = AI_STATUS::AI_IDLE;

	_float3 m_fSoundPosition = { 0.f,0.f,0.f };
	_float m_fVolume = 0.f;

	_float m_fMove_Angle = 0.f;


	// serach
	_float m_fSearch_Time = 5.f;
	_float m_fSearch_Interval = 5.f;
	_int m_iSearch_Count = 0;

public:
	static CLongvinter_Container* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END