#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float		fSpeedPerSec{30.f}, fRotationPerSec{30.f};
		_float3		fRight{ -1,-1,-1 };
		_float3		fUp{ -1,-1,-1 };
		_float3		fLook{ -1,-1,-1 };
		_float3		fPosition{ -1,-1,-1 };
	}TRANSFORM_DESC;


	typedef struct
	{
		_float3		fBeforePosition = { -1.f,-1.f,-1.f};
		_float3		fPushedDirection = { 0.f,0.f,0.f };
		_float		fPushedPower = 0.f;
		_float		fPushedPowerDecrease = 0.01f;
	}TRANSFORM_PHYSICS;

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransform() = default;

public:
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat3((_float3*)&m_WorldMatrix.m[eState][0], vState);
	}

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float4x4	Get_WorldMatrix() const {
		return m_WorldMatrix;
	}

	_float3 Get_Scaled() const;

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	const _float4x4* Get_WorldMatrix_Ptr() const {
		return &m_WorldMatrix;
	}

	

public:	
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	
public:
	void Set_Scaled(_float fX, _float fY, _float fZ);
	void LookAt(_fvector vAt);
	void Go_Straight(_float fTimeDelta, _bool bHorizonal = false);
	void Go_Backward(_float fTimeDelta, _bool bHorizonal = false);
	void Go_Left(_float fTimeDelta, _bool bHorizonal = false);
	void Go_Right(_float fTimeDelta, _bool bHorizonal = false);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);	
	void Turn(_bool isRotationX, _bool isRotationY, _bool isRotationZ, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation(_float fX, _float fY, _float fZ);

public: // Physics
	void Save_BeforePosition() 
	{
		_vector tTemp = Get_State(STATE::STATE_POSITION);
		m_tPhysics.fBeforePosition = { tTemp.m128_f32[0], tTemp.m128_f32[1], tTemp.m128_f32[2] };
	}

	_float3 Get_BeforePosition() { return m_tPhysics.fBeforePosition; }
	_float3 Get_AdjustVector() 
	{
		_float3 fAdjust = m_tPhysics.fBeforePosition;
		fAdjust.x = (m_WorldMatrix._41 - fAdjust.x) * -1;
		fAdjust.y = (m_WorldMatrix._42 - fAdjust.y) * -1;
		fAdjust.z = (m_WorldMatrix._43 - fAdjust.z) * -1;
		return fAdjust;
	}

	_float3 Get_Pushed_Dir() { return m_tPhysics.fPushedDirection; }
	_float Get_Pushed_Power() { return m_tPhysics.fPushedPower; }
	_float Get_Pushed_PowerDecrease() { return m_tPhysics.fPushedPowerDecrease; }

	void Add_Pushed_Power(_float3 Dir, _float Power);
	void Set_Pushed_Power(_float3 Dir, _float Power) { m_tPhysics.fPushedDirection = Dir; m_tPhysics.fPushedPower = Power; }
	void Set_Pushed_PowerDecrease(_float Power) { m_tPhysics.fPushedPowerDecrease = Power; }

	void Set_RotationPerSec(_float fSpeed) { m_fRotationPerSec = fSpeed; }
	void Set_SpeedPerSec(_float fSpeed) { m_fSpeedPerSec = fSpeed; }
	
public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);


private:
	_float			m_fSpeedPerSec{};
	_float			m_fRotationPerSec{};

	_float4x4		m_WorldMatrix = {};

	TRANSFORM_PHYSICS	m_tPhysics = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CComponent* Clone(void* pArg) { return nullptr; }
	virtual void Free() override;
};

END