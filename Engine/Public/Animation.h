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
	/* �� �ִϸ��̼��� ����ϴ� ���� ���� */
	_uint						m_iNumChannels = { 0 };
	/* ���� ������ ����*/
	/* Channel : �� (�ð��� ���� �� ���� ���� ����� �����Ѵ�) */
	vector<class CChannel*>		m_Channels;

	/* �� �ִϸ��̼��� �����ϱ����� �Ÿ��� ��ü �Ÿ�. */
	_double						m_Duration = {};

	/* �ִϸ��̼��� �ʴ� ����ӵ� */
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