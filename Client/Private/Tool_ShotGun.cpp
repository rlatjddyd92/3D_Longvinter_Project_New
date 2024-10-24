#include "stdafx.h"
#include "..\Public\Tool_ShotGun.h"

#include "Player.h"

#include "GameInstance.h"

CTool_ShotGun::CTool_ShotGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{ pDevice, pContext }
{
}

CTool_ShotGun::CTool_ShotGun(const CTool_ShotGun& Prototype)
	: CTool{ Prototype }
{
}

HRESULT CTool_ShotGun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_ShotGun::Initialize(void* pArg)
{
	CTool::TOOL_DESC* pDesc = static_cast<CTool::TOOL_DESC*>(pArg);
	
	m_bEmptyPart = false;

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_Scaled(0.1f, 0.1f, 0.1f);
	//m_pTransformCom->Rotation(0.0f, XMConvertToRadians(90.0f), 0.1f);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.5f, 0.f, 0.f, 1.f));

	m_pTransformCom->Set_RotationPerSec(1.f);

	//m_pTransformCom->Rotation({ false,true,false }, XMConvertToRadians(180.f));
	

	return S_OK;
}

void CTool_ShotGun::Priority_Update(_float fTimeDelta)
{
	
	int a = 10;
}

void CTool_ShotGun::Update(_float fTimeDelta)
{
	


}

void CTool_ShotGun::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);






	//m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTool_ShotGun::Render()
{
	m_pTransformCom->Rotation(14.6963120f, 30.0189304f, 7.91829014f);

	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
	{
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
	}

	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * SocketMatrix * XMLoadFloat4x4(&m_RenderMatrix));

	//__super::Render();

	if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

#ifdef _DEBUG
	//m_pColliderCom->Render();
#endif

	return S_OK;
}

HRESULT CTool_ShotGun::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Shotgun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	///* FOR.Com_Collider */
	//CBounding_OBB::BOUNDING_OBB_DESC			ColliderDesc{};
	//ColliderDesc.vExtents = _float3(0.5f, 0.8f, 1.3f);
	//ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
	//ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	//if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_SINGLE), TEXT("Prototype_Component_Collider_OBB"),
	//	TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
	//	return E_FAIL;





	return S_OK;
}

CTool_ShotGun* CTool_ShotGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_ShotGun* pInstance = new CTool_ShotGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTool_ShotGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CTool_ShotGun::Clone(void* pArg)
{
	CTool_ShotGun* pInstance = new CTool_ShotGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTool_ShotGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_ShotGun::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
