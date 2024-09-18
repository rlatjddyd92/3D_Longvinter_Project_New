#include "stdafx.h"
#include "..\Public\UIPart_Back.h"

#include "GameInstance.h"

CUIPart_Back::CUIPart_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Back::CUIPart_Back(const CUIPart_Back& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Back::Initialize(void* pArg)
{
	UIBack_DESC* Desc = static_cast<UIBack_DESC*>(pArg);

	m_eType = Desc->eType;

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	if (m_eType == BACK_INGAME_WINDOW)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 255.f / 255.f;
		m_fRGB[1] = 222.f / 255.f;
		m_fRGB[2] = 173.f / 255.f;
	}
	if (m_eType == BACK_INGAME_WINDOW_HEADER)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 210.f / 255.f;
		m_fRGB[1] = 180.f / 255.f;
		m_fRGB[2] = 140.f / 255.f;
	}

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_Back::Priority_Update(_float fTimeDelta)
{
	m_bPushed[0] = m_bPushed[1];
	m_bPushed[1] = false;
	m_bOnCursor = false;
	POINT mousePos{};

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	if (((m_fX - m_fSizeX / 2.f) < mousePos.x) + ((m_fX + m_fSizeX / 2.f) > mousePos.x) == 2)
		if (((m_fY - m_fSizeY / 2.f) < mousePos.y) + ((m_fY + m_fSizeY / 2.f) > mousePos.y) == 2)
		{
			m_bOnCursor = true;
			if (m_pGameInstance->Get_DIMouseState(DIMK_LBUTTON, true))
				m_bPushed[1] = true;
		}

	int a = 10;
}

void CUIPart_Back::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Back::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_Back::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUIPart_Back::Ready_Components()
{

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

CUIPart_Back* CUIPart_Back::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Back* pInstance = new CUIPart_Back(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Back"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Back::Clone(void* pArg)
{
	CUIPart_Back* pInstance = new CUIPart_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Back"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Back::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
