#pragma once

#include "Component.h"

// [CPysics] 
// 1. 역할 
//  (1) 충돌체의 역할 
//  (2) 객체가 받은 힘의 방향과 세기를 저장 
//  (3) 게임에서 적용되는 중력과 각종 가속도/감속도의 설정 내용을 저장 
//  (4) (2)와 (3)의 내용을 토대로 객체가 이번 프레임에 이동할 위치를 계산 
//  
// 2. 사용방식 
//  (1) transform이 포인터를 소유하는 형태 
//  (2) 하나의 transform은 여러 개의 Physics 소유 가능 
//  (3) 
// 

BEGIN(Engine)

class ENGINE_DLL CPhysics final : public CComponent
{
private:
	CPhysics(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhysics(const CPhysics& Prototype);
	virtual ~CPhysics() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);






private:


public:
	static CPhysics* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END