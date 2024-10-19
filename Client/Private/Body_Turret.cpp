#include "stdafx.h"
#include "..\Public\Body_Turret.h"

#include "Player.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CBody_Turret::CBody_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody{ pDevice, pContext }
{
}

CBody_Turret::CBody_Turret(const CBody_Turret& Prototype)
	: CBody{ Prototype }
{
}

const _float4x4* CBody_Turret::Get_BoneMatrix_Ptr(const _char* pBoneName) const
{
	return m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(pBoneName);
}

HRESULT CBody_Turret::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Turret::Initialize(void* pArg)
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

void CBody_Turret::Priority_Update(_float fTimeDelta)
{
	int a = 10;

}

void CBody_Turret::Update(_float fTimeDelta)
{


	__super::Update(fTimeDelta);
}

void CBody_Turret::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */


	__super::Late_Update(fTimeDelta);

}

HRESULT CBody_Turret::Render()
{
	_float3 fPosition = { m_pParentMatrix->m[3][0], m_pParentMatrix->m[3][1], m_pParentMatrix->m[3][2] };

	if (!GET_INSTANCE->GetIsLender(fPosition))
		return S_OK;

	__super::Render();




	return S_OK;
}

HRESULT CBody_Turret::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Human"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Human"),
		TEXT("Com_Model_1"), reinterpret_cast<CComponent**>(&m_pModelCom_Body))))
		return E_FAIL;


	return S_OK;
}

CBody_Turret* CBody_Turret::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Turret* pInstance = new CBody_Turret(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Turret::Clone(void* pArg)
{
	CBody_Turret* pInstance = new CBody_Turret(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBody_Turret"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Turret::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom_Body);

}
