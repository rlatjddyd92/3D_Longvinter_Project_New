#include "stdafx.h"
#include "LandObject_NonAnim.h"
#include "ClientInstance.h"
#include "Container_Turret.h"

CLandObject_NonAnim::CLandObject_NonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{
}

CLandObject_NonAnim::CLandObject_NonAnim(const CLandObject_NonAnim& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CLandObject_NonAnim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject_NonAnim::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	

	return S_OK;
}

void CLandObject_NonAnim::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	

}

void CLandObject_NonAnim::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	for (auto& iter : m_Actionlist)
	{
		if (iter->fBurning > 0.f)
		{
			iter->fBurning -= fTimeDelta;
			if (iter->fBurning < 0.f)
			{
				iter->bDead = true;
			}

			if (m_fMakeEffect >= 0.1f)
			{
				for (_int i = 0; i < 3; ++i)
					GET_INSTANCE->MakeEffect(EFFECT_TYPE::EFFECT_PARTICLE_FIRE, iter->pCollider->GetBoundingCenter());
			}

			if (m_fMakeFire >= 1.f)
			{
				_float3 fDirec = { _float((rand() % 1000) - (rand() % 1000)) / 1000.f, 0.5f, _float((rand() % 1000) - (rand() % 1000)) / 1000.f };
				GET_INSTANCE->Add_InterActionObject_BySpec(INTERACTION::INTER_FIRE, nullptr, iter->pCollider->GetBoundingCenter(), fDirec);
			}


			
		}


		if ((!iter->bSetting) || (m_eType == INTERACTION::INTER_ITEM))
		{
			CPhysicsManager::P_RESULT tResult = {};

			tResult = GET_INSTANCE->Total_Physics(*iter->pTransform, *iter->pCollider, true, true, false, fTimeDelta);
			GET_INSTANCE->Update_By_P_Result(iter->pTransform, iter->pCollider, tResult);

			if (GET_INSTANCE->Check_OnGround(iter->pCollider->GetBoundingCenter(), iter->pCollider->GetBoundingExtents()))
				iter->bSetting = true;
		}
		else
		{
			
		}

		m_bActive = GET_INSTANCE->GetMonsterMake();


		if (m_eType == INTERACTION::INTER_MONSTERMAKER)
		{
			if (!m_bActive)
				continue;

			if (iter->iActCount >= m_iMonsterMake)
				continue;
			
			if (m_fShowTime == 0.f)
				m_fShowTime = 1.0f;
			else
				continue;

			_vector vPlayer = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
			_vector vThis = iter->pTransform->Get_State(CTransform::STATE_POSITION);

			_float fDistance = sqrt(pow(vPlayer.m128_f32[0] - vThis.m128_f32[0], 2) + pow(vPlayer.m128_f32[1] - vThis.m128_f32[1], 2) + pow(vPlayer.m128_f32[2] - vThis.m128_f32[2], 2));

			_float3 fCheck{};
			XMStoreFloat3(&fCheck, vThis);

			if (fDistance < 30.f)
			{
				_int iDirec = _int(m_pGameInstance->Get_Random(0.f, 4.f));

				if (iDirec == 0)
				{
					if (!GET_INSTANCE->Check_Wall(fCheck, { 1.0f, 0.5f, 0.f }, 1))
					{
						fCheck.x += 1.f;
						fCheck.y += 0.5f;
						++iter->iActCount;
					}
					else
						++iDirec;
				}
				
				if (iDirec == 1)
				{
					if (!GET_INSTANCE->Check_Wall(fCheck, { -1.0f, 0.5f, 0.f }, 1))
					{
						fCheck.x -= 1.f;
						fCheck.y += 0.5f;
						++iter->iActCount;
					}
					else
						++iDirec;
				}

				if (iDirec == 2)
				{
					if (!GET_INSTANCE->Check_Wall(fCheck, { 0.f, 0.5f, 1.f }, 1))
					{
						fCheck.z += 1.f;
						fCheck.y += 0.5f;
						++iter->iActCount;
					}
					else
						++iDirec;
				}

				if (iDirec == 3)
				{
					if (!GET_INSTANCE->Check_Wall(fCheck, { 0.f, 0.5f, -1.f }, 1))
					{
						fCheck.z -= 1.f;
						fCheck.y += 0.5f;
						++iter->iActCount;
					}
					else
						fCheck.x = -1.f;
				}

				if (fCheck.x != -1.f)
				{
					GET_INSTANCE->Make_Container_Enemy(fCheck, ENEMY_TYPE::ENEMY_TYPE_END, 0.f);
					static_cast<CLongvinter_Container*>(m_pGameInstance->Find_Object(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), -1))->SetMonsterMake(true);
				}
					
						
			}
		}
		
		if (m_eType == INTERACTION::INTER_CONTROL)
		{

		}

	}

	m_fMakeEffect -= fTimeDelta;
	if (m_fMakeEffect < 0.f)
		m_fMakeEffect = 0.1f;

	m_fMakeFire -= fTimeDelta;
	if (m_fMakeFire < 0.f)
		m_fMakeFire = 1.f;
}

