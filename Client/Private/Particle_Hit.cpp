#include "stdafx.h"
#include "Particle_Hit.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

CParticle_Hit::CParticle_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect_Particle{ pDevice, pContext }
{
}

CParticle_Hit::CParticle_Hit(const CParticle_Hit& Prototype)
	: CEffect_Particle{ Prototype }
{
}

HRESULT CParticle_Hit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Hit::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_bSpec_TerrainDelete = true;
	m_tSpec.fTime = 0.1f;
	m_tSpec.fFly = 0.f;
	m_tSpec.fScale = 0.2f;
	m_tSpec.fDirec = { 0.f,0.2f,0.f };
	m_tSpec.fColor = { 255.f / 255.f,0.f / 255.f,0.f / 255.f,1.f };
	m_tSpec.fPowerDecrease = 0.0f;
	m_tSpec.fPower = 0.3f;

	m_pTransformCom->Set_Scaled(0.2f, 0.2f, 0.2f);

	m_bChangeColor[0] = true;
	m_bChangeColor[1] = true;
	m_bChangeColor[2] = true;

	m_bRandDirec[0] = true;
	m_bRandDirec[1] = true;
	m_bRandDirec[2] = true;

	return S_OK;
}

void CParticle_Hit::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);


	for (auto& iter : m_EffectInfolist)
	{
		//if (_float(iter->fTime / m_tSpec.fTime) > 0.95f) // 흰색(에 가까운 색)
		//	iter->fColor = { 255.f / 255.f, 250.f / 255.f , 240.f / 255.f , 255.f / 255.f };
		//else if (_float(iter->fTime / m_tSpec.fTime) > 0.85f) // 노란색
		//	iter->fColor = { 255.f / 255.f, 215.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		//else if (_float(iter->fTime / m_tSpec.fTime) > 0.75f) // 주황색
		//	iter->fColor = { 255.f / 255.f, 165.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		//else if (_float(iter->fTime / m_tSpec.fTime) > 0.65f) // 붉은색
		//	iter->fColor = { 255.f / 255.f, 69.f / 255.f , 0.f / 255.f , 255.f / 255.f };
		//else if (_float(iter->fTime / m_tSpec.fTime) > 0.55f) // 짙은 회색 
		//	iter->fColor = { 70.f / 255.f, 70.f / 255.f , 70.f / 255.f , 255.f / 255.f };
		//else //if (_float(iter->fTime / m_tSpec.fTime) > 0.3f) // 회색
		//	iter->fColor = { 105.f / 255.f, 105.f / 255.f , 105.f / 255.f , 255.f / 255.f };
		////else  // 연한 회색
		////	iter->fColor = { 211.f / 255.f, 211.f / 255.f , 211.f / 255.f , 255.f / 255.f };

	}

}

void CParticle_Hit::Update(_float fTimeDelta)
{





	__super::Update(fTimeDelta);





}

void CParticle_Hit::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CParticle_Hit::Render()
{
	//__super::Render();

	for (auto& iter : m_EffectInfolist)
	{
		_float fScale = _float(iter->fTime / m_tSpec.fTime) * m_tSpec.fScale;

		_float4x4 fMat{};
		XMStoreFloat4x4(&fMat, XMMatrixIdentity());

		m_pTransformCom->Set_WorldMatrix(fMat);

		m_pTransformCom->Set_Scaled(fScale, fScale, fScale);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&iter->fPosition));

		_float fAngle = __super::BillBoard(m_pTransformCom);

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

HRESULT CParticle_Hit::Bind_WorldMatrix(CShader* pShader, const _char* pContantName)
{
	return pShader->Bind_Matrix(pContantName, &m_WorldMatrix);
}

HRESULT CParticle_Hit::Ready_Components()
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

HRESULT CParticle_Hit::Ready_PartObjects()
{
	return S_OK;
}

CParticle_Hit* CParticle_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Hit* pInstance = new CParticle_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CParticle_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Hit::Clone(void* pArg)
{
	CParticle_Hit* pInstance = new CParticle_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CParticle_Hit"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Hit::Free()
{
	__super::Free();

}
