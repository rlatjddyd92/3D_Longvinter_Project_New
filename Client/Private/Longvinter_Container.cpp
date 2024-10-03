#include "stdafx.h"
#include "..\Public\Longvinter_Container.h"

#include "Body_Human.h"
#include "ClientInstance.h"
#include "Tool.h"

CLongvinter_Container::CLongvinter_Container(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CLongvinter_Container::CLongvinter_Container(const CLongvinter_Container& Prototype)
	: CContainerObject{ Prototype }
{
}

HRESULT CLongvinter_Container::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLongvinter_Container::Initialize(void* pArg)
{
	GAMEOBJECT_DESC* pTemp = static_cast<GAMEOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CLongvinter_Container::Priority_Update(_float fTimeDelta)
{


}

void CLongvinter_Container::Update(_float fTimeDelta)
{
	
}

void CLongvinter_Container::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CLongvinter_Container::Render()
{

	return S_OK;
}

void CLongvinter_Container::Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex)
{
}

void CLongvinter_Container::Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex)
{
}

CLongvinter_Container* CLongvinter_Container::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLongvinter_Container* pInstance = new CLongvinter_Container(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLongvinter_Container"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CLongvinter_Container::Clone(void* pArg)
{
	CLongvinter_Container* pInstance = new CLongvinter_Container(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLongvinter_Container"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLongvinter_Container::Free()
{
	__super::Free();

}