void CLandObject_NonAnim::Late_Update(_float fTimeDelta)
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

HRESULT CLandObject_NonAnim::Render()
{
	__super::Render();

	for (auto& iter : m_Actionlist)
	{
		_float3 fPosition{};
		_vector vPosition = iter->pTransform->Get_State(CTransform::STATE_POSITION);

		if ((m_eType == INTERACTION::INTER_ITEM) || (m_eType == INTERACTION::INTER_CONTROL))
		{
			vPosition += _vector{ 0.f, iter->pCollider->GetBoundingExtents().y, 0.f, 0.f };
			iter->pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		

		XMStoreFloat3(&fPosition, vPosition);

		if (!GET_INSTANCE->GetIsLender(fPosition))
			continue;


		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &iter->pTransform->Get_WorldMatrix())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;

		_uint		iNumMeshes = m_vecModelCom[iter->iIndex]->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			if (m_bTexture)
				m_vecModelCom[iter->iIndex]->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i);


			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_vecModelCom[iter->iIndex]->Render(i)))
				return E_FAIL;
		}
#ifdef _DEBUG
		iter->pCollider->Render();
#endif

		if ((m_eType == INTERACTION::INTER_ITEM) || (m_eType == INTERACTION::INTER_CONTROL))
		{
			vPosition = iter->pTransform->Get_State(CTransform::STATE_POSITION) - _vector{ 0.f, iter->pCollider->GetBoundingExtents().y, 0.f, 0.f };
			iter->pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		
	}

	return S_OK;
}

void CLandObject_NonAnim::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);

	if (eIndex == INTERACTION::INTER_EXPLOSION_NORMAL)
	{
		if (!GET_INSTANCE->Check_OnGround(pAction->pCollider->GetBoundingCenter(), pAction->pCollider->GetBoundingExtents()))
			DestroyAction(pAction);
		else if (pAction->fBurning == 0.f)
			pAction->fBurning = 10.f;
	}
	else if (eIndex == INTERACTION::INTER_FIRE)
	{
		if (pAction->fBurning == 0.f)
			pAction->fBurning = 10.f;
	}


	

}

void CLandObject_NonAnim::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);

	if (eIndex == CONTAINER::CONTAINER_PLAYER)
	{
		if (m_eType == INTERACTION::INTER_CONTROL)
		{
			

			_vector vPlayer = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION);
			_vector vThis = pAction->pTransform->Get_State(CTransform::STATE_POSITION);

			vPlayer -= vThis;
		

			_float fDistance = sqrt(pow(vPlayer.m128_f32[0], 2) + pow(vPlayer.m128_f32[1], 2) + pow(vPlayer.m128_f32[2], 2));

			if (fDistance <= 1.f)
			{
				if ((pAction->pTrace == nullptr) || (pAction->pTrace->GetDead()))
				{
					_vector vAdjust = { 0.f,2.f,0.f,0.f };
					_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&pAction->pTransform->Get_WorldMatrix()), TEXT("터렛 콘솔"), TEXT("Error : 연결된 터렛이 nullptr입니다."), TEXT("없음"), TEXT("없음"), vAdjust);

					
					return;
	
				}
					
				_vector vAdjust = { 0.f,2.f,0.f,0.f };

				_bool bActive = GET_INSTANCE->Show_Interaction_Function(XMLoadFloat4x4(&pAction->pTransform->Get_WorldMatrix()), TEXT("터렛 콘솔"), TEXT("E키 : 위치 확인"), TEXT("F키 : 해킹"), TEXT("없음"), vAdjust);

				if (bActive)
				{
					if (m_pGameInstance->Get_DIKeyState(DIK_E))
						GET_INSTANCE->ShowInformMessage(TEXT("[TEST] 확인 기능 작동"));
					else if (m_pGameInstance->Get_DIKeyState(DIK_F))
					{
						GET_INSTANCE->OpenHackPage(static_cast<CContainer_Turret*>(pAction->pTrace));
					}
				}
			}


		}
	}
	else if (eIndex == CONTAINER::CONTAINER_TURRET)
	{
		if (m_eType == INTERACTION::INTER_CONTROL)
		{
			pAction->pTrace = static_cast<CLongvinter_Container*>(pPoint);
		}
	}
}

