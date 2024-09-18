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
	m_iNewPage = 0;
}

void CUIManager::Update(_float fTimeDelta)
{
	for (list<CUIPage*>::iterator iter = m_Pagelist.begin(); iter != m_Pagelist.end();++iter)
	{
		if (!(*iter)->GetOff())
			if ((*iter)->Key_Action())
			{
				CUIPage* pTemp = (*iter);

				m_Pagelist.erase(iter);
				m_Pagelist.push_front(pTemp);
				break;
			}
				
	}
}

void CUIManager::Late_Update(_float fTimeDelta)
{
	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_LOGO)
	{
		m_pPage_Main->SetOn();
		m_pPage_Main->AddRender_UIPage();
	}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_O))
			if (m_pPage_Test->GetOff())
		{
			m_pPage_Test->SetOn();
			++m_iNewPage;
			m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Test));
		}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_I))
			if (m_pPage_Inven->GetOff())
		{
			m_pPage_Inven->SetOn();
			++m_iNewPage;
			m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Inven));
		}
			
	for (list<CUIPage*>::iterator iter = m_Pagelist.begin(); iter != m_Pagelist.end();)
	{
		if (!(*iter)->GetOff())
		{
			(*iter)->AddRender_UIPage();
			++iter;
		}
		else
			iter = m_Pagelist.erase(iter);
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

	m_pPage_Test = GET_INSTANCE->MakeUIPage_Test();
	Safe_AddRef(m_pPage_Test);

	m_pPage_Inven = GET_INSTANCE->MakeUIPage_Inven();
	Safe_AddRef(m_pPage_Inven);
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
	Safe_Release(m_pPage_Test);
	Safe_Release(m_pPage_Inven);
	

	m_Pagelist.clear();
}
