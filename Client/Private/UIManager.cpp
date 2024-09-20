#include "stdafx.h"
#include "UIManager.h"
#include "ClientInstance.h"

CUIManager::CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice ,pContext }
{
}

CUIManager::CUIManager(const CUIManager& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUIManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIManager::Initialize(void* pArg)
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = 20.f;
	Desc.fSizeY = 20.f;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);


	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_UIPage();

	return S_OK;
}

void CUIManager::Priority_Update(_float fTimeDelta)
{
	m_iTextureIndex = 0;

	POINT mousePos{};

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	m_fX = mousePos.x + (m_fSizeX * 0.5f);
	m_fY = mousePos.y + (m_fSizeY * 0.5f);
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
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_LOGO)
	{
		m_pPage_Main->SetOn();
		m_pPage_Main->AddRender_UIPage();
	}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_L))
			if (m_pPage_Test->GetOff())
		{
			m_pPage_Test->SetOn();
			m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Test));
		}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_I))
			if (m_pPage_Inven->GetOff())
		{
			m_pPage_Inven->SetOn();
			m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Inven));
		}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_P))
			if (m_pPage_Equip->GetOff())
			{
				m_pPage_Equip->SetOn();
				m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Equip));
			}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_K))
			if (m_pPage_Crafting->GetOff())
			{
				m_pPage_Crafting->SetOn();
				m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Crafting));
			}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_O))
			if (m_pPage_Option->GetOff())
			{
				m_pPage_Option->SetOn();
				m_Pagelist.push_front(static_cast<CUIPage*>(m_pPage_Option));
			}


	// 렌더링 준비 

	
			
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

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;


	return S_OK;
}

HRESULT CUIManager::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cursor"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

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

	m_pPage_Equip = GET_INSTANCE->MakeUIPage_Equip();
	Safe_AddRef(m_pPage_Equip);

	m_pPage_Crafting = GET_INSTANCE->MakeUIPage_Crafting();
	Safe_AddRef(m_pPage_Crafting);

	m_pPage_Option = GET_INSTANCE->MakeUIPage_Option();
	Safe_AddRef(m_pPage_Option);
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
	Safe_Release(m_pPage_Equip);
	Safe_Release(m_pPage_Crafting);
	Safe_Release(m_pPage_Option);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	m_Pagelist.clear();
}
