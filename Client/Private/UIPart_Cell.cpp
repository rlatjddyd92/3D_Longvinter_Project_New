#include "stdafx.h"
#include "..\Public\UIPart_Cell.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPart_Cell::CUIPart_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Cell::CUIPart_Cell(const CUIPart_Cell& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Cell::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Cell::Initialize(void* pArg)
{
	UICell_DESC* Desc = static_cast<UICell_DESC*>(pArg);

	m_eType = Desc->eType;

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	if (m_eType == CELL_INVEN)
	{
		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 192.f / 255.f;
		m_fRGB[1] = 192.f / 255.f;
		m_fRGB[2] = 192.f / 255.f;
	}

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_Cell::Priority_Update(_float fTimeDelta)
{
	m_bPushed[0] = m_bPushed[1];
	m_bPushed[1] = false;
	m_bOnCursor = false;
	POINT mousePos{};

	GetCursorPos(&mousePos);
	ScreenToClient(g_hWnd, &mousePos);

	if (((m_fX - m_fSizeX) < mousePos.x) + ((m_fX + m_fSizeX) > mousePos.x) == 2)
		if (((m_fY - m_fSizeY) < mousePos.y) + ((m_fY + m_fSizeY) > mousePos.y) == 2)
		{
			m_bOnCursor = true;
			if (m_eType == CELL_INVEN)
				if (m_pGameInstance->Get_DIMouseState(DIMK_LBUTTON, false))
					m_bPushed[1] = true;
		}

	int a = 10;
}

void CUIPart_Cell::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Cell::Late_Update(_float fTimeDelta)
{
	if ((m_eType == CELL_INVEN) || (m_iItemIndex != -1))
	{
		m_fSizeX = INVEN_TEXTURE;
		m_fSizeY = INVEN_TEXTURE;
	}


	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_Cell::Render()
{
	if ((!m_eType == CELL_INVEN) || (m_iItemIndex == -1))
		__super::Render();
	else
		Inven_Render();

	return S_OK;
}

HRESULT CUIPart_Cell::Inven_Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CTexture* pTemp = GET_INSTANCE->GetItemInvenTexture(ITEMINDEX(m_iItemIndex));
	if (pTemp == nullptr)
		return E_FAIL;

	if (FAILED(pTemp->Bind_ShadeResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor, m_fRGB)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	// 수량 표시 
	if (m_iItemCount > -1)
	{
		m_pTransformCom->Set_Scaled(20.f, 15.f, 1.f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet((m_fX + 11.f) - m_fViewWidth * 0.5f,  - (m_fY + 11.f) + m_fViewHeight * 0.5f, 0.f, 1.f));

		if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom_Count, "g_WorldMatrix")))
			return E_FAIL;
		if (FAILED(m_pShaderCom_Count->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom_Count->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;

		if (FAILED(m_pTextureCom_Count->Bind_ShadeResource(m_pShaderCom_Count, "g_Texture", 0)))
			return E_FAIL;

		_float* m_fCount = new _float[3];
		m_fCount[0] = 0.f;
		m_fCount[1] = 0.f;
		m_fCount[2] = 0.f;

		if (FAILED(m_pShaderCom_Count->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor_CountNew, m_fCount)))
			return E_FAIL;

		if (FAILED(m_pShaderCom_Count->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		_tchar* tTemp = new _tchar[5];

		swprintf(tTemp, 5, L"%d", m_iItemCount);
		m_pGameInstance->Render_Text(TEXT("Font_Test3"), tTemp, { m_fX + 10.f,m_fY + 11.f,0.f,0.f }, 0.25f, true, { 1.f,1.f,1.f,1.f });

		Safe_Delete_Array(m_fCount);
		Safe_Delete_Array(tTemp);
	}

	// 신규 아이템 표시 
	if (m_bNew)
	{
		m_pTransformCom->Set_Scaled(20.f, 20.f, 1.f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet((m_fX - 11.f) - m_fViewWidth * 0.5f, -(m_fY - 11.f) + m_fViewHeight * 0.5f, 0.f, 1.f));

		if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom_New, "g_WorldMatrix")))
			return E_FAIL;
		if (FAILED(m_pShaderCom_New->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom_New->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;

		if (FAILED(m_pTextureCom_New->Bind_ShadeResource(m_pShaderCom_New, "g_Texture", 0)))
			return E_FAIL;

		_float* m_fCheck = new _float[3];
		m_fCheck[0] = 255.f/255.f;
		m_fCheck[1] = 0.f/ 255.f;
		m_fCheck[2] = 0.f/ 255.f;

		if (FAILED(m_pShaderCom_New->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor_CountNew, m_fCheck)))
			return E_FAIL;

		if (FAILED(m_pShaderCom_New->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		Safe_Delete_Array(m_fCheck);
	}
		

	//if (m_bPicked)
	//	m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("Picked"), _vector{ m_fX,m_fY,0.f,0.f }, 0.5f, true);

	return S_OK;
}

HRESULT CUIPart_Cell::Ready_Components()
{
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cell_InvenEmpty"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom)), 1))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Check"),
		TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_New)), 1))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Symbol_Back"),
		TEXT("Com_Texture_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Count)), 1))
		return E_FAIL;

	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader_1"), reinterpret_cast<CComponent**>(&m_pShaderCom_Count))))
		return E_FAIL;

	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader_2"), reinterpret_cast<CComponent**>(&m_pShaderCom_New))))
		return E_FAIL;
	

	return S_OK;
}

CUIPart_Cell* CUIPart_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Cell* pInstance = new CUIPart_Cell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Cell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPart_Cell::Clone(void* pArg)
{
	CUIPart_Cell* pInstance = new CUIPart_Cell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Cell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Cell::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom_New);
	Safe_Release(m_pTextureCom_Count);
	Safe_Release(m_pShaderCom_Count);
	Safe_Release(m_pShaderCom_New);

	
}
