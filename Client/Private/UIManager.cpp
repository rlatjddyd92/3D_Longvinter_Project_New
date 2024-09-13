#include "stdafx.h"
#include "UIManager.h"
#include "ClientInstance.h"

CUIManager::CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CUIManager::CUIManager(const CUIManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUIManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIManager::Initialize(void* pArg)
{
	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_UIPage();

	return S_OK;
}

void CUIManager::Priority_Update(_float fTimeDelta)
{
}

void CUIManager::Update(_float fTimeDelta)
{
}

void CUIManager::Late_Update(_float fTimeDelta)
{
	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_LOGO)
	{
		m_pPage_Main->SetOn();
		m_pPage_Main->AddRender_UIPage();
	}



	

}

HRESULT CUIManager::Render()
{
	return S_OK;
}

HRESULT CUIManager::Ready_Components()
{
	return S_OK;
}

HRESULT CUIManager::Ready_PartObjects()
{
	return S_OK;
}

void CUIManager::Ready_UIPage()
{
	m_pPage_Main = GET_INSTANCE->MakeUIPage_Main();
	Safe_AddRef(m_pPage_Main);
}

CUIManager* CUIManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIManager* pInstance = new CUIManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIManager::Clone(void* pArg)
{
	CUIManager* pInstance = new CUIManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIManager::Free()
{
	__super::Free();

	Safe_Release(m_pPage_Main);
}