HRESULT CLandObject_NonAnim::SetLandObject(INTERACTION eIndex)
{
	m_fSpec_PushedPower = 0.f;
	m_fSpec_PushedPower_Decrease = 0.f;
	m_iColliderType = CCollider::TYPE_AABB;
	m_fSpec_Time = 0.f;
	m_fSpec_Sensor = 1.f;
	m_eType = eIndex;

	if (eIndex == INTERACTION::INTER_TREE)
	{
		// 나무
		m_vecModelCom.resize(2);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Tree_Fine"), TEXT("Com_Model_0"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Tree_Normal"), TEXT("Com_Model_1"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.3f,0.2f };
		m_fSpec_Scale = 0.5f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_BUSH)
	{
		// 덤불
		m_vecModelCom.resize(3);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_1"), TEXT("Com_Model_2"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_2"), TEXT("Com_Model_3"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Bush_3"), TEXT("Com_Model_4"), reinterpret_cast<CComponent**>(&m_vecModelCom[2]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.4f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_ROCK)
	{
		// 바위
		m_vecModelCom.resize(8);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_1"), TEXT("Com_Model_5"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_2"), TEXT("Com_Model_6"), reinterpret_cast<CComponent**>(&m_vecModelCom[1]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_3"), TEXT("Com_Model_7"), reinterpret_cast<CComponent**>(&m_vecModelCom[2]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_4"), TEXT("Com_Model_8"), reinterpret_cast<CComponent**>(&m_vecModelCom[3]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_5"), TEXT("Com_Model_9"), reinterpret_cast<CComponent**>(&m_vecModelCom[4]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_6"), TEXT("Com_Model_10"), reinterpret_cast<CComponent**>(&m_vecModelCom[5]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_7"), TEXT("Com_Model_11"), reinterpret_cast<CComponent**>(&m_vecModelCom[6]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Rock_8"), TEXT("Com_Model_12"), reinterpret_cast<CComponent**>(&m_vecModelCom[7]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.1f;
		m_bTexture = true;
	}
	else if (eIndex == INTERACTION::INTER_APPLETREE)
	{
		// 사과나무
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_AppleTree"), TEXT("Com_Model_13"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,1.f,0.2f };
		m_fSpec_Scale = 1.f;
		m_bTexture = true;
	}
	else if (eIndex == INTERACTION::INTER_ITEM)
	{
		// 아이템
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_ItemPocket"), TEXT("Com_Model_14"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.2f,0.2f,0.2f };
		m_fSpec_Scale = 0.3f;
		m_fSpec_PushedPower = 5.f;
		m_fSpec_PushedPower_Decrease = 5.f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_DOOR)
	{
		// 문
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Door"), TEXT("Com_Model_15"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.5f,1.f,0.5f };
		m_fSpec_Scale = 2.f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_MONSTERMAKER)
	{
		// 몬스터 생성기 
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_MonsterMaker"), TEXT("Com_Model_16"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.5f,1.f,0.5f };
		m_fSpec_Scale = 1.f;
		m_bTexture = false;
	}
	else if (eIndex == INTERACTION::INTER_CONTROL)
	{
		// 터렛 콘솔
		m_vecModelCom.resize(1);

		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Model_Turret_Control"), TEXT("Com_Model_17"), reinterpret_cast<CComponent**>(&m_vecModelCom[0]))))
			return E_FAIL;
		if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;

		m_fSpec_Extent = { 0.5f,0.5f,0.5f };
		m_fSpec_Scale = 1.5f;
		m_fSpec_Sensor = 20.f;
		m_bTexture = true;
	}
	


	
}

void CLandObject_NonAnim::DestroyAction(INTER_INFO* pAction)
{
	pAction->bDead = true; 
}

HRESULT CLandObject_NonAnim::Ready_Components()
{

	


	return S_OK;
}

HRESULT CLandObject_NonAnim::Ready_PartObjects()
{
	return S_OK;
}

CLandObject_NonAnim* CLandObject_NonAnim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLandObject_NonAnim* pInstance = new CLandObject_NonAnim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandObject_NonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandObject_NonAnim::Clone(void* pArg)
{
	CLandObject_NonAnim* pInstance = new CLandObject_NonAnim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLandObject_NonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandObject_NonAnim::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	for (auto& iter : m_vecModelCom)
		Safe_Release(iter);

	m_vecModelCom.clear();
}
