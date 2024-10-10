#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CFreeCamera final : public CCamera
{
public:
	
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor;
	}CAMERA_FREE_DESC;
private:
	CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFreeCamera(const CFreeCamera& Prototype);
	virtual ~CFreeCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetCameraMode(CAMERAMODE eInput);
	CAMERAMODE GetCameraMode() { return m_eCameraMode; }
	void ShakeCamera(_float fDeltaTime);
	_vector GetCameraPosition() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_vector GetCameraLook() { return m_pTransformCom->Get_State(CTransform::STATE_LOOK); }

private:
	void SetFirstCamera();
	void SetThirdCamera();
	void SetEditorCamera();

private:
	_float				m_fSensor = { 0.f };
	CAMERAMODE			m_eCameraMode = CAMERAMODE::CAMERA_OFF;
	_float				m_fCameraUp_First = 1.f; // <- 1인칭 모드에서 카메라 위치 조정 

	_float3				m_fCamera_Third = { 0.f,10.f,-10.f }; 
	_float3				m_fCamera_Third_Look = { 0.f,0.f,0.f };

	_long				m_lCamera_Y_move_First = 0.f;


public:
	static CFreeCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END