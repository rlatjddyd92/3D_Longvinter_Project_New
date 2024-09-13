#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel);
	_bool Update_TransformationMatrices(const vector<class CBone*>& Bones, _bool isLoop, _float fTimeDelta);

	void SaveModel(HANDLE hHandle, DWORD* byte);
	HRESULT LoadModel(HANDLE hHandle, DWORD* byte, const class CModel* pModel);

private:
	/* 이 애니메이션이 사용하는 뼈의 갯수 */
	_uint						m_iNumChannels = { 0 };
	/* 실제 뼈들의 정보*/
	/* Channel : 뼈 (시간에 따른 이 뼈의 상태 행렬을 보관한다) */
	vector<class CChannel*>		m_Channels;

	/* 이 애니메이션을 구동하기위해 거리는 전체 거리. */
	_double						m_Duration = {};

	/* 애니메이션의 초당 재생속도 */
	_double						m_SpeedPerSec = {};

	/* m_CurrentTrackPosition += m_SpeedPerSec * fTimeDelta */
	_double						m_CurrentTrackPosition = {};

	vector<_uint>				m_CurrentKeyFrameIndices = { 0 };


public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const class CModel* pModel);
	static CAnimation* Create(HANDLE hHandle, DWORD* byte, const class CModel* pModel);
	CAnimation* Clone();
	virtual void Free() override;
};

END