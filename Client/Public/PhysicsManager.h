#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

// [PhysicsManager�� ����] 
// => �� ��ü�� ���� ��ġ ã�� 
// => �� ��ü ���ο� ���� ���� �ڵ� ���� �ּ�ȭ 
// => ������ ���� ���� ������ ���� (Ư�� �߷�) 
// 
// �� ����� ��ü�� ���� ���� �������� �ʱ� (��ü����)
// 
// 1. Priority_Update
//  1) �߷� ���� 
//  2) PushedPower ���� 
//  3) ���� �浹 ó�� 
//  4) BeforePosition�� ���� ��ġ�� ���� 
// 2. Update
//  1) ���� ���� (����, AI �� ��Ÿ ����)
//  2) ���� �浹 ó��
//  3) BeforePosition�� ���� ��ġ�� ����
// 3. ��ü �浹 Ȯ�� 
//  1) �浹 �����Լ� ��� ���� 
// 4. Late_Update
//  1) ������� ����� ���� ���� Ŭ������ ���� 
//  2) BeforePosition�� ���� ��ġ�� ����
// 
// Update���� ���� 
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

		_float4x4 fWorld; // <- ���� ��� �� ���� ��Ʈ������ 
		_float3 fBeforePosition;
		_float3 fPushed_Power_Dir; // <- ���� ��� �� Ǫ�� �Ŀ� 
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
	// �߷� ���� 
	// Ǫ�� �Ŀ� ���� + �Ŀ� ���� 
	// 
	void Set_Gravity_Direction(_float3 fDirection) { memcpy(&m_fGravity_Direction, &fDirection, sizeof(_float3)); }
	void Set_Gravity_Power(_float fPower) { m_fGravity_Power = fPower; }
	_float3 Get_Gravity_Direction() { return m_fGravity_Direction; }
	_float Get_Gravity_Power() { return m_fGravity_Power; }

	

	P_RESULT Total_Physics(CTransform& Transform, CCollider& Collder, _bool IsGravity, _bool IsTerrainCollision, _bool IsSlideControl, _float fTimeDelta); // ��� ���� ����� ����(IsGravity - �߷� ����, IsTerrainCollision - ���� �浹 ����, IsSlideControl - ���� �浹 �� �ε巯�� ó��)
	
	P_RESULT Make_P_Result(CTransform& Transform, CCollider& Collder); // ��� ��� ��Ͽ� ����ü ���� 
	void Update_By_P_Result(CTransform* Transform, CCollider* Collder, P_RESULT tResult);

	void Gravity(P_RESULT* tResult, _bool IsTerrainCollision, _float fTimeDelta);
	void PushedPower(P_RESULT* tResult, _float fTimeDelta); // Ǫ�� �Ŀ� ���, ���� ȸ�� ���� ��� �ֱ� 
	void CheckTerrainCollision(P_RESULT* tResult, _bool IsSlideControl); // ���� �浹�� ��� 




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