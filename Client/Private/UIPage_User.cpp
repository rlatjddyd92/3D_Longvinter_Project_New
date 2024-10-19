#include "stdafx.h"
#include "..\Public\UIPage_User.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_User::CUIPage_User(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_User::CUIPage_User(const CUIPage_User& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_User::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_User::Initialize(void* pArg)
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_UIPart();

	m_fSizeX = 30.f;
	m_fSizeY = 30.f;

	return S_OK;
}

void CUIPage_User::Priority_Update(_float fTimeDelta)
{



	int a = 10;
}

void CUIPage_User::Update(_float fTimeDelta)
{

	int a = 10;
}

void CUIPage_User::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_User::Render()
{
	if (GET_INSTANCE->GetCameraMode() != CAMERAMODE::CAMERA_FIRST)
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	ITEMINDEX eNow = GET_INSTANCE->GetPickedItemIndex();

	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

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

void CUIPage_User::AddRender_UIPage()
{
	if (!m_vecPart[_int(PART_INTER_NAME)]->GetOff())
		m_vecPart[_int(PART_INTER_NAME)]->AddRender_UIPart();
	if (!m_vecPart[_int(PART_INTER_E)]->GetOff())
		m_vecPart[_int(PART_INTER_E)]->AddRender_UIPart();
	if (!m_vecPart[_int(PART_INTER_F)]->GetOff())
		m_vecPart[_int(PART_INTER_F)]->AddRender_UIPart();
	if (!m_vecPart[_int(PART_INTER_C)]->GetOff())
		m_vecPart[_int(PART_INTER_C)]->AddRender_UIPart();



	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPage_User::Ready_UIPart()
{
	m_vecPart.resize(_int(PART_END));






	m_vecPart[_int(PART_INTER_NAME)] = static_cast<CUIPart*>(GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_INTER_NAME,0.f,0.f,120.f,15.f,false,false));
	m_vecPart[_int(PART_INTER_E)] = static_cast<CUIPart*>(GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_INTER_FUNCTION, 0.f, 0.f, 100.f, 12.f, false, false));
	m_vecPart[_int(PART_INTER_F)] = static_cast<CUIPart*>(GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_INTER_FUNCTION, 0.f, 0.f, 100.f, 12.f, false, false));
	m_vecPart[_int(PART_INTER_C)] = static_cast<CUIPart*>(GET_INSTANCE->MakeUIPart_TextBox(CUIPart_TextBox::TEXTBOX_INTER_FUNCTION, 0.f, 0.f, 100.f, 12.f, false, false));



}

_bool CUIPage_User::Key_Action()
{
	_int Check = 0;

	for (auto& iter : m_vecPart)
		Check += iter->IsPushed();

	if (Check == 0)
		return false;



	return true;
}

void CUIPage_User::Show_Interaction_Function(_matrix mHost, wstring InterName, wstring Function_E, wstring Function_F, wstring Function_C)
{
	_vector vAdjust = { 0.f,1.f,0.f,0.f };

	mHost.r[3] += vAdjust;



	m_vecPart[_int(PART_INTER_NAME)]->SetOn();
	m_vecPart[_int(PART_INTER_NAME)]->SetPositionByObject(mHost);
	m_vecPart[_int(PART_INTER_NAME)]->Move_UI(200.f, -90.f);
	m_vecPart[_int(PART_INTER_NAME)]->AddRender_UIPart();
	static_cast<CUIPart_TextBox*>(m_vecPart[_int(PART_INTER_NAME)])->SetText(InterName);

	if (Function_E == TEXT("없음"))
		return;

	m_vecPart[_int(PART_INTER_E)]->SetOn();
	m_vecPart[_int(PART_INTER_E)]->SetPositionByObject(mHost);
	m_vecPart[_int(PART_INTER_E)]->Move_UI(190.f, -60.f);
	m_vecPart[_int(PART_INTER_E)]->AddRender_UIPart();
	static_cast<CUIPart_TextBox*>(m_vecPart[_int(PART_INTER_E)])->SetText(Function_E);

	if (Function_F == TEXT("없음"))
		return;

	m_vecPart[_int(PART_INTER_F)]->SetOn();
	m_vecPart[_int(PART_INTER_F)]->SetPositionByObject(mHost);
	m_vecPart[_int(PART_INTER_F)]->Move_UI(190.f, -40.f);
	m_vecPart[_int(PART_INTER_F)]->AddRender_UIPart();
	static_cast<CUIPart_TextBox*>(m_vecPart[_int(PART_INTER_F)])->SetText(Function_F);


	if (Function_C == TEXT("없음"))
		return;

	m_vecPart[_int(PART_INTER_C)]->SetOn();
	m_vecPart[_int(PART_INTER_C)]->SetPositionByObject(mHost);
	m_vecPart[_int(PART_INTER_C)]->Move_UI(190.f, -20.f);
	m_vecPart[_int(PART_INTER_C)]->AddRender_UIPart();
	static_cast<CUIPart_TextBox*>(m_vecPart[_int(PART_INTER_C)])->SetText(Function_C);

}



HRESULT CUIPage_User::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Crosshair"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CUIPage_User* CUIPage_User::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_User* pInstance = new CUIPage_User(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_User"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_User::Clone(void* pArg)
{
	CUIPage_User* pInstance = new CUIPage_User(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_User"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_User::Free()
{
	__super::Free();



	for (auto& iter : m_vecPart)
		Safe_Release(iter);

	m_vecPart.clear();
}
