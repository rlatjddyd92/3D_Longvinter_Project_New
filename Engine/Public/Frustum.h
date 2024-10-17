#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();

	/* ���彺���̽� ���� ������ ���� �̵���Ű�� */
	/* ����� �����Ѵ�. */
	void Update();

public:
	_bool isIn_WorldSpace(_fvector vPosition, _float fRadius);
	_bool isIn_LocalSpace(_fvector vPosition, _float fRadius);
	void Transform_ToLocalSpace(_fmatrix WorldMatrix);

private:
	class CGameInstance*	m_pGameInstance = { nullptr };
	_float3					m_vPoints[8];
	_float3					m_vWorldPoints[8];
	_float4					m_vPlanes_InWorldSpace[6];
	_float4					m_vPlanes_InLocalSpace[6];

private:
	void Make_Plane(const _float3* pPoints, _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void Free() override;
};

END