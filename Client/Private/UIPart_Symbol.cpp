#include "stdafx.h"
#include "..\Public\UIPart_Symbol.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPart_Symbol::CUIPart_Symbol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Symbol::CUIPart_Symbol(const CUIPart_Symbol& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Symbol::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Symbol::Initialize(void* pArg)
{
	UISymbol_DESC* Desc = static_cast<UISymbol_DESC*>(pArg);

	m_eType = Desc->eType;
	m_pHost = Desc->pHost;

	Safe_AddRef(m_pHost);

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	m_fAdjust = { 0.f, 3.f, 0.f };

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 35.f;
	m_fSizeY = 35.f;
	



	return S_OK;
}

void CUIPart_Symbol::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CUIPart_Symbol::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Symbol::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
	__super::Late_Update(fTimeDelta);

	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
	{
		__super::SetDead();
		return;
	}

	


	__super::SetRender(true);

	if (m_pHost->Get_AI_Status() == AI_STATUS::AI_ATTACK)
		m_eType = SYMBOL_FIGHT;
	else if ((m_pHost->Get_AI_Status() == AI_STATUS::AI_SERACH) && (m_pHost->GetContainerType() != CONTAINER::CONTAINER_NPC))
		m_eType = SYMBOL_SEARCHING;
	else if (m_pHost->Get_AI_Status() == AI_STATUS::AI_PANIC)
		m_eType = SYMBOL_PANIC;
	else if (m_pHost->GetContainerType() == CONTAINER::CONTAINER_NPC)
	{
		if (static_cast<CContainer_NPC*>(m_pHost)->Get_NPC_Type() == NPC_TYPE::NPC_LAST)
		{
			if ((GET_INSTANCE->IsStartQuest()) && (!GET_INSTANCE->IsFinishQuest()))
				m_eType = SYMBOL_QUEST;
			else
				__super::SetRender(false);
		}
		else
			__super::SetRender(false);
	}
	else
		__super::SetRender(false);

}

HRESULT CUIPart_Symbol::Render()
{
	if ((m_pHost == nullptr) || (m_pHost->GetDead()))
	{
		__super::SetDead();
		return S_OK;
	}

	if (m_eType != SYMBOL_QUEST)
	{
		_float3 fPosition{};
		_vector vPosition = m_pHost->GetTransform(CTransform::STATE_POSITION);
		XMStoreFloat3(&fPosition, vPosition);

		if (!GET_INSTANCE->GetIsLender(fPosition))
			return S_OK;

		
	}

	if (!SetPositionByObject(XMLoadFloat4x4(&m_pHost->GetWorldMatrix())))
		return S_OK;
	
	if (m_eType == SYMBOL_QUEST)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 0.f / 255.f;
		m_fRGB[1] = 255.f / 255.f;
		m_fRGB[2] = 0.f / 255.f;
	}
	else if (m_eType == SYMBOL_SEARCHING)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 255.f / 255.f;
		m_fRGB[1] = 215.f / 255.f;
		m_fRGB[2] = 0.f / 255.f;
	}
	else if (m_eType == SYMBOL_STUN)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 000.f / 255.f;
		m_fRGB[1] = 000.f / 255.f;
		m_fRGB[2] = 000.f / 255.f;
	}
	else if (m_eType == SYMBOL_FIGHT)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 255.f / 255.f;
		m_fRGB[1] = 000.f / 255.f;
		m_fRGB[2] = 000.f / 255.f;
	}
	else if (m_eType == SYMBOL_PANIC)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 000.f / 255.f;
		m_fRGB[1] = 000.f / 255.f;
		m_fRGB[2] = 000.f / 255.f;
	}
	

	__super::Render();

	if (m_eType == SYMBOL_QUEST)
		m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("Q"), { m_fX,m_fY,0.f,0.f }, 1.f, true, { 0.f,0.f,0.f,1.f });
	else if (m_eType == SYMBOL_SEARCHING)
		m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("??"), { m_fX,m_fY,0.f,0.f }, 1.f, true, { 0.f,0.f,0.f,1.f });
	else if (m_eType == SYMBOL_STUN)
		m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("ZZ"), { m_fX,m_fY,0.f,0.f }, 1.f, true, { 255.f,255.f,255.f,1.f });
	else if (m_eType == SYMBOL_FIGHT)
		m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("!!!"), { m_fX,m_fY,0.f,0.f }, 1.f, true, { 0.f,0.f,0.f,1.f });
	else if (m_eType == SYMBOL_PANIC)
		m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("!!!"), { m_fX,m_fY,0.f,0.f }, 1.f, true, { 255.f,255.f,255.f,1.f });



	return S_OK;
}

HRESULT CUIPart_Symbol::Ready_Components()
{
	
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Symbol_Back"),
			TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom)), 1))
			return E_FAIL;


	
	


	return S_OK;
}

CUIPart_Symbol* CUIPart_Symbol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Symbol* pInstance = new CUIPart_Symbol(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Symbol"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Symbol::Clone(void* pArg)
{
	CUIPart_Symbol* pInstance = new CUIPart_Symbol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Symbol"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Symbol::Free()
{
	__super::Free();


	Safe_Release(m_pHost);
}
