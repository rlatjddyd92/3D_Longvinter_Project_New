#include "stdafx.h"
#include "LandObject.h"
#include "ClientInstance.h"
#include "Longvinter_Container.h"

CLandObject::CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInterAction{ pDevice, pContext }
{
}

CLandObject::CLandObject(const CLandObject& Prototype)
	: CInterAction{ Prototype }
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
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

void CLandObject::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLandObject::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLandObject::Late_Update(_float fTimeDelta)
{

	__super::Late_Update(fTimeDelta);
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

void CLandObject::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_InterAction(pPoint, eIndex, pAction);
}

void CLandObject::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction)
{
	__super::Collision_Reaction_Container(pPoint, eIndex, pAction);
}


HRESULT CLandObject::Ready_Components()
{
	return S_OK;
}

HRESULT CLandObject::Ready_PartObjects()
{
	return S_OK;
}

CLandObject* CLandObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLandObject* pInstance = new CLandObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLandObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLandObject::Clone(void* pArg)
{
	CLandObject* pInstance = new CLandObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLandObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLandObject::Free()
{
	__super::Free();

	
}
