#include "stdafx.h"
#include "LandObject_Anim.h"
#include "ClientInstance.h"


CLandObject_Anim::CLandObject_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject{ pDevice, pContext }
{
}

CLandObject_Anim::CLandObject_Anim(const CLandObject_Anim& Prototype)
	: CLandObject{ Prototype }
{
}

HRESULT CLandObject_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject_Anim::Initialize(void* pArg)
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

void CLandObject_Anim::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CLandObject_Anim::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLandObject_Anim::Late_Update(_float fTimeDelta)
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


}

HRESULT CLandObject_Anim::Render()
{
	return S_OK;
}

void CLandObject_Anim::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CLandObject_Anim::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}

HRESULT CLandObject_Anim::Ready_Components()
{
	return S_OK;
}

HRESULT CLandObject_Anim::Ready_PartObjects()
{
	return S_OK;
}

CLandObject_Anim* CLandObject_Anim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLandObject_Anim* pInstance = new CLandObject_Anim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandObject_Anim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandObject_Anim::Clone(void* pArg)
{
	CLandObject_Anim* pInstance = new CLandObject_Anim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLandObject_Anim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandObject_Anim::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom_Texture);
	Safe_Release(m_pShaderCom_NonTexture);

	for (auto& iter : m_vecModelCom)
		Safe_Release(iter);

	m_vecModelCom.clear();
}
