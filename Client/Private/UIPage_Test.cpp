#include "stdafx.h"
#include "..\Public\UIPage_Test.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Test::CUIPage_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Test::CUIPage_Test(const CUIPage_Test& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Test::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Test::Initialize(void* pArg)
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fX = 400.f;
	m_fY = 300.f;

	m_fSizeX = 300.f;
	m_fSizeY = 300.f;

	__super::SetOff(true);

	Ready_UIPart();

	return S_OK;
}

void CUIPage_Test::Priority_Update(_float fTimeDelta)
{



	int a = 10;
}

void CUIPage_Test::Update(_float fTimeDelta)
{
	if (m_pButton_Close->IsPushed())
		__super::SetOff(true);
	int a = 10;
}

void CUIPage_Test::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIPage_Test::Render()
{

	return S_OK;
}

void CUIPage_Test::AddRender_UIPage()
{
	m_pButton_Close->AddRender_UIPart();
	m_pBack_Window_Header->AddRender_UIPart();
	m_pBack_Window->AddRender_UIPart();
}

void CUIPage_Test::Ready_UIPart()
{
	m_pButton_Close = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_CLOSE, m_fX + (m_fSizeY / 2) - 25.f, m_fY - (m_fSizeY / 2) + 25.f, 20.f, 20.f);
	m_pBack_Window = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW, m_fX, m_fY, m_fSizeX, m_fSizeY);
	m_pBack_Window_Header = GET_INSTANCE->MakeUIPart_Back(CUIPart_Back::BACK_INGAME_WINDOW_HEADER, m_fX, m_fY - (m_fSizeY / 2) + 25.f, m_fSizeX - 20.f, 30.f);

}

_bool CUIPage_Test::Check_Cursor()
{
	return _bool();
}

_bool CUIPage_Test::Check_Cursor()
{
	return _bool();
}

HRESULT CUIPage_Test::Ready_Components()
{


	return S_OK;
}

CUIPage_Test* CUIPage_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Test* pInstance = new CUIPage_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Test::Clone(void* pArg)
{
	CUIPage_Test* pInstance = new CUIPage_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Test::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
