#include "stdafx.h"
#include "..\Public\SurFace.h"

#include "GameInstance.h"

CSurFace::CSurFace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSurFace::CSurFace(const CSurFace& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSurFace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSurFace::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		Desc{};

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSurFace::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CSurFace::Update(_float fTimeDelta)
{
	int a = 10;
}

void CSurFace::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CSurFace::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pManagerTexture->Bind_ShadeResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CSurFace::SetPosition(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec, CTexture* ManagerTexture )
{
	m_pManagerTexture = ManagerTexture;

	XMVECTOR NewPosition = { LCUBESIZE * 0.5f,  LCUBESIZE * 0.5f ,  LCUBESIZE * 0.5f };
	NewPosition += {iX* LCUBESIZE, iY* LCUBESIZE, iZ* LCUBESIZE};

	if (eDirec == LCUBEDIRECION::LDIREC_TOP)
	{
		m_pTransformCom->Rotation(_fvector({ 1.f, 0.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { 0, LCUBESIZE * 0.5f, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_BOTTOM)
	{
		m_pTransformCom->Rotation(_fvector({ -1.f, 0.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { 0, -LCUBESIZE * 0.5f, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_WEST)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, 1.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { -LCUBESIZE * 0.5f, 0, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_EAST)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, -1.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { LCUBESIZE * 0.5f, 0, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_NORTH)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, -1.f, 0.f }), _float(XMConvertToRadians(180.f)));
		NewPosition += { 0,0, LCUBESIZE * 0.5f };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_SOUTH)
	{
		NewPosition += { 0, 0, -LCUBESIZE * 0.5f };
	}
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, NewPosition);
}

_float3 CSurFace::IsPicking()
{
	_float3 vPickPos{ -1,-1,-1 };
	m_pVIBufferCom->isPicking(m_pTransformCom->Get_WorldMatrix(), &vPickPos);
	return vPickPos;
}

void CSurFace::SetTransForm(CTransform::STATE eState, XMVECTOR vecNew)
{
	m_pTransformCom->Set_State(eState, vecNew);


}

HRESULT CSurFace::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect3D"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CSurFace* CSurFace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSurFace* pInstance = new CSurFace(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSurFace"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CSurFace::Clone(void* pArg)
{
	CSurFace* pInstance = new CSurFace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSurFace"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSurFace::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
