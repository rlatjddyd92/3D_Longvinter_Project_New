#include "stdafx.h"
#include "..\Public\Body.h"

#include "Player.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CBody::CBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBody::CBody(const CBody& Prototype)
	: CPartObject{ Prototype }
{
}

const _float4x4* CBody::Get_BoneMatrix_Ptr(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(pBoneName);
}

HRESULT CBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody::Initialize(void* pArg)
{
	BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	
	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pModelCom->SetUp_Animation(3, true);

	return S_OK;
}

void CBody::Priority_Update(_float fTimeDelta)
{
	
	m_bNonLoopAnimStart = false;
	int a = 10;
}

void CBody::Update(_float fTimeDelta)
{

	
	
}

void CBody::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

	if (m_iBeforeState != *m_pParentState)
		if ((m_bLoop == true) || (m_bNonLoopAnimStart == true))
	{
			m_pModelCom->AnimReset();
			m_iBeforeState = *m_pParentState;
			m_pModelCom->SetUp_Animation(m_iBeforeState, m_bLoop);
			m_fAnimTime_Deposit = 0.f;
	}

	_float3 fPosition = { m_pParentMatrix->m[3][0], m_pParentMatrix->m[3][1], m_pParentMatrix->m[3][2] };

	m_bIsEnd = false;

	if (!GET_INSTANCE->GetIsLender(fPosition))
	{
		m_fAnimTime_Deposit += fTimeDelta;
	}
	else
	{
		if (m_pModelCom->Play_Animation(fTimeDelta + m_fAnimTime_Deposit))
		{
			m_bIsEnd = true;
		}

		m_fAnimTime_Deposit = 0.f;
	}




	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody::Render()
{
	_float3 fPosition = { m_pParentMatrix->m[3][0], m_pParentMatrix->m[3][1], m_pParentMatrix->m[3][2] };
	
	if (!GET_INSTANCE->GetIsLender(fPosition))
		return S_OK;

	if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", aiTextureType_NORMALS, i)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CBody::Start_NonLoopAnim()
{
	m_bNonLoopAnimStart = true;
}

HRESULT CBody::Ready_Components()
{
	

	

	return S_OK;
}

CBody* CBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody* pInstance = new CBody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CBody::Clone(void* pArg)
{
	CBody* pInstance = new CBody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBody"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
