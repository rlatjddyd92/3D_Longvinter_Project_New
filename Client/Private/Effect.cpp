#include "stdafx.h"
#include "Effect.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CEffect::CEffect(const CEffect& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
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

void CEffect::Priority_Update(_float fTimeDelta)
{
	for (auto& iter : m_EffectInfolist)
		if (iter->fTime > 0.f)
		{
			iter->fTime -= fTimeDelta;
			if (iter->fTime < 0.f)
			{
				iter->fTime = 0.f;
				iter->bDead = true;
			}
		}
}

void CEffect::Update(_float fTimeDelta)
{
	for (auto& iter : m_EffectInfolist)
	{
		if (iter->bDead)
			continue;

		iter->fPosition.x += (iter->fDirec.x * iter->fPower);
		iter->fPosition.y += (iter->fDirec.y * iter->fPower);
		iter->fPosition.z += (iter->fDirec.z * iter->fPower);

		if (iter->fPower > 0.f)
		{
			iter->fPower -= iter->fPowerDecrease;
			if (iter->fPower < 0.f)
				iter->fPower = 0.f;
		}
		

		iter->fPosition.y += iter->fFly;

		if (m_bSpec_TerrainDelete)
			if (GET_INSTANCE->Check_IsTerrain(iter->fPosition))
				iter->bDead = true;
	}



}

void CEffect::Late_Update(_float fTimeDelta)
{
	for (list<E_INFO*>::iterator iter = m_EffectInfolist.begin(); iter != m_EffectInfolist.end(); )
	{
		if ((*iter)->bDead)
		{
			Safe_Delete((*iter));
			iter = m_EffectInfolist.erase(iter);
		}
		else
			++iter;
	}



}

HRESULT CEffect::Render()
{
	return S_OK;




}

_float CEffect::BillBoard(CTransform* pTransform)
{
	_vector vCam = (m_pGameInstance->Get_CamPosition_Vector() - pTransform->Get_State(CTransform::STATE_POSITION));
	_vector vLook = pTransform->Get_State(CTransform::STATE_LOOK);

	vCam.m128_f32[1] = 0.f;
	vLook.m128_f32[1] = 0.f;

	_float fDot = XMVector3Dot(vCam, vLook).m128_f32[0];
	_float fCos = (sqrt(pow(vCam.m128_f32[0], 2) + pow(vCam.m128_f32[2], 2)) * sqrt(pow(vLook.m128_f32[0], 2) + pow(vLook.m128_f32[2], 2)));
	_float fAngle = acos(fDot / fCos);

	if (isnan(fAngle))
		fAngle = 0.f;

	_float3 fCam{};
	_float3 fLook{};

	XMStoreFloat3(&fCam, vCam);
	XMStoreFloat3(&fLook, vLook);

	if (!GET_INSTANCE->Check_CCW_XZ(fCam, { 0.f,0.f,0.f }, fLook))
		fAngle *= -1;

	pTransform->Rotation({ 0.f, 1.f, 0.f }, fAngle * PI_DEFINE);

	return fAngle;
}

void CEffect::AddEffectBySpec(_float3 fPosition)
{
	E_INFO* pNew = new E_INFO;

	*pNew = m_tSpec;

	pNew->fPosition = fPosition;

	if (m_bRandDirec[0])
		pNew->fDirec.x = _float((rand() % 1000) - (rand() % 1000)) / 1000.f;
	if (m_bRandDirec[1])
		pNew->fDirec.y = _float((rand() % 1000) - (rand() % 1000)) / 1000.f;
	if (m_bRandDirec[2])
		pNew->fDirec.z = _float((rand() % 1000) - (rand() % 1000)) / 1000.f;

	m_EffectInfolist.push_back(pNew);
}



HRESULT CEffect::Ready_Components()
{
	return S_OK;
}

HRESULT CEffect::Ready_PartObjects()
{
	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& iter : m_EffectInfolist)
		Safe_Delete(iter);

	m_EffectInfolist.clear();
}
