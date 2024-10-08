#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(const _float4x4* pWorldMatrix);
	virtual HRESULT Render();

public:
	_float3 GetBoundingCenter(); //{ return m_pBounding->GetBoundingCenter(); }
	_float3 GetBoundingExtents(); //{ return m_pBounding->GetBoundingExtents(); }
	_float GetBoundingRadius_Sphere(); //{ return m_pBounding->GetBoundingRadius_Sphere(); }
	void SetBoundingExtents(_float3 fExtent);
	void SetBoundingRadius_Sphere(_float fRadius);

	_bool GetCollision(CCollider::TYPE eColliderType, CCollider* pOpponent); // { return m_pBounding->GetCollision(eColliderType, pOpponent); }
	class CBounding* GetBounding() { return m_pBounding; }

private:
	class CBounding*				m_pBounding = { nullptr };
	TYPE							m_eColliderType = {};

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = { nullptr };
	BasicEffect*									m_pEffect = { nullptr };
	ID3D11InputLayout*								m_pInputLayout = { nullptr };


#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END