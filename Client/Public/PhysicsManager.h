#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

// [PhysicsManager의 역할] 
// => 각 객체의 현재 위치 찾기 
// => 각 객체 내부에 물리 관련 코드 내용 최소화 
// => 게임의 각종 물리 변수를 관리 (특히 중력) 
// 
// ★ 절대로 객체의 값을 직접 변경하지 않기 (객체지향)
// 
// 1. Priority_Update
//  1) 중력 적용 
//  2) PushedPower 적용 
//  3) 지형 충돌 처리 
//  4) BeforePosition을 현재 위치로 갱신 
// 2. Update
//  1) 조작 적용 (유저, AI 등 기타 조작)
//  2) 지형 충돌 처리
//  3) BeforePosition을 현재 위치로 갱신
// 3. 객체 충돌 확인 
//  1) 충돌 반응함수 결과 적용 
// 4. Late_Update
//  1) 현재까지 저장된 값을 실제 클래스에 적용 
//  2) BeforePosition을 현재 위치로 갱신
// 
// Update에서 조작 
// 




BEGIN(Engine)
class CTransform;
class CCollider;

END

BEGIN(Client)

class CPhysicsManager : public CGameObject
{
public:
	typedef struct PHYSICS_CONTROL_RESULT
	{
		_float3 Get_AdjustVector()
		{
			_float3 fAdjust = fBeforePosition;
			fAdjust.x = (fWorld._41 - fAdjust.x) * -1;
			fAdjust.y = (fWorld._42 - fAdjust.y) * -1;
			fAdjust.z = (fWorld._43 - fAdjust.z) * -1;
			return fAdjust;
		}

		_float4x4 fWorld; // <- 물리 계산 후 월드 매트릭스값 
		_float3 fBeforePosition;
		_float3 fPushed_Power_Dir; // <- 물리 계산 후 푸시 파워 
		_float fPushed_Power;
		_float fPushed_Power_Decrease;
		_float3 fCollider_Center;
		_float3 fCollider_Extents;
		_float fCollider_Radius;
	}P_RESULT;


private:
	CPhysicsManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhysicsManager(const CPhysicsManager& Prototype);
	virtual ~CPhysicsManager() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	// 중력 적용 
	// 푸시 파워 적용 + 파워 조정 
	// 
	void Set_Gravity_Direction(_float3 fDirection) { memcpy(&m_fGravity_Direction, &fDirection, sizeof(_float3)); }
	void Set_Gravity_Power(_float fPower) { m_fGravity_Power = fPower; }
	_float3 Get_Gravity_Direction() { return m_fGravity_Direction; }
	_float Get_Gravity_Power() { return m_fGravity_Power; }

	

	P_RESULT Total_Physics(CTransform& Transform, CCollider& Collder, _bool IsGravity, _bool IsTerrainCollision, _bool IsSlideControl, _float fTimeDelta); // 모든 물리 계산을 진행(IsGravity - 중력 적용, IsTerrainCollision - 지형 충돌 적용, IsSlideControl - 지형 충돌 시 부드러운 처리)
	
	P_RESULT Make_P_Result(CTransform& Transform, CCollider& Collder); // 계산 결과 기록용 구조체 제작 
	void Update_By_P_Result(CTransform* Transform, CCollider* Collder, P_RESULT tResult);

	void Gravity(P_RESULT* tResult, _bool IsTerrainCollision, _float fTimeDelta);
	void PushedPower(P_RESULT* tResult, _float fTimeDelta); // 푸시 파워 계산, 추후 회전 관련 기능 넣기 
	void CheckTerrainCollision(P_RESULT* tResult, _bool IsSlideControl); // 지형 충돌만 계산 




private:
	HRESULT Ready_Components();
	_float3 m_fGravity_Direction = { 0.f,-1.f,0.f };
	_float m_fGravity_Power = GRAVITY_ACCELE;
	_float m_fPushedPower_Decrease = 1.f;


public:
	static CPhysicsManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END