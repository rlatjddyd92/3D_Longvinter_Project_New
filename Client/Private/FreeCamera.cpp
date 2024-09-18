#include "stdafx.h"
#include "..\Public\FreeCamera.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CFreeCamera::CFreeCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera { pDevice, pContext }
{
}

CFreeCamera::CFreeCamera(const CFreeCamera & Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CFreeCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFreeCamera::Initialize(void * pArg)
{
	CAMERA_FREE_DESC*		pDesc = static_cast<CAMERA_FREE_DESC*>(pArg);

	m_fSensor = pDesc->fSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CFreeCamera::Priority_Update(_float fTimeDelta)
{
	if (m_eCameraMode == CAMERA_FIRST)
	{
		_vector vPosition = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
		vPosition += {0.f, m_fCameraUp_First, 0.f};
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_LOOK));
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_UP));



		m_pTransformCom->Go_Right(0.03f);
		m_pTransformCom->Go_Up(0.01f);
		m_pTransformCom->Go_Backward(0.04f);
	}
	else if (m_eCameraMode == CAMERA_THIRD)
	{
		_vector vPosition = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
		vPosition += {m_fCamera_Third.x, m_fCamera_Third.y, m_fCamera_Third.z, 0.f};
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		_vector vLook = { 0.f, -0.7f, 0.7f, 0.f };
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);
		_vector vRight = { 1.f, 0.f, 0.f, 0.f };
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		_vector vUp = { 0.f, 0.7f, 0.7f, 0.f };
		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp);

	}
	else if (m_eCameraMode == CAMERA_EDITOR)
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_W,true) & 0x80)
			m_pTransformCom->Go_Straight(fTimeDelta);
		if (m_pGameInstance->Get_DIKeyState(DIK_S, true) & 0x80)
			m_pTransformCom->Go_Backward(fTimeDelta);
		if (m_pGameInstance->Get_DIKeyState(DIK_A, true) & 0x80)
			m_pTransformCom->Go_Left(fTimeDelta);
		if (m_pGameInstance->Get_DIKeyState(DIK_D, true) & 0x80)
			m_pTransformCom->Go_Right(fTimeDelta);

		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT, true) & 0x80)
		{
			_long		MouseMove = { 0 };

			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fSensor);
			}

			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
			{
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * m_fSensor);
			}
		}
	}
	__super::Priority_Update(fTimeDelta);
}

void CFreeCamera::Update(_float fTimeDelta)
{
}

void CFreeCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CFreeCamera::Render()
{
	return S_OK;
}

void CFreeCamera::SetCameraMode(CAMERAMODE eInput)
{
	m_eCameraMode = eInput;
	
	if (m_eCameraMode == CAMERAMODE::CAMERA_OFF)
	{
		m_bOff = true;
		return;
	}
		
	m_bOff = false;
	if (m_eCameraMode == CAMERAMODE::CAMERA_FIRST)
		SetFirstCamera();
	else if (m_eCameraMode == CAMERAMODE::CAMERA_THIRD)
		SetThirdCamera();
	else if (m_eCameraMode == CAMERAMODE::CAMERA_EDITOR)
		SetEditorCamera();
}

void CFreeCamera::ShakeCamera(_float fDeltaTime)
{
}

void CFreeCamera::SetFirstCamera()
{
	_vector vPosition = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
	vPosition += {0.f, m_fCameraUp_First, 0.f};
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_LOOK));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_RIGHT));
	m_pTransformCom->Set_State(CTransform::STATE_UP, GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_UP));

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
}
void CFreeCamera::SetThirdCamera()
{
	_vector vPosition = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
	vPosition += {m_fCamera_Third.x, m_fCamera_Third.y, m_fCamera_Third.z, 0.f};
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	

}

void CFreeCamera::SetEditorCamera()
{

}

CFreeCamera * CFreeCamera::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFreeCamera*		pInstance = new CFreeCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFreeCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CFreeCamera::Clone(void * pArg)
{
	CFreeCamera*		pInstance = new CFreeCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFreeCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFreeCamera::Free()
{
	__super::Free();

}
