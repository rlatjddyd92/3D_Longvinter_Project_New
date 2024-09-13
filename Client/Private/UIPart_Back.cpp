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

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_Back::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CUIPart_Back::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Back::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
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
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_UIPart_Back"),
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
