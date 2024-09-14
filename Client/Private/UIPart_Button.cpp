#include "stdafx.h"
#include "..\Public\UIPart_Button.h"

#include "GameInstance.h"

CUIPart_Button::CUIPart_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Button::CUIPart_Button(const CUIPart_Button& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Button::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Button::Initialize(void* pArg)
{
	UIButton_DESC* Desc = static_cast<UIButton_DESC*>(pArg);

	m_eType = Desc->eType;

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	if (m_eType == BUTTON_INGAME)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 000.f / 255.f;
		m_fRGB[1] = 000.f / 255.f;
		m_fRGB[2] = 153.f / 255.f;
	}
	if (m_eType == BUTTON_EDITOR)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 204.f / 255.f;
		m_fRGB[1] = 000.f / 255.f;
		m_fRGB[2] = 000.f / 255.f;
	}

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_Button::Priority_Update(_float fTimeDelta)
{
	

	int a = 10;
}

void CUIPart_Button::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Button::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_Button::Render()
{
	

	__super::Render();

	

	return S_OK;
}

HRESULT CUIPart_Button::Ready_Components()
{

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

CUIPart_Button* CUIPart_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Button* pInstance = new CUIPart_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Button::Clone(void* pArg)
{
	CUIPart_Button* pInstance = new CUIPart_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Button::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
