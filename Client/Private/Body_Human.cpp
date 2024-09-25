#include "stdafx.h"
#include "..\Public\Body_Human.h"

#include "Player.h"

#include "GameInstance.h"

CBody_Human::CBody_Human(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody{ pDevice, pContext }
{
}

CBody_Human::CBody_Human(const CBody_Human& Prototype)
	: CBody{ Prototype }
{
}

const _float4x4* CBody_Human::Get_BoneMatrix_Ptr(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(pBoneName);
}

HRESULT CBody_Human::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Human::Initialize(void* pArg)
{
	BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
	m_pParentState = pDesc->pParentState;
	m_iBeforeState = *m_pParentState;

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBody_Human::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CBody_Human::Update(_float fTimeDelta)
{
	

	__super::Update(fTimeDelta);
}

void CBody_Human::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

	
}

HRESULT CBody_Human::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBody_Human::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxAnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Human"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(0, true);
	return S_OK;
}

CBody_Human* CBody_Human::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Human* pInstance = new CBody_Human(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_Human"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Human::Clone(void* pArg)
{
	CBody_Human* pInstance = new CBody_Human(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBody_Human"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Human::Free()
{
	__super::Free();
}
