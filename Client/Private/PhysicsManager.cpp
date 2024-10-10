#include "stdafx.h"
#include "..\Public\PhysicsManager.h"

#include "GameInstance.h"
#include "ClientInstance.h"

CPhysicsManager::CPhysicsManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{




}

CPhysicsManager::CPhysicsManager(const CPhysicsManager& Prototype)
	: CGameObject{ Prototype }
{




}

HRESULT CPhysicsManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysicsManager::Initialize(void* pArg)
{
	return S_OK;
}

void CPhysicsManager::Priority_Update(_float fTimeDelta)
{
}

void CPhysicsManager::Update(_float fTimeDelta)
{
}

void CPhysicsManager::Late_Update(_float fTimeDelta)
{
}

HRESULT CPhysicsManager::Render()
{
	return S_OK;
}

CPhysicsManager::P_RESULT CPhysicsManager::Total_Physics(CTransform& Transform, CCollider& Collder, _bool IsGravity, _bool IsTerrainCollision, _bool IsSlideControl, _float fTimeDelta)
{
	CPhysicsManager::P_RESULT pResult = Make_P_Result(Transform, Collder);

	if (IsGravity)
		Gravity(&pResult, IsTerrainCollision, fTimeDelta);

	PushedPower(&pResult, fTimeDelta);

	if (IsTerrainCollision)
		CheckTerrainCollision(&pResult, IsSlideControl);

	return pResult;
}

CPhysicsManager::P_RESULT CPhysicsManager::Make_P_Result(CTransform& Transform, CCollider& Collder)
{
	CPhysicsManager::P_RESULT pResult{};

	pResult.fWorld = Transform.Get_WorldMatrix();
	pResult.fPushed_Power_Dir = Transform.Get_Pushed_Dir();
	pResult.fPushed_Power = Transform.Get_Pushed_Power();
	pResult.fBeforePosition = Transform.Get_BeforePosition();
	pResult.fCollider_Center = Collder.GetBoundingCenter();
	pResult.fCollider_Extents = Collder.GetBoundingExtents();
	pResult.fPushed_Power_Decrease = Transform.Get_Pushed_PowerDecrease();
	//pResult.fCollider_Radius = Collder.GetBoundingRadius_Sphere();

	return pResult;
}

CPhysicsManager::P_RESULT CPhysicsManager::Bounce_Physics(CTransform& Transform, CCollider& Collder, _bool IsGravity, _float fTimeDelta)
{
	CPhysicsManager::P_RESULT pResult = Make_P_Result(Transform, Collder);

	if (IsGravity)
		Gravity(&pResult, true, fTimeDelta);

	PushedPower(&pResult, fTimeDelta);

	BounceControl(&pResult, false);

	return pResult;
}

CPhysicsManager::P_RESULT CPhysicsManager::LandMine_Physics(CTransform& Transform, CCollider& Collder, _float fTimeDelta)
{
	CPhysicsManager::P_RESULT pResult = Make_P_Result(Transform, Collder);

	Gravity(&pResult, true, fTimeDelta);

	PushedPower(&pResult, fTimeDelta);

	BounceControl(&pResult, true);

	return pResult;
}

void CPhysicsManager::Update_By_P_Result(CTransform* Transform, CCollider* Collder, P_RESULT tResult)
{
	Transform->Set_State(CTransform::STATE_RIGHT, { tResult.fWorld._11,tResult.fWorld._12, tResult.fWorld._13,tResult.fWorld._14 });
	Transform->Set_State(CTransform::STATE_UP, { tResult.fWorld._21,tResult.fWorld._22, tResult.fWorld._23,tResult.fWorld._24 });
	Transform->Set_State(CTransform::STATE_LOOK, { tResult.fWorld._31,tResult.fWorld._32, tResult.fWorld._33,tResult.fWorld._34 });
	Transform->Set_State(CTransform::STATE_POSITION, { tResult.fWorld._41,tResult.fWorld._42, tResult.fWorld._43,tResult.fWorld._44 });
	Transform->Set_Pushed_Power(tResult.fPushed_Power_Dir, tResult.fPushed_Power);
	Collder->Update(Transform->Get_WorldMatrix_Ptr());
}

