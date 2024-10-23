#include "stdafx.h"
#include "Bullet_MachineGun.h"
#include "ClientInstance.h"

CBullet_MachineGun::CBullet_MachineGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMissile{ pDevice, pContext }
{
}

CBullet_MachineGun::CBullet_MachineGun(const CBullet_MachineGun& Prototype)
	: CMissile{ Prototype }
{
}

HRESULT CBullet_MachineGun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_MachineGun::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);
	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	m_pTransformCom->Set_Pushed_PowerDecrease(0.f); // <- 속도 감소 없음 
	m_pTransformCom->Set_RotationPerSec(100.f);
	m_fSpec_Extent = { 0.2f,0.2f,0.2f };
	m_fSpec_Scale = 0.1f;
	m_fSpec_PushedPower = 65.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = _int(CCollider::TYPE_SPHERE);
	return S_OK;
}

void CBullet_MachineGun::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBullet_MachineGun::Update(_float fTimeDelta)
{

	for (auto& iter : m_Actionlist)
	{
		CPhysicsManager::P_RESULT tResult = {};

		if (iter->iActCount == 0)
		{
			_vector vLook = iter->pTransform->Get_State(CTransform::STATE_RIGHT);
			_vector vDirec = XMLoadFloat3(&iter->pTransform->Get_Pushed_Dir());

			_float fDot = XMVector3Dot({ vDirec.m128_f32[0], 0.f, vDirec.m128_f32[2], 0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2], 0.f }).m128_f32[0];
			_float fCos = (sqrt(pow(vDirec.m128_f32[0], 2) + pow(vDirec.m128_f32[2], 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));
			_float m_fMove_Angle_Y = acos(fDot / fCos);

			if (isnan(m_fMove_Angle_Y))
				m_fMove_Angle_Y = 0.f;

			_bool bResult = GET_INSTANCE->Check_CCW_XZ({ vDirec.m128_f32[0], 0.f, vDirec.m128_f32[2] }, { 0.f,0.f,0.f }, { vLook.m128_f32[0], 0.f, vLook.m128_f32[2] });

			if (bResult)
				m_fMove_Angle_Y *= -1;

			 fDot = XMVector3Dot({ vDirec.m128_f32[0], vDirec.m128_f32[1], 0.f, 0.f }, { vLook.m128_f32[0],vLook.m128_f32[1], 0.f,  0.f }).m128_f32[0];
			 fCos = (sqrt(pow(vDirec.m128_f32[0], 2) + pow(vDirec.m128_f32[1], 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[1], 2)));
			 _float m_fMove_Angle_Z = acos(fDot / fCos);

			if (isnan(m_fMove_Angle_Z))
				m_fMove_Angle_Z = 0.f;

			bResult = GET_INSTANCE->Check_CCW_XZ({ vDirec.m128_f32[0], vDirec.m128_f32[1], 0.f }, { 0.f,0.f,0.f }, { vLook.m128_f32[0], vLook.m128_f32[1], 0.f });

			if (bResult)
				m_fMove_Angle_Z *= -1;

			iter->pTransform->Rotation(0.f, m_fMove_Angle_Y, 0.f);
			++iter->iActCount;
		}
			

		tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, false, false, false, fTimeDelta);
		GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

		//GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_TRAIL, iter->pCollider->GetBoundingCenter());
		
		if (GET_INSTANCE->Check_Terrain_Collision(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
		{
			for (_int i = 0; i < 10; ++i)
				GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_DEBRIS, iter->pCollider->GetBoundingCenter());
			iter->bDead = true;
		}
	}
}

void CBullet_MachineGun::Late_Update(_float fTimeDelta)
{
	for (list<INTER_INFO*>::iterator iter = m_Actionlist.begin(); iter != m_Actionlist.end();)
	{
		if (*iter == nullptr)
			iter = m_Actionlist.erase(iter);
		else
			++iter;
	}

	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
		iter->pTransform->Save_BeforePosition();




	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBullet_MachineGun::Render()
{
	__super::Render();

	for (auto& iter : m_Actionlist)
	{
		_float3 fPosition{};
		XMStoreFloat3(&fPosition, iter->pTransform->Get_State(CTransform::STATE_POSITION));

		if (!GET_INSTANCE->GetIsLender(fPosition))
			continue;


		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &iter->pTransform->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;

		_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
				return E_FAIL;*/

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
#ifdef _DEBUG
		iter->pCollider->Render();
#endif
	}

	return S_OK;
}
void CBullet_MachineGun::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}
void CBullet_MachineGun::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);

}


HRESULT CBullet_MachineGun::Ready_Components()
{


	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bullet_Normal"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CBullet_MachineGun::Ready_PartObjects()
{
	return S_OK;
}

CBullet_MachineGun* CBullet_MachineGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_MachineGun* pInstance = new CBullet_MachineGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBullet_MachineGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_MachineGun::Clone(void* pArg)
{
	CBullet_MachineGun* pInstance = new CBullet_MachineGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBullet_MachineGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_MachineGun::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
