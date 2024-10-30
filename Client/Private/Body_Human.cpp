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

	m_iDefalut = STATE::STATE_IDEL;

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

	m_bLoop = true;


	if (*m_pParentState == STATE::STATE_EAT)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_GRANADE)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_GUN)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_HANDGUN)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_HI)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_THROW)
	{
		m_bLoop = false;
	}
	else if (*m_pParentState == STATE::STATE_HIT)
	{
		m_bLoop = false;
	}
	

	__super::Late_Update(fTimeDelta);

}

HRESULT CBody_Human::Render()
{
	
	
	
	_bool bResult = __super::Render();

	if (!bResult)
		return E_FAIL;

	

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

	m_vecTexture_HumanBody.resize(_int(HUMAN_BODY::BODY_END));
	m_vecTexture_HumanFace.resize(_int(HUMAN_FACE::FACE_END));

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Red"),TEXT("Com_Texture_0"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanBody[_int(HUMAN_BODY::BODY_RED)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Green"), TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanBody[_int(HUMAN_BODY::BODY_GREEN)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Blue"), TEXT("Com_Texture_2"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanBody[_int(HUMAN_BODY::BODY_BLUE)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Yellow"), TEXT("Com_Texture_3"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanBody[_int(HUMAN_BODY::BODY_YELLOW)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Brown"), TEXT("Com_Texture_4"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanBody[_int(HUMAN_BODY::BODY_BROWN)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Mad"), TEXT("Com_Texture_5"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_MAD)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Boring"), TEXT("Com_Texture_6"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_BORING)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Normal"), TEXT("Com_Texture_7"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_NORMAL)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Sleep"), TEXT("Com_Texture_8"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_SLEEP)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Psycho"), TEXT("Com_Texture_9"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_PSYCHO)])), 1))
		return E_FAIL;
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Sad"), TEXT("Com_Texture_10"), reinterpret_cast<CComponent**>(&m_vecTexture_HumanFace[_int(HUMAN_FACE::FACE_SAD)])), 1))
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

	for (auto& iter : m_vecTexture_HumanBody)
		Safe_Release(iter);
	for (auto& iter : m_vecTexture_HumanFace)
		Safe_Release(iter);

	m_vecTexture_HumanBody.clear();
	m_vecTexture_HumanFace.clear();

}
