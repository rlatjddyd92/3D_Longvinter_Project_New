#include "..\Public\Channel.h"
#include "Bone.h"
#include "Model.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim * pAIChannel, const CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);	

	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3			vScale = {};
	_float4			vRotation = {};
	_float3			vTranslation = {};

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME			KeyFrame{};

		if(pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.TrackPosition = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;			
			KeyFrame.TrackPosition = pAIChannel->mRotationKeys[i].mTime;
		}
	

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.TrackPosition = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vTranslation = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition)
{
	if (0.0 == CurrentTrackPosition)
		(*pCurrentKeyFrameIndex) = 0;

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector		vScale, vRotation, vTranslation;

	/* ������ �ʿ���� ��� */
	if (CurrentTrackPosition >= LastKeyFrame.TrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}

	/* ������ �ʿ��� ��κ� ��� */
	else
	{
		if(CurrentTrackPosition >= m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].TrackPosition)
			++(*pCurrentKeyFrameIndex);

		_vector		vSourScale = XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vScale);
		_vector		vDestScale = XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vScale);

		_vector		vSourRotation = XMLoadFloat4(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vRotation);
		_vector		vDestRotation = XMLoadFloat4(&m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vRotation);

		_vector		vSourTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex)].vTranslation), 1.f);
		_vector		vDestTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vTranslation), 1.f);

		/* �װ� ���� �����Ѱ�. */
		_double		Ratio = (CurrentTrackPosition - m_KeyFrames[(*pCurrentKeyFrameIndex)].TrackPosition) / (m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].TrackPosition - m_KeyFrames[(*pCurrentKeyFrameIndex)].TrackPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, (_float)Ratio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, (_float)Ratio);
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, (_float)Ratio);
	}

	/* �� ������ ������� = ȸ�������� �翬�� �θ����.(�ڽĻ����� ���¸� ���鶧 ������������ �����ؾ��Ѵ�.) */

	_matrix			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::SaveModel(HANDLE hHandle, DWORD* byte)
{
	_uint iSize = sizeof(m_szName) / sizeof(_char);
	WriteFile(hHandle, &iSize, sizeof(_uint), byte, nullptr);
	for (_uint i = 0; i < iSize; ++i)
		WriteFile(hHandle, m_szName + i, sizeof(_char), byte, nullptr);

	WriteFile(hHandle, &m_iNumKeyFrames, sizeof(_uint), byte, nullptr);

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		WriteFile(hHandle, &m_KeyFrames[i].vScale, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &m_KeyFrames[i].vRotation, sizeof(_float4), byte, nullptr);
		WriteFile(hHandle, &m_KeyFrames[i].vTranslation, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &m_KeyFrames[i].TrackPosition, sizeof(_double), byte, nullptr);
	}
}

HRESULT CChannel::LoadModel(HANDLE hHandle, DWORD* byte, const class CModel* pModel)
{
	_uint iSize = 0;
	ReadFile(hHandle, &iSize, sizeof(_uint), byte, nullptr);

	for (_uint i = 0; i < iSize; ++i)
		ReadFile(hHandle, m_szName + i, sizeof(_char), byte, nullptr);

	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);
	ReadFile(hHandle, &m_iNumKeyFrames, sizeof(_uint), byte, nullptr);

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME			KeyFrame{};

		ReadFile(hHandle, &KeyFrame.vScale, sizeof(_float3), byte, nullptr);
		ReadFile(hHandle, &KeyFrame.vRotation, sizeof(_float4), byte, nullptr);
		ReadFile(hHandle, &KeyFrame.vTranslation, sizeof(_float3), byte, nullptr);
		ReadFile(hHandle, &KeyFrame.TrackPosition, sizeof(_double), byte, nullptr);

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

CChannel * CChannel::Create(const aiNodeAnim * pAIChannel, const CModel* pModel)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		MSG_BOX(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Create(HANDLE hHandle, DWORD* byte, const class CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->LoadModel(hHandle, byte, pModel)))
	{
		MSG_BOX(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CChannel::Free()
{
	__super::Free();
	m_KeyFrames.clear();
}
