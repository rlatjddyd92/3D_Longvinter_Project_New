#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& Prototype)
	: m_iNumChannels{ Prototype.m_iNumChannels }
	, m_Channels{ Prototype.m_Channels }
	, m_Duration{ Prototype.m_Duration }
	, m_SpeedPerSec{ Prototype.m_SpeedPerSec }
	, m_CurrentTrackPosition{ Prototype.m_CurrentTrackPosition }
	, m_CurrentKeyFrameIndices{ Prototype.m_CurrentKeyFrameIndices }

{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel)
{
	m_Duration = pAIAnimation->mDuration;
	m_SpeedPerSec = pAIAnimation->mTicksPerSecond;

	/* 이 애니메이션이 사용하는 뼈의 갯수저장한다. */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.emplace_back(pChannel);
	}

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrices(const vector<CBone*>& Bones, _bool isLoop, _float fTimeDelta)
{
	/* 현재 재생위치를 계산하낟. */
	m_CurrentTrackPosition += m_SpeedPerSec * fTimeDelta;

	if (m_CurrentTrackPosition >= m_Duration)
	{
		if (false == isLoop)
			return true;

		m_CurrentTrackPosition = 0.f;
	}

	/* 현재 재생위치에 맞게 현재 애니메이션이 컨트롤해야 할 뼈의 상태들을 갱신해준다. */
	_uint		iChannelIndex = { 0 };
	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(Bones, &m_CurrentKeyFrameIndices[iChannelIndex++], m_CurrentTrackPosition);
	}

	return false;
}

void CAnimation::SaveModel(HANDLE hHandle, DWORD* byte)
{
	WriteFile(hHandle, &m_Duration, sizeof(_double), byte, nullptr);
	WriteFile(hHandle, &m_SpeedPerSec, sizeof(_double), byte, nullptr);
	WriteFile(hHandle, &m_iNumChannels, sizeof(_uint), byte, nullptr);
	for (size_t i = 0; i < m_iNumChannels; i++)
		m_Channels[i]->SaveModel(hHandle, byte);
}

HRESULT CAnimation::LoadModel(HANDLE hHandle, DWORD* byte, const class CModel* pModel)
{
	ReadFile(hHandle, &m_Duration, sizeof(_double), byte, nullptr);
	ReadFile(hHandle, &m_SpeedPerSec, sizeof(_double), byte, nullptr);

	/* 이 애니메이션이 사용하는 뼈의 갯수저장한다. */
	ReadFile(hHandle, &m_iNumChannels, sizeof(_uint), byte, nullptr);

	m_CurrentKeyFrameIndices.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(hHandle, byte, pModel);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.emplace_back(pChannel);
	}

	return S_OK;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const class CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, pModel)))
	{
		MSG_BOX(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create(HANDLE hHandle, DWORD* byte, const class CModel* pModel)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->LoadModel(hHandle, byte, pModel)))
	{
		MSG_BOX(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
	m_CurrentKeyFrameIndices.clear();
}
