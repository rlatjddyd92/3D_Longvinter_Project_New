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

	m_bTransParent = false;

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

	m_bShowTooltip = false;

	POINT mousePos{};

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	m_fX = mousePos.x + (m_fSizeX * 0.5f);
	m_fY = mousePos.y + (m_fSizeY * 0.5f);
}

void CUIManager::Update(_float fTimeDelta)
{
	if (!m_Pagelist.empty())
	{
		list<CUIPage*>::iterator iter = m_Pagelist.end();
		--iter;

		while (1)
		{
			if (!(*iter)->GetOff())
				if ((*iter)->Key_Action())
				{
					CUIPage* pTemp = (*iter);

					m_Pagelist.erase(iter);
					m_Pagelist.push_back(pTemp);
					break;
				}

			if (iter == m_Pagelist.begin())
				break;
			else
				--iter;
		}
	}





	//for (list<CUIPage*>::iterator iter = m_Pagelist.begin(); iter != m_Pagelist.end();++iter)
	//{
	//	if (!(*iter)->GetOff())
	//		if ((*iter)->Key_Action())
	//		{
	//			CUIPage* pTemp = (*iter);

	//			m_Pagelist.erase(iter);
	//			m_Pagelist.push_back(pTemp);
	//			break;
	//		}
	//			
	//}
}

void CUIManager::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pPage_User->AddRender_UIPage();


	for (list<CUIPart_Bar*>::iterator iter = m_EnemyHplist.begin(); iter != m_EnemyHplist.end();)
	{
		if (((*iter) == nullptr) || ((*iter)->GetDead()))
		{
			iter = m_EnemyHplist.erase(iter);
		}
		else
		{
			(*iter)->AddRender_UIPart();
			++iter;
		}
	}

	for (list<CUIPart_Symbol*>::iterator iter = m_Symbollist.begin(); iter != m_Symbollist.end();)
	{
		if (((*iter) == nullptr) || ((*iter)->GetDead()))
		{
			iter = m_Symbollist.erase(iter);
		}
		else
		{
			(*iter)->AddRender_UIPart();
			++iter;
		}
	}

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
			m_Pagelist.push_back(static_cast<CUIPage*>(m_pPage_Test));
		}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_I))
			if (m_pPage_Inven->GetOff())
		{
			m_pPage_Inven->SetOn();
			m_Pagelist.push_back(static_cast<CUIPage*>(m_pPage_Inven));
		}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_P))
			if (m_pPage_Equip->GetOff())
			{
				m_pPage_Equip->SetOn();
				m_Pagelist.push_back(static_cast<CUIPage*>(m_pPage_Equip));
			}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_K))
			if (m_pPage_Crafting->GetOff())
			{
				m_pPage_Crafting->SetOn();
				m_Pagelist.push_back(static_cast<CUIPage*>(m_pPage_Crafting));
			}

	if (GET_INSTANCE->GetNowLevel() == LEVELID::LEVEL_GAMEPLAY)
		if (m_pGameInstance->Get_DIKeyState(DIK_O))
			if (m_pPage_Option->GetOff())
			{
				m_pPage_Option->SetOn();
				m_Pagelist.push_back(static_cast<CUIPage*>(m_pPage_Option));
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

	if (m_bShowTooltip)
		m_pPage_ToolTip->AddRender_UIPage();


	_int iInformNum = 0;

	for (list<CUIPart_TextBox*>::iterator iter = m_Informlist.begin(); iter != m_Informlist.end();)
	{
		if (!(*iter)->GetDead())
		{
			
			(*iter)->Set_UIPosition(g_iWinSizeX >> 1, 70.f + (30.f * iInformNum));
			(*iter)->AddRender_UIPart();
			++iInformNum;
			++iter;
		}
		else
		{
			//Safe_Release(*iter);
			iter = m_Informlist.erase(iter);
		}
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIManager::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	ITEMINDEX eNow = GET_INSTANCE->GetPickedItemIndex();

	if (eNow == ITEMINDEX::ITEM_END)
	{
		m_fSizeX = 20.f;
		m_fSizeY = 20.f;
		if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
			return E_FAIL;
	}
	else
	{
		m_fSizeX = 30.f;
		m_fSizeY = 30.f;
		CTexture* pItemTexture = GET_INSTANCE->GetItemInvenTexture(eNow);
		if (FAILED(pItemTexture->Bind_ShadeResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor, m_fRGB)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_ChangeAlpah("g_Istransparency", "g_TransAlpah", &m_bTransParent, &m_fAlpah)))
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





void CUIManager::ShowInformMessage(wstring Text)
{
	while (m_Informlist.size() >= m_iMaxInform)
		m_Informlist.pop_front();

	CUIPart_TextBox* pNew = GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_NOTICE, g_iWinSizeX >> 1, 0.f, 500.f, 25.f, true, true, 3.f);
	pNew->SetText(Text);
	pNew->SetSize(0.5f);

	m_Informlist.push_back(pNew);
	//Safe_AddRef(pNew);

}


void CUIManager::MakeEnemyHpBar(CLongvinter_Container* pHost)
{
	CUIPart_Bar* pNew = GET_INSTANCE->MakeUIPart_Bar(CUIPart_Bar::BAR_ENEMY_HP, 0.f, 0.f, 0.f, 0.f, pHost);
	m_EnemyHplist.push_back(pNew);
}

void CUIManager::MakeSymbol(CLongvinter_Container* pHost)
{
	CUIPart_Symbol* pNew = GET_INSTANCE->MakeUIPart_Symbol(CUIPart_Symbol::SYMBOL_END, 0.f, 0.f, 0.f, 0.f, pHost);
	m_Symbollist.push_back(pNew);
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

	m_pPage_ToolTip = GET_INSTANCE->MakeUIPage_ToolTip();
	Safe_AddRef(m_pPage_ToolTip);

	m_pPage_User = GET_INSTANCE->MakeUIPage_User();
	Safe_AddRef(m_pPage_User);
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
	Safe_Release(m_pPage_ToolTip);
	Safe_Release(m_pPage_User);
	

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom); 

	for (auto& iter : m_Informlist)
		Safe_Release(iter);

	m_Informlist.clear();

	for (auto& iter : m_EnemyHplist)
		Safe_Release(iter);
	
	m_EnemyHplist.clear();

	for (auto& iter : m_Symbollist)
		Safe_Release(iter);

	m_Symbollist.clear();

	m_Pagelist.clear();
}
