#include "stdafx.h"
#include "Fire.h"
#include "ClientInstance.h"

CFire::CFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CFire::CFire(const CFire& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CFire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFire::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_fSpec_Extent = { 0.1f,0.1f,0.1f };
	m_fSpec_Scale = 0.1f;
	m_fSpec_PushedPower = GRAVITY_ACCELE * 2.f;
	m_fSpec_PushedPower_Decrease = 0.2f;
	m_iColliderType = _int(CCollider::TYPE_OBB);
	m_fSpec_Time = BURN_TIME;

	return S_OK;
}

void CFire::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);


}

void CFire::Update(_float fTimeDelta)
{
	for (auto& iter : m_Actionlist)
	{
		if (iter->fTime > 0.f)
		{
			iter->fTime -= fTimeDelta;
			if (iter->fTime < 0.f)
			{
				iter->bDead = true;
				continue;
			}
		}

		if (iter->pHost == nullptr)
		{
			if (GET_INSTANCE->Check_Terrain_Collision(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
			{
				iter->pTransform->Set_Pushed_Power({ 0.f,0.f,0.f }, 0.f);
			}

			CPhysicsManager::P_RESULT tResult = {};

			tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, true, true, false, fTimeDelta);
			GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);
		}
		else if ((iter->pHost->GetDead()) || (!iter->pHost->Get_CC(CROWDCONTROL::CC_BURN)))
		{
			iter->bDead = true;
			continue;
		}
		else if (!iter->pHost->GetDead())
		{

			iter->pTransform->Set_State(CTransform::STATE_POSITION, iter->pHost->GetTransform(CTransform::STATE_POSITION));
			
		}

			if (m_fMakeEffect >= 0.1f)
			{
				for (_int i = 0; i < 3; ++i)
					GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FIRE, iter->pCollider->GetBoundingCenter());
			}
		
			if (iter->iActCount == 0)
				GET_INSTANCE->PlaySound(SOUND_NAME::SOUND_FIRE, SOUND_CHANNEL::CH_INGAME_FIRE, 10.f, iter->pCollider->GetBoundingCenter());
			else
				GET_INSTANCE->SetChannelVolume(SOUND_NAME::SOUND_FIRE, SOUND_CHANNEL::CH_INGAME_FIRE, 10.f, iter->pCollider->GetBoundingCenter());

			++iter->iActCount;
	}

	m_fMakeEffect -= fTimeDelta;
	if (m_fMakeEffect < 0.f)
		m_fMakeEffect = 0.1f;


}

void CFire::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
		iter->pTransform->Save_BeforePosition();




	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CFire::Render()
{
	__super::Render();

	for (auto& iter : m_Actionlist)
	{
		_float3 fPosition{};
		XMStoreFloat3(&fPosition, iter->pTransform->Get_State(CTransform::STATE_POSITION));

		if (!GET_INSTANCE->GetIsLender(fPosition))
			continue;


		
#ifdef _DEBUG
		iter->pCollider->Render();
#endif
	}

	return S_OK;
}
void CFire::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CFire::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CFire::Ready_Components()
{

	
	return S_OK;
}

HRESULT CFire::Ready_PartObjects()
{
	return S_OK;
}

CFire* CFire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFire* pInstance = new CFire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFire::Clone(void* pArg)
{
	CFire* pInstance = new CFire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFire"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire::Free()
{
	__super::Free();

}
