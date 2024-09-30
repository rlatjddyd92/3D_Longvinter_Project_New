#pragma once

#include "Base.h"
#include "DebugDraw.h"
#include "Collider.h"

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct
	{
		_float3		vCenter;
	}BOUNDING_DESC;
protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CBounding() = default;

public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pBoundingDesc);
	virtual void Update(_fmatrix WorldMatrix);
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch);

public:
	virtual _bool Intersect(CCollider::TYPE eColliderType, CBounding* pBounding) = 0;

public:
	_float3 GetBoundingCenter()
	{
		if (m_pBoundingDesc_AABB)
			return m_pBoundingDesc_AABB->Center;
		else if (m_pBoundingDesc_OBB)
			return m_pBoundingDesc_OBB->Center;
		else if (m_pBoundingDesc_Sphere)
			return m_pBoundingDesc_Sphere->Center;
	}
	_float3 GetBoundingExtents()
	{ 
		if (m_pBoundingDesc_AABB)
			return m_pBoundingDesc_AABB->Extents;
		else if (m_pBoundingDesc_OBB)
			return m_pBoundingDesc_OBB->Extents;
		else if (m_pBoundingDesc_Sphere)
		{
			XMFLOAT3 tResult{};
			_float fRadius = GetBoundingRadius_Sphere();

			tResult.x = fRadius;
			tResult.y = fRadius;
			tResult.z = fRadius;

			return tResult;
		}
	}
	_float GetBoundingRadius_Sphere() { return m_pBoundingDesc_Sphere->Radius; }

protected: // AABB
	BoundingBox* m_pOriginalBoundingDesc_AABB = { nullptr };
	BoundingBox* m_pBoundingDesc_AABB = { nullptr };

protected: // OBB
	BoundingOrientedBox* m_pOriginalBoundingDesc_OBB = { nullptr };
	BoundingOrientedBox* m_pBoundingDesc_OBB = { nullptr };

protected: // Sphere
	BoundingSphere* m_pOriginalBoundingDesc_Sphere = { nullptr };
	BoundingSphere* m_pBoundingDesc_Sphere = { nullptr };

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	
	_bool						m_isColl = { false };

public:
	virtual void Free() override;
};

END