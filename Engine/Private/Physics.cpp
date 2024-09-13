#include "..\Public\Physics.h"

CPhysics::CPhysics(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CPhysics::CPhysics(const CPhysics& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CPhysics::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhysics::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CPhysics::Render(_uint iMeshIndex)
{
	return S_OK;
}

CPhysics* CPhysics::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhysics* pInstance = new CPhysics(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPhysics"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPhysics::Clone(void* pArg)
{
	CPhysics* pInstance = new CPhysics(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CPhysics"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhysics::Free()
{
	__super::Free();
}
