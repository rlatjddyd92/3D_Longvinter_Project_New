#include "stdafx.h"
#include "..\Public\UIPart.h"

#include "GameInstance.h"

CUIPart::CUIPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIPart::CUIPart(const CUIPart& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUIPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart::Initialize(void* pArg)
{
	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart::Priority_Update(_float fTimeDelta)
{
	



	int a = 10;
}

void CUIPart::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor, m_fRGB)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_ChangeAlpah("g_Istransparency", "g_TransAlpah", &m_bTransParent, &m_fAlpah)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CUIPart::AddRender_UIPart()
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPart::Change_Back_Color(_float fR, _float fG, _float fB, _float fAlpah)
{
	if (fR > 0.f)
	{
		m_bChangeColor[0] = true;
		m_fRGB[0] = fR;
	}
	else if (fR == -1)
		m_bChangeColor[0] = false;
	if (fG > 0.f)
	{
		m_bChangeColor[1] = true;
		m_fRGB[1] = fG;
	}
	else if (fG == -1)
		m_bChangeColor[1] = false;
	if (fB > 0.f)
	{
		m_bChangeColor[2] = true;
		m_fRGB[2] = fB;
	}
	else if (fB == -1)
		m_bChangeColor[2] = false;
	if (fAlpah > 0.f)
	{
		m_bTransParent = true;
		m_fAlpah = fR;
	}
	else if (fAlpah == -1)
		m_bTransParent = false;
}

HRESULT CUIPart::Ready_Components()
{

	
	/* FOR.Com_Shader */

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	

	/* FOR.Com_VIBuffer */

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

_bool CUIPart::SetPositionByObject(_matrix mHost)
{
	_matrix mProj = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ));
	_matrix mView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

	_vector vResult = { m_fAdjust.x, m_fAdjust.y, m_fAdjust.z, 0.f };

	// 투영 좌표 계산
	vResult = XMVector3Transform(vResult, mHost);
	vResult = XMVector3Transform(vResult, mView);
	vResult = XMVector3Transform(vResult, mProj);



	// W나누기
	_float4 fResult{};
	XMStoreFloat4(&fResult, vResult);

	if (fResult.w < 0.f)
		return false;


	m_fX = fResult.x / fResult.w;
	m_fY = fResult.y / fResult.w;

	// 스크린 좌표로 변환
	m_fX = ((m_fX + 1.f) * 0.5) * 1280.f;
	m_fY = ((1.f - m_fY) * 0.5) * 720.f;

	return true;
}

CUIPart* CUIPart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart* pInstance = new CUIPart(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart::Clone(void* pArg)
{
	CUIPart* pInstance = new CUIPart(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
