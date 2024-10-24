#include "stdafx.h"
#include "..\Public\UIPart_TextBox.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPart_TextBox::CUIPart_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_TextBox::CUIPart_TextBox(const CUIPart_TextBox& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_TextBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_TextBox::Initialize(void* pArg)
{
	UITextBox_DESC* Desc = static_cast<UITextBox_DESC*>(pArg);

	m_eType = Desc->eType;
	m_bCenter = Desc->bCenter;
	m_bAutoRemove = Desc->AutoRemove;
	m_pHost = Desc->pHost;

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
	m_fRGB[0] = 0.f / 255.f;
	m_fRGB[1] = 0.f / 255.f;
	m_fRGB[2] = 0.f / 255.f;


	m_bTransParent = true;
	m_fAlpah = 0.3f;

	if (m_eType == UITEXTBOX_TYPE::TEXTBOX_INTER_NAME)
		m_fSize = 0.5f;
	else if (m_eType == UITEXTBOX_TYPE::TEXTBOX_INTER_FUNCTION)
	{
		m_Font = TEXT("Font_Test3");
		m_fSize = 0.3f;
	}
	else if (m_eType == UITEXTBOX_TYPE::TEXTBOX_CASH)
	{
		m_Font = TEXT("Font_Test3");
		m_fSize = 0.5f;

		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 0 / 255.f;
		m_fRGB[1] = 0 / 255.f;
		m_fRGB[2] = 0 / 255.f;
	}
	else if (m_eType == UITEXTBOX_TYPE::TEXTBOX_SCRIPT)
	{
		m_Font = TEXT("Font_Test3");
		m_fSize = 0.3f;

		m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		m_fRGB[0] = 255.f / 255.f;
		m_fRGB[1] = 255.f / 255.f;
		m_fRGB[2] = 255.f / 255.f;
	}
	else 
		m_fSize = 0.7f;




	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUIPart_TextBox::Priority_Update(_float fTimeDelta)
{
	if ((m_eType == UITEXTBOX_TYPE::TEXTBOX_INTER_NAME) || (m_eType == UITEXTBOX_TYPE::TEXTBOX_INTER_FUNCTION))
		__super::SetOff(true);

	

	

	if (m_bAutoRemove)
	{
		if(m_fShowTime > 0.f)
			m_fShowTime -= fTimeDelta;

		if (m_fShowTime <= 0.f)
			__super::SetDead();
	}
	
	int a = 10;
}

void CUIPart_TextBox::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_TextBox::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

}

HRESULT CUIPart_TextBox::Render()
{
	if (m_eType != CUIPart_TextBox::TEXTBOX_CASH)
		__super::Render();

	if (m_eType == CUIPart_TextBox::TEXTBOX_SCRIPT)
	{
		if ((m_pHost == nullptr) || (m_pHost->GetDead()))
		{
			__super::SetDead();
			return S_OK;

			
		}

		_float3 fPosition{};
		_vector vPosition = m_pHost->GetTransform(CTransform::STATE_POSITION);
		XMStoreFloat3(&fPosition, vPosition);

		if (!GET_INSTANCE->GetIsLender(fPosition))
			return S_OK;

		if (!SetPositionByObject(XMLoadFloat4x4(&m_pHost->GetWorldMatrix())))
			return S_OK;
	}
	




	XMVECTOR vCenter = { m_fX,m_fY,0.f,0.f };

	if (!m_bCenter)
	{
		vCenter.m128_f32[0] -= m_fSizeX * 0.5f;
		vCenter.m128_f32[1] -= m_fSizeY * 0.5f;
	}



	if (m_eType == CUIPart_TextBox::TEXTBOX_CASH)
	{
		_tchar* tTemp_Origin = new _tchar[30];
		swprintf(tTemp_Origin, 30, L"%d", m_iCash);
		
		_int iCount = 0;

		for (_int i = 0; i < 30; ++i)
		{
			if (tTemp_Origin[i] == '\0')
			{
				iCount = i;
				break;
			}
		}

		_int iNow_Origin = 0;
		_int iNow_New = 0;
		_tchar* tTemp_New = new _tchar[30];

		while (iCount > iNow_Origin)
		{
			if ((iCount - iNow_Origin) % 3 == 0)
			{
				if (iNow_New > 0)
				{
					tTemp_New[iNow_New] = ',';
					++iNow_New;
				}
			}
			tTemp_New[iNow_New] = tTemp_Origin[iNow_Origin];
			++iNow_Origin;
			++iNow_New;
		}

		tTemp_New[iNow_New] = '\0';

		if (m_iCash < 0)
			m_fTextColor = { 1.f,0.f,0.f,1.f };
		else 
			m_fTextColor = { 0.f,0.f,0.f,1.f };

		m_pGameInstance->Render_Text(m_Font, tTemp_New, vCenter, m_fSize, m_bCenter, XMLoadFloat4(&m_fTextColor));
		Safe_Delete_Array(tTemp_Origin);
		Safe_Delete_Array(tTemp_New);

	}
	else
	{
		_tchar* strTemp = new _tchar[m_Text.size() + 1];

		for (_int i = 0; i <= m_Text.size(); ++i)
			strTemp[i] = m_Text[i];

		m_pGameInstance->Render_Text(m_Font, strTemp, vCenter, m_fSize, m_bCenter, XMLoadFloat4(&m_fTextColor));
		Safe_Delete_Array(strTemp);
	}


	

	return S_OK;
}



HRESULT CUIPart_TextBox::Ready_Components()
{


	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

CUIPart_TextBox* CUIPart_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_TextBox* pInstance = new CUIPart_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Message"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_TextBox::Clone(void* pArg)
{
	CUIPart_TextBox* pInstance = new CUIPart_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Message"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_TextBox::Free()
{
	__super::Free();

	Safe_Release(m_pHost);
}
