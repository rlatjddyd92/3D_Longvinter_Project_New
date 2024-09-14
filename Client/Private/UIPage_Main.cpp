#include "stdafx.h"
#include "..\Public\UIPage_Main.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPage_Main::CUIPage_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{
}

CUIPage_Main::CUIPage_Main(const CUIPage_Main& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Main::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage_Main::Initialize(void* pArg)
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

	

	return S_OK;
}

void CUIPage_Main::Priority_Update(_float fTimeDelta)
{
	

	int a = 10;
}

void CUIPage_Main::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPage_Main::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);
	if (GET_INSTANCE->GetNowLevel() != LEVELID::LEVEL_LOGO)
		__super::SetOff(true);
	else 
		m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIPage_Main::Render()
{
	_vector vTemp = { 0.f,0.f,0.f,0.f };

	vTemp.m128_f32[0] = (g_iWinSizeX >> 1);
	vTemp.m128_f32[1] = _float(g_iWinSizeY) * _float(2.f / 3.f);

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("Game Start"), vTemp, 1.f, true);

	vTemp.m128_f32[0] = (g_iWinSizeX >> 1);
	vTemp.m128_f32[1] = _float(g_iWinSizeY) * _float(2.f / 3.f) + 60.f;

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), TEXT("Map Editor"), vTemp, 1.f, true);

	return S_OK;
}

void CUIPage_Main::AddRender_UIPage()
{
	
	m_pPicture_BGA->AddRender_UIPart();
	m_pPicture_MainLogo->AddRender_UIPart();
	m_pButton_Ingame->AddRender_UIPart();
	m_pButton_Editor->AddRender_UIPart();
	

	

}

void CUIPage_Main::Ready_UIPart()
{
	m_pPicture_MainLogo = GET_INSTANCE->MakeUIPart_Picture(CUIPart_Picture::PICTUER_MAINLOGO, g_iWinSizeX >> 1, _float(g_iWinSizeY) * _float(1.f / 3.f), 400.f, 110.f);
	m_pButton_Ingame = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_INGAME, g_iWinSizeX >> 1, _float(g_iWinSizeY) * _float(2.f / 3.f), 250.f, 50.f);
	m_pButton_Editor = GET_INSTANCE->MakeUIPart_Button(CUIPart_Button::BUTTON_EDITOR, g_iWinSizeX >> 1, _float(g_iWinSizeY) * _float(2.f / 3.f) + 60.f, 250.f, 50.f);
	m_pPicture_BGA = GET_INSTANCE->MakeUIPart_Picture(CUIPart_Picture::PICTUER_BGA, g_iWinSizeX >> 1, g_iWinSizeY >> 1, g_iWinSizeX, g_iWinSizeY);
}

HRESULT CUIPage_Main::Ready_Components()
{
	

	return S_OK;
}

CUIPage_Main* CUIPage_Main::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Main* pInstance = new CUIPage_Main(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Main"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPage_Main::Clone(void* pArg)
{
	CUIPage_Main* pInstance = new CUIPage_Main(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Main"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Main::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
