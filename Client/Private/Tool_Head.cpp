#include "stdafx.h"
#include "..\Public\Tool_Head.h"

#include "Player.h"

#include "GameInstance.h"

const _float4x4* CTool_Head::Get_BoneMatrix_Ptr(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(pBoneName);
}

CTool_Head::CTool_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool{ pDevice, pContext }
{
}

CTool_Head::CTool_Head(const CTool_Head& Prototype)
	: CTool{ Prototype }
{
}

HRESULT CTool_Head::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Head::Initialize(void* pArg)
{
	CTool_Head::HEAD_DESC* pDesc = static_cast<CTool_Head::HEAD_DESC*>(pArg);

	m_bEmptyPart = false;
	m_eType = pDesc->eType;
	//m_eType = TYPE_BROWN;

	CTool::TOOL_DESC* pTool = static_cast<CTool::TOOL_DESC*>(pDesc);

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pTool)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/*m_pTransformCom->Set_Scaled(0.1f, 0.1f, 0.1f);
	m_pTransformCom->Rotation(0.0f, XMConvertToRadians(90.0f), 0.1f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.5f, 0.f, 0.f, 1.f));*/

	m_pTransformCom->Set_RotationPerSec(1.f);

	//m_pTransformCom->Rotation({ false,true,false }, XMConvertToRadians(180.f));

	SetOff(false);
	SetOn();

	return S_OK;
}

void CTool_Head::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	int a = 10;
}

void CTool_Head::Update(_float fTimeDelta)
{
	//__super::Update(fTimeDelta);

	
}

void CTool_Head::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);






	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTool_Head::Render()
{
	m_pTransformCom->Rotation(XMConvertToRadians(90.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) - _vector{ 1.f,0.f,0.f,0.f });

	//__super::Render();

	_matrix		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	for (size_t i = 0; i < 3; i++)
	{
		SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
	}
	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));



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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _vector{ 1.f,0.f,0.f,0.f });

	return S_OK;
}

HRESULT CTool_Head::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (m_eType == TYPE_NORMAL_1)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Normal_1"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_NORMAL_2)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Normal_2"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_NORMAL_3)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Normal_3"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_BROWN)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Brown"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_YELLOW)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Yellow"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_PINK)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Pink"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_BLUE)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_Blue"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}
	else if (m_eType == TYPE_HEADGEAR)
	{
		/* FOR.Com_Model */
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Head_HeadGear"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}



	return S_OK;
}

CTool_Head* CTool_Head::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Head* pInstance = new CTool_Head(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTool_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CTool_Head::Clone(void* pArg)
{
	CTool_Head* pInstance = new CTool_Head(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTool_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Head::Free()
{
	__super::Free();

	//Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
