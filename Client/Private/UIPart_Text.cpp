#include "stdafx.h"
#include "..\Public\UIPart_Text.h"

#include "GameInstance.h"

CUIPart_Text::CUIPart_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Text::CUIPart_Text(const CUIPart_Text& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Text::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Text::Initialize(void* pArg)
{
	UITEXT_DESC* Desc = static_cast<UITEXT_DESC*>(pArg);

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

void CUIPart_Text::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CUIPart_Text::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Text::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_Text::Render()
{
	// __super::Render(); <- Text ��Ʈ�� FontManager�� ���� ���� ���� 

	return S_OK;
}

HRESULT CUIPart_Text::Ready_Components()
{

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_UIPart_Text"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;





	return S_OK;
}

CUIPart_Text* CUIPart_Text::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Text* pInstance = new CUIPart_Text(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Text"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Text::Clone(void* pArg)
{
	CUIPart_Text* pInstance = new CUIPart_Text(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Text"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Text::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
