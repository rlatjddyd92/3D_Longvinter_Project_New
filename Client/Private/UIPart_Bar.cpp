#include "stdafx.h"
#include "..\Public\UIPart_Bar.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CUIPart_Bar::CUIPart_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPart{ pDevice, pContext }
{
}

CUIPart_Bar::CUIPart_Bar(const CUIPart_Bar& Prototype)
	: CUIPart{ Prototype }
{
}

HRESULT CUIPart_Bar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPart_Bar::Initialize(void* pArg)
{
	UIBar_DESC* Desc = static_cast<UIBar_DESC*>(pArg);

	m_eType = Desc->eType;
	m_pHost = Desc->pHost;

	Safe_AddRef(m_pHost);

	Desc->fSpeedPerSec = 10.f;
	Desc->fRotationPerSec = XMConvertToRadians(90.0f);

	
	m_fAdjust = { 0.f, 2.f, 0.f };

	

	

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (m_eType == BAR_ENEMY_HP)
	{
		m_fSizeX = 50.f;
		m_fSizeY = 5.f;
		m_fFill_X = Desc->fX;
		m_fFill_SizeX = m_fSizeX;
	}


	return S_OK;
}

void CUIPart_Bar::Priority_Update(_float fTimeDelta)
{
	

	int a = 10;
}

void CUIPart_Bar::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIPart_Bar::Late_Update(_float fTimeDelta)
{
	if (m_eType == BAR_ENEMY_HP)
	{
		
	}
	else 
		__super::Late_Update(fTimeDelta);
	


	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	
}

HRESULT CUIPart_Bar::Render()
{
	if (m_eType == BAR_ENEMY_HP)
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
		
		m_fFill_X = m_fX;
		m_fFill_SizeX = m_fSizeX;

		m_fFill_SizeX = (m_pHost->Get_Hp() / m_pHost->Get_HpMax()) * m_fSizeX;
		m_fFill_X = m_fX - (m_fSizeX * 0.5f) + (m_fFill_SizeX * 0.5f);


		for (_int i = 0; i < 2; ++i)
		{
			if (i == 0)
			{
				m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
				m_fRGB[0] = 0.f / 255.f;
				m_fRGB[1] = 0.f / 255.f;
				m_fRGB[2] = 0.f / 255.f;

				m_bTransParent = true;
				m_fAlpah = 0.3f;

				m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_fX - m_fViewWidth * 0.5f, -m_fY + m_fViewHeight * 0.5f, 0.f, 1.f));
			}
			else
			{
				m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
				m_fRGB[0] = 204.f / 255.f;
				m_fRGB[1] = 000.f / 255.f;
				m_fRGB[2] = 000.f / 255.f;

				m_bTransParent = false;


				m_pTransformCom->Set_Scaled(m_fFill_SizeX, m_fSizeY, 1.f);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_fFill_X - m_fViewWidth * 0.5f, -m_fY + m_fViewHeight * 0.5f, 0.f, 1.f));
			}

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

			if (FAILED(m_pShaderCom->Bind_ChangeAlpah("g_Istransparency", "g_TransAlpah", &m_bTransParent, &m_fAlpah)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Bind_Buffers()))
				return E_FAIL;
			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;
		}	
	}
	else 
		__super::Render();

	return S_OK;
}



HRESULT CUIPart_Bar::Ready_Components()
{

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

CUIPart_Bar* CUIPart_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPart_Bar* pInstance = new CUIPart_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPart_Bar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIPart_Bar::Clone(void* pArg)
{
	CUIPart_Bar* pInstance = new CUIPart_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPart_Bar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPart_Bar::Free()
{
	__super::Free();


	Safe_Release(m_pHost);
}
