#pragma once

#include "Component.h"
#include "Bone.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	TYPE Get_ModelType() const {
		return m_eType;
	}

	_int Get_BoneIndex(const _char* pBoneName) const;
	_matrix Get_BoneCombindTransformationMatrix(_uint iBoneIndex) const {
		return m_Bones[iBoneIndex]->Get_CombinedTransformationMatrix();
	}
	const _float4x4* Get_BoneCombindTransformationMatrix_Ptr(const _char* pBoneName) const {
		return m_Bones[Get_BoneIndex(pBoneName)]->Get_CombinedTransformationMatrix_Ptr();
	}

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, _bool bTexture, const _char* pModelFilePath, _fmatrix PreTransformMatrix, _bool bIsLoad = false);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

	void SaveModel(const _char* pModelFilePath);
	HRESULT LoadModel(const _char* pModelFilePath, TYPE eType, _bool bTexture, _fmatrix PreTransformMatrix);

public:
	void SetUp_Animation(_uint iAnimationIndex, _bool isLoop = false)
	{
		m_iCurrentAnimIndex = iAnimationIndex;
		m_isLoop = isLoop;
	}

	/* ���� �����δ�.(CBone`s m_TransformationMatrix����� �����Ѵ�.) */
	/* ��� ���� ������ �ִ� m_CombinedTransformationMatrix�� �����Ѵ�. */
	_bool Play_Animation(_float fTimeDelta);


public:
	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex);
	HRESULT Bind_MeshBoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

	void SetAnimSpeed(_float fSpeed) { m_fAnimSpeed = fSpeed; } // <- �ִϸ��̼� �ӵ� ���� (1�� ����ӵ� 0�� ����� ���� �������� 0�̵Ǹ� ��� �ȵ�, �Ƹ� ���� ������ �ݴ�� ������ ��) 
	_float GetAnimSpeed() { return m_fAnimSpeed; }

	void SetTexture(_int iMetarial, _int iTextureType, CTexture* pTexture) { m_Materials[iMetarial].pMaterialTextures[iTextureType] = pTexture; }



private:
	/* ���� �־��� ��ο� �ش��ϴ� ������ ������ �о aiScene��ü�� �������ش�. */
	Assimp::Importer				m_Importer;

	/* ���Ϸκ��� �о�� ��� ������ �����ϰ� �ִ´�. */
	const aiScene* m_pAIScene = { nullptr };
	TYPE							m_eType = { TYPE_END };

private: /* �޽��� ������ �����Ѵ�. */
	_uint							m_iNumMeshes = { 0 };
	vector<class CMesh*>			m_Meshes;
	_float4x4						m_PreTransformMatrix = {};

private:
	_uint							m_iNumMaterials = { 0 };
	vector<_tchar*>					m_Materials_Texture;
	vector<_int>					m_Materials_Exist;
	vector<MESH_MATERIAL>			m_Materials;

private:
	vector<class CBone*>			m_Bones;

private:
	_bool							m_isLoop = { false };
	_uint							m_iCurrentAnimIndex = { 0 };
	_uint							m_iNumAnimations = { 0 };
	vector<class CAnimation*>		m_Animations;
	_float							m_fAnimSpeed = 1.f; // <- �ִϸ��̼� �ӵ� ���� (1�� ����ӵ� 0�� ����� ���� �������� 0�̵Ǹ� ��� �ȵ�, �Ƹ� ���� ������ �ݴ�� ������ ��) 

	/* ���� �ִϸ��̼��� ��� ��ġ. */
	_double							m_CurrentTrackPosition = {};
	// _double							m_CurrentTrackPosition = {};

	/* �� �ִϸ��̼Ǵ� �� ä�ε��� ���� Ű�������ε��� */
	vector<vector<_uint>>			m_KeyFrameIndices;

public:
	HRESULT	Ready_Meshes(_bool bTexture);
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAIBone, _int iParentBoneIndex);
	HRESULT Ready_Animations();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType,_bool bTexture, const _char* pModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity(), _bool bIsLoad = false);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END