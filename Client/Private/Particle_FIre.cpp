#include "stdafx.h"
#include "Particle_Fire.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

CParticle_Fire::CParticle_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect_Particle{ pDevice, pContext }
{
}

CParticle_Fire::CParticle_Fire(const CParticle_Fire& Prototype)
	: CEffect_Particle{ Prototype }
{
}

HRESULT CParticle_Fire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Fire::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_bSpec_TerrainDelete = true;
	m_tSpec.fTime = 3.f;
	m_tSpec.fFly = 0.02f;
	m_tSpec.fDirec = { 0.f,0.2f,0.f };
	m_tSpec.fColor = { 0.f,0.f,0.f,1.f };
	m_tSpec.fPowerDecrease = 0.01f;
	m_tSpec.fPower = 0.1f;

	m_pTransformCom->Set_Scaled(0.1f, 0.1f, 0.1f);

	m_bChangeColor[0] = true;
	m_bChangeColor[1] = true;
	m_bChangeColor[2] = true;

	m_bRandDirec[0] = true;
	m_bRandDirec[2] = true;

	return S_OK;
}

void CParticle_Fire::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);


	for (auto& iter : m_EffectInfolist)
	{
		if (_float(iter->fTime / m_tSpec.fTime) > 0.8f) // 흰색(에 가까운 색)
			iter->fColor = { 255.f / 255.f, 250.f / 255.f , 240.f / 255.f , 255.f / 255.f };
		else if (_float(iter->fTime / m_tSpec.fTime) > 0.7f) // 노란색
			iter->fColor = { 255.f / 255.f, 215.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		else if (_float(iter->fTime / m_tSpec.fTime) > 0.6f) // 주황색
			iter->fColor = { 255.f / 255.f, 165.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		else if (_float(iter->fTime / m_tSpec.fTime) > 0.5f) // 붉은색
			iter->fColor = { 255.f / 255.f, 69.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		else if (_float(iter->fTime / m_tSpec.fTime) > 0.4f) // 짙은 회색 
			iter->fColor = { 50.f / 255.f, 50.f / 255.f , 50.f / 255.f , 255.f / 255.f };
		else //if (_float(iter->fTime / m_tSpec.fTime) > 0.3f) // 회색
			iter->fColor = { 105.f / 255.f, 105.f / 255.f , 105.f / 255.f , 255.f / 255.f };
		//else  // 연한 회색
		//	iter->fColor = { 211.f / 255.f, 211.f / 255.f , 211.f / 255.f , 255.f / 255.f };

	}

}

void CParticle_Fire::Update(_float fTimeDelta)
{





	__super::Update(fTimeDelta);
}

void CParticle_Fire::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CParticle_Fire::Render()
{
	//__super::Render();
	for (auto& iter : m_EffectInfolist)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&iter->fPosition));

		_float fRGB[3] = { iter->fColor.x, iter->fColor.y, iter->fColor.z };

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_ChangeColor("g_IsChange", "g_ChangeColor", m_bChangeColor, fRGB)))
			return E_FAIL;

		/*if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}




	return S_OK;
}

HRESULT CParticle_Fire::Bind_WorldMatrix(CShader* pShader, const _char* pContantName)
{
	return pShader->Bind_Matrix(pContantName, &m_WorldMatrix);
}

HRESULT CParticle_Fire::Ready_Components()
{
	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxFireSmoke"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;



	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect3D"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CParticle_Fire::Ready_PartObjects()
{
	return S_OK;
}

CParticle_Fire* CParticle_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Fire* pInstance = new CParticle_Fire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CParticle_Fire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Fire::Clone(void* pArg)
{
	CParticle_Fire* pInstance = new CParticle_Fire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CParticle_Fire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Fire::Free()
{
	__super::Free();

}
