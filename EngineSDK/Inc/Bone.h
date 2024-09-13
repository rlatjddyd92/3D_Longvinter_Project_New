#pragma once

#include "Base.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	const _float4x4* Get_CombinedTransformationMatrix_Ptr() const {
		return &m_CombinedTransformationMatrix;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const aiNode* pAIBone, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

	void SaveModel(HANDLE hHandle, DWORD* byte);
	HRESULT LoadModel(HANDLE hHandle, DWORD* byte, _int iParentBoneIndex);

private:
	_char				m_szName[MAX_PATH] = {};

	/* �θ�������� ǥ���� �� ������ �������. */
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_TransformationMatrix_Origin = {};

	/* �� ���� �������� ������� ( �� ������� * �θ��� ����������� ) */
	/* m_TransformationMatrix * Parent`s m_CombinedTransformationMatrix */
	_float4x4			m_CombinedTransformationMatrix = {};
	_int				m_iParentBoneIndex = { -1 };
	
public:
	static CBone* Create(const aiNode* pAIBone, _int iParentBoneIndex);
	static CBone* Create(HANDLE hHandle, DWORD* byte, _int iParentBoneIndex);
	CBone* Clone();
	virtual void Free() override;
};

END