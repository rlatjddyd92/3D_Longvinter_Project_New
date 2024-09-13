#pragma once

#include "Component.h"

// [CPysics] 
// 1. ���� 
//  (1) �浹ü�� ���� 
//  (2) ��ü�� ���� ���� ����� ���⸦ ���� 
//  (3) ���ӿ��� ����Ǵ� �߷°� ���� ���ӵ�/���ӵ��� ���� ������ ���� 
//  (4) (2)�� (3)�� ������ ���� ��ü�� �̹� �����ӿ� �̵��� ��ġ�� ��� 
//  
// 2. ����� 
//  (1) transform�� �����͸� �����ϴ� ���� 
//  (2) �ϳ��� transform�� ���� ���� Physics ���� ���� 
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