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

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
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
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
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
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

void CUIPage_User::Ready_UIPart()
{
	m_vecPart.resize(_int(PART_END));

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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& iter : m_vecPart)
		Safe_Release(iter);

	m_vecPart.clear();
}
