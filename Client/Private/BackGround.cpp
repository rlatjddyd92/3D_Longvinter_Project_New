#include "stdafx.h"
#include "..\Public\BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUIObject{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround & Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround::Initialize(void * pArg)
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_1) & 0x80)
	{
		m_bChangeColor[0] = true;
		m_bChangeColor[1] = false;
		m_bChangeColor[2] = false;
	}
		

	if (m_pGameInstance->Get_DIKeyState(DIK_2) & 0x80)
	{
		m_bChangeColor[0] = false;
		m_bChangeColor[1] = true;
		m_bChangeColor[2] = false;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_3) & 0x80)
	{
		m_bChangeColor[0] = false;
		m_bChangeColor[1] = false;
		m_bChangeColor[2] = true;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_0) & 0x80)
	{
		m_bChangeColor[0] = false;
		m_bChangeColor[1] = false;
		m_bChangeColor[2] = false;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_4, true) & 0x80)
	{
		m_fRGB[0] += fTimeDelta;
		m_fRGB[1] += fTimeDelta;
		m_fRGB[2] += fTimeDelta;
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_5, true) & 0x80)
	{
		m_fRGB[0] -= fTimeDelta;
		m_fRGB[1] -= fTimeDelta;
		m_fRGB[2] -= fTimeDelta;
	}

	int a = 10;
}

void CBackGround::Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
	__super::Late_Update(fTimeDelta);
	

	m_pGameInstance->Add_RenderObject(CRenderer::RG_PRIORITY, this);
}

HRESULT CBackGround::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", 1)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ChangeColor("g_IsChange", "g_ChangeColor",m_bChangeColor, m_fRGB)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*		pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CBackGround::Clone(void * pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
