#include "stdafx.h"
#include "AI_NPC.h"
#include "ClientInstance.h"

CAI_NPC::CAI_NPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainer_NPC{ pDevice, pContext }
{
}

CAI_NPC::CAI_NPC(const CAI_NPC& Prototype)
	: CContainer_NPC{ Prototype }
{
}

HRESULT CAI_NPC::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CAI_NPC::Initialize(void* pArg)
{
	AI_NPC_Desc* pTemp = static_cast<AI_NPC_Desc*>(pArg);

	eContainerType = CONTAINER::CONTAINER_NPC;
	m_eNPC_Type = pTemp->eType;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	m_iBody = _int(HUMAN_BODY::BODY_YELLOW);

	GET_INSTANCE->MakeEnemyHpBar(this);
	GET_INSTANCE->MakeSymbol(this);

	m_fDetective_Length = 20.f;


	return S_OK;
}

void CAI_NPC::Priority_Update(_float fTimeDelta)
{



	__super::Priority_Update(fTimeDelta);

	

	Set_AI_Status(fTimeDelta);
}

void CAI_NPC::Update(_float fTimeDelta)
{

	if (m_fDistance_from_Player < m_fInterActionRange)
	{
		if (m_eNPC_Type == NPC_TYPE::NPC_SHOP)
		{
			_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix()), TEXT("상인"), TEXT("E키 : 대화하기"), TEXT("F키 : 사기"), TEXT("C키 : 팔기"));

			if (bActive)
			{
				if (m_pGameInstance->Get_DIKeyState(DIK_E))
					GET_INSTANCE->ShowInformMessage(TEXT("[TEST] 대화 기능 작동"));
				else if (m_pGameInstance->Get_DIKeyState(DIK_F))
					GET_INSTANCE->ShowInformMessage(TEXT("[TEST] 구매 기능 작동"));
				else if (m_pGameInstance->Get_DIKeyState(DIK_C))
					GET_INSTANCE->ShowInformMessage(TEXT("[TEST] 판매 기능 작동"));
			}

		}
	}

	







	if (m_eAI_Status == AI_STATUS::AI_DEAD)
	{
		m_pTransformCom->Turn({ -1.f,0.f,0.f }, fTimeDelta * m_fMove_Angle);


		_float3 fExtent = m_pColliderCom->GetBoundingExtents();

		fExtent.y -= ((fTimeDelta * m_fMove_Angle / _float(PI_DEFINE)) * fExtent.y);

		if (fExtent.y < 0.5f)
			fExtent.y = 0.5f;

		m_pColliderCom->SetBoundingExtents({ fExtent.x, fExtent.y - ((fTimeDelta * m_fMove_Angle / _float(PI_DEFINE)) * fExtent.y), fExtent.z });


		m_fMove_Angle -= fTimeDelta * m_fMove_Angle;
	}
	else
		Moving_Control(fTimeDelta);




	__super::Update(fTimeDelta);

}

void CAI_NPC::Late_Update(_float fTimeDelta)
{



	__super::Late_Update(fTimeDelta);
}

HRESULT CAI_NPC::Render()
{
	__super::Render();


	return S_OK;
}

void CAI_NPC::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent)
{
	if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	__super::Collision_Reaction_InterAction(pPoint, eIndex, tOpponent);











}

void CAI_NPC::Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex)
{
	if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	__super::Collision_Reaction_MadeInterAction(pPoint, eIndex);
}

void CAI_NPC::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
	if (m_eAI_Status == AI_STATUS::AI_DEAD) return;

	__super::Collision_Reaction_Container(pPoint, eIndex);






}

void CAI_NPC::DeadAction()
{
	__super::DeadAction();

	if (m_eAI_Status == AI_STATUS::AI_DEAD)
		return;

	m_eAI_Status = AI_STATUS::AI_DEAD;
	m_fActionTimer = 3.f;
	m_fMove_Angle = PI_DEFINE * 0.15f;
	m_iState = STATE_IDEL;
}



void CAI_NPC::Moving_Control(_float fTimeDelta)
{
	__super::Moving_Control(fTimeDelta);

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fMove_Angle);

	
}

void CAI_NPC::Weapon_Control(_float fTimeDelta)
{
	__super::Weapon_Control(fTimeDelta);


}

void CAI_NPC::Camera_Control(_float fTimeDelta)
{
	__super::Camera_Control(fTimeDelta);
}

void CAI_NPC::Test_Control(_float fTimeDelta)
{
	__super::Test_Control(fTimeDelta);
}

void CAI_NPC::Set_AI_Status(_float fTimeDelta)
{
	__super::Set_AI_Status(fTimeDelta);



	if (m_eAI_Status == AI_STATUS::AI_DEAD)
		return;

	m_fMove_Angle = 0.f;

	

	__super::Get_Sound(&m_fSoundPosition, &m_fVolume, &m_fMove_Angle);

	_float3 fPlayerPosition{};
	_bool bCanSee = false;

	__super::Look_Player(&fPlayerPosition, &bCanSee, &m_fMove_Angle);



}


HRESULT CAI_NPC::Ready_Components()
{

	return S_OK;
}

HRESULT CAI_NPC::Ready_PartObjects()
{

	return S_OK;
}

CAI_NPC* CAI_NPC::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAI_NPC* pInstance = new CAI_NPC(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAI_NPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAI_NPC::Clone(void* pArg)
{
	CAI_NPC* pInstance = new CAI_NPC(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CAI_NPC"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_NPC::Free()
{
	__super::Free();
}