void CPhysicsManager::Gravity(P_RESULT* tResult, _bool IsTerrainCollision, _float fTimeDelta)
{
	if (!GET_INSTANCE->Check_OnGround(tResult->fCollider_Center, tResult->fCollider_Extents))
	{
		_float3 fAdjust = { m_fGravity_Direction.x * m_fGravity_Power* fTimeDelta, m_fGravity_Direction.y * m_fGravity_Power* fTimeDelta, m_fGravity_Direction.z * m_fGravity_Power* fTimeDelta };
		tResult->fWorld._41 += fAdjust.x;
		tResult->fWorld._42 += fAdjust.y;
		tResult->fWorld._43 += fAdjust.z;
		tResult->fCollider_Center.x += fAdjust.x;
		tResult->fCollider_Center.y += fAdjust.y;
		tResult->fCollider_Center.z += fAdjust.z;
	}
}

void CPhysicsManager::PushedPower(P_RESULT* tResult, _float fTimeDelta)
{
	_float3 fAdjust = { tResult->fPushed_Power_Dir.x * tResult->fPushed_Power* fTimeDelta, tResult->fPushed_Power_Dir.y * tResult->fPushed_Power* fTimeDelta, tResult->fPushed_Power_Dir.z * tResult->fPushed_Power* fTimeDelta };
	tResult->fWorld._41 += fAdjust.x;
	tResult->fWorld._42 += fAdjust.y;
	tResult->fWorld._43 += fAdjust.z;
	tResult->fCollider_Center.x += fAdjust.x;
	tResult->fCollider_Center.y += fAdjust.y;
	tResult->fCollider_Center.z += fAdjust.z;
	tResult->fPushed_Power -= tResult->fPushed_Power_Decrease;
	if (tResult->fPushed_Power < 0)
		tResult->fPushed_Power = 0;

	if (tResult->fCollider_Center.y < 0.f)
		cout << tResult->fCollider_Center.x << " " << tResult->fCollider_Center.y << " " << tResult->fCollider_Center.z << "\n";
}

void CPhysicsManager::CheckTerrainCollision(P_RESULT* tResult, _bool IsSlideControl)
{
	_float3 vAdjustPosition = {};
	LCUBEDIRECION eDirec = LCUBEDIRECION::LDIREC_END;

	while (1)
	{
		vAdjustPosition = GET_INSTANCE->Check_Terrain_Collision_Adjust(tResult->fCollider_Center, tResult->fCollider_Extents, tResult->Get_AdjustVector(), &eDirec);

		if ((vAdjustPosition.x != -1.f) + (vAdjustPosition.y != -1.f) + (vAdjustPosition.z != -1.f) == 3)
		{
			tResult->fWorld._41 += vAdjustPosition.x;
			tResult->fWorld._42 += vAdjustPosition.y;
			tResult->fWorld._43 += vAdjustPosition.z;
			tResult->fCollider_Center.x += vAdjustPosition.x;
			tResult->fCollider_Center.y += vAdjustPosition.y;
			tResult->fCollider_Center.z += vAdjustPosition.z;
			tResult->fBeforePosition = { tResult->fWorld._41, tResult->fWorld._42, tResult->fWorld._43 };
		}
		else
			break;

		if (!IsSlideControl)
			break;

		vAdjustPosition.x *= -1;
		vAdjustPosition.y *= -1;
		vAdjustPosition.z *= -1;
		
		if ((eDirec == LCUBEDIRECION::LDIREC_TOP) || (eDirec == LCUBEDIRECION::LDIREC_BOTTOM))
			vAdjustPosition.y = 0.f;

		if ((eDirec == LCUBEDIRECION::LDIREC_EAST) || (eDirec == LCUBEDIRECION::LDIREC_WEST))
			vAdjustPosition.x = 0.f;

		if ((eDirec == LCUBEDIRECION::LDIREC_NORTH) || (eDirec == LCUBEDIRECION::LDIREC_SOUTH))
			vAdjustPosition.z = 0.f;

		tResult->fWorld._41 += vAdjustPosition.x;
		tResult->fWorld._42 += vAdjustPosition.y;
		tResult->fWorld._43 += vAdjustPosition.z;
		tResult->fCollider_Center.x += vAdjustPosition.x;
		tResult->fCollider_Center.y += vAdjustPosition.y;
		tResult->fCollider_Center.z += vAdjustPosition.z;
	}
}

