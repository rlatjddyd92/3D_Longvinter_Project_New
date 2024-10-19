#include "stdafx.h"
#include "..\Public\UIPart_Picture.h"

#include "GameInstance.h"

CUIPart_Picture::CUIPart_Picture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Picture::CUIPart_Picture(const CUIPart_Picture& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Picture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Picture::Initialize(void* pArg)
{
	UIPicture_DESC* Desc = static_cast<UIPicture_DESC*>(pArg);

	m_eType = Desc->eType;

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_Picture::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CUIPart_Picture::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Picture::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_Picture::Render()
{
	__super::Render();

	

	return S_OK;
}

HRESULT CUIPart_Picture::Ready_Components()
{

	if (m_eType == PICTUER_BGA)
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BGA"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom)), 6))
			return E_FAIL;

	if (m_eType == PICTUER_MAINLOGO)
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MainLogo"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom)), 1))
			return E_FAIL;

	if (m_eType == PICTUER_CASH)
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cash"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom)), 1))
			return E_FAIL;

	


	return S_OK;
}

CUIPart_Picture* CUIPart_Picture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Picture* pInstance = new CUIPart_Picture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Picture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Picture::Clone(void* pArg)
{
	CUIPart_Picture* pInstance = new CUIPart_Picture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Picture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Picture::Free()
{
	__super::Free();


}
