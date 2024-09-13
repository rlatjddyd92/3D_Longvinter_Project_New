#include "..\Public\Bone.h"

CBone::CBone()
{

}

HRESULT CBone::Initialize(const aiNode * pAIBone, _int iParentBoneIndex)
{
	m_iParentBoneIndex = iParentBoneIndex;

	strcpy_s(m_szName, pAIBone->mName.data);	

	memcpy(&m_TransformationMatrix, &pAIBone->mTransformation, sizeof(_float4x4));
	m_TransformationMatrix_Origin = m_TransformationMatrix;
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));	

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	if(-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * Bones[m_iParentBoneIndex]->Get_CombinedTransformationMatrix());
}

void CBone::SaveModel(HANDLE hHandle, DWORD* byte)
{
	WriteFile(hHandle, &m_iParentBoneIndex, sizeof(_int), byte, nullptr);
	_uint iSize = sizeof(m_szName) / sizeof(_char);
	WriteFile(hHandle, &iSize, sizeof(_uint), byte, nullptr);
	for (_uint i = 0; i < iSize; ++i)
		WriteFile(hHandle, m_szName + i, sizeof(_char), byte, nullptr);
	WriteFile(hHandle, &m_TransformationMatrix_Origin, sizeof(_float4x4), byte, nullptr);
}

HRESULT CBone::LoadModel(HANDLE hHandle, DWORD* byte, _int iParentBoneIndex)
{
	ReadFile(hHandle, &m_iParentBoneIndex, sizeof(_int), byte, nullptr);

	_uint iSize = 0;
	ReadFile(hHandle, &iSize, sizeof(_uint), byte, nullptr);

	for (_uint i = 0; i < iSize; ++i)
		ReadFile(hHandle, m_szName + i, sizeof(_char), byte, nullptr);

	ReadFile(hHandle, &m_TransformationMatrix, sizeof(_float4x4), byte, nullptr);
	m_TransformationMatrix_Origin = m_TransformationMatrix;
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

CBone * CBone::Create(const aiNode * pAIBone, _int iParentBoneIndex)
{
	CBone*		pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAIBone, iParentBoneIndex)))
	{
		MSG_BOX(TEXT("Failed to Created : CBone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Create(HANDLE hHandle, DWORD* byte, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->LoadModel(hHandle, byte, iParentBoneIndex)))
	{
		MSG_BOX(TEXT("Failed to Created : CBone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone * CBone::Clone()
{
	return new CBone(*this);
}


void CBone::Free()
{
	__super::Free();

}