void CPhysicsManager::BounceControl(P_RESULT* tResult, _bool bLandMine)
{
	_float3 vAdjustPosition = {};
	LCUBEDIRECION eDirec = LCUBEDIRECION::LDIREC_END;

	_bool bCollision = false;

	while (1)
	{
		vAdjustPosition = GET_INSTANCE->Check_Terrain_Collision_Adjust(tResult->fCollider_Center, tResult->fCollider_Extents, tResult->Get_AdjustVector(), &eDirec);

		if ((vAdjustPosition.x != -1.f) + (vAdjustPosition.y != -1.f) + (vAdjustPosition.z != -1.f) == 3)
		{
			tResult->fWorld._41 += vAdjustPosition.x;
			tResult->fWorld._42 += vAdjustPosition.y;
			tResult->fWorld._43 += vAdjustPosition.z;
			tResult->fCollider_Center.x += vAdjustPosition.x;
			tResult->fCollider_Center.y += vAdjustPosition.y;
			tResult->fCollider_Center.z += vAdjustPosition.z;
			tResult->fBeforePosition = { tResult->fWorld._41, tResult->fWorld._42, tResult->fWorld._43 };

			bCollision = true;
		}
		else
			break;

		if ((bLandMine) && (eDirec == LCUBEDIRECION::LDIREC_TOP))
		{
			
			tResult->fPushed_Power = 0.f;
			break;
		}
			
		vAdjustPosition.x *= -1;
		vAdjustPosition.y *= -1;
		vAdjustPosition.z *= -1;

		

		if ((eDirec == LCUBEDIRECION::LDIREC_TOP) || (eDirec == LCUBEDIRECION::LDIREC_BOTTOM))
		{
			vAdjustPosition.y = 0.f;
			tResult->fPushed_Power_Dir.y *= -1;
		}
			

		if ((eDirec == LCUBEDIRECION::LDIREC_EAST) || (eDirec == LCUBEDIRECION::LDIREC_WEST))
		{
			vAdjustPosition.x = 0.f;
			tResult->fPushed_Power_Dir.x *= -1;
		}
			

		if ((eDirec == LCUBEDIRECION::LDIREC_NORTH) || (eDirec == LCUBEDIRECION::LDIREC_SOUTH))
		{
			vAdjustPosition.z = 0.f;
			tResult->fPushed_Power_Dir.z *= -1;
		}
			

		tResult->fWorld._41 += vAdjustPosition.x;
		tResult->fWorld._42 += vAdjustPosition.y;
		tResult->fWorld._43 += vAdjustPosition.z;
		tResult->fCollider_Center.x += vAdjustPosition.x;
		tResult->fCollider_Center.y += vAdjustPosition.y;
		tResult->fCollider_Center.z += vAdjustPosition.z;
	}

	if (bCollision)
		tResult->fPushed_Power *= 0.9f;
}

_bool CPhysicsManager::Check_CCW_XZ(_float3 fPointA, _float3 fPointB, _float3 fPointC)
{
	_float fX[3] = { fPointA.x,  fPointB.x ,  fPointC.x };
	_float fZ[3] = { fPointA.z,  fPointB.z ,  fPointC.z };

	_float fResult = ((fX[1] - fX[0]) * (fZ[2] - fZ[0])) - ((fX[2] - fX[0]) * (fZ[1] - fZ[0]));


	return (fResult > 0.f);
}

HRESULT CPhysicsManager::Ready_Components()
{
	return E_NOTIMPL;
}

CPhysicsManager* CPhysicsManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhysicsManager* pInstance = new CPhysicsManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPhysicsManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPhysicsManager::Clone(void* pArg)
{
	CPhysicsManager* pInstance = new CPhysicsManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPhysicsManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhysicsManager::Free()
{
}
