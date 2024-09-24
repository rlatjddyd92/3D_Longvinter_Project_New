#pragma once

#include "VIBuffer.h"
#include "Model.h"

/* 모델 = 메시 + 메시 + 메시 .. */
/* 메시로 구분하는 이유 : 파츠의 교체를 용이하게 만들어주기 위해서.*/
/* 메시 = 정점버퍼 + 인덱스 */

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
public:
	struct Vertices_Origin
	{
		XMFLOAT3 GetPosition() 
		{ 
			XMFLOAT3 tReturn = {};
			XMStoreFloat3(&tReturn, { vPosition.x,vPosition.y,vPosition.z });
			return tReturn;
		}
		XMFLOAT3 GetNormal()
		{
			XMFLOAT3 tReturn = {};
			XMStoreFloat3(&tReturn, { vNormal.x,vNormal.y,vNormal.z });
			return tReturn;
		}
		XMFLOAT2 GetTexCoord()
		{
			XMFLOAT2 tReturn = {};
			XMStoreFloat2(&tReturn, { vTexcoord.x,vTexcoord.y });
			return tReturn;
		}
		XMFLOAT3 GetTangent()
		{
			XMFLOAT3 tReturn = {};
			XMStoreFloat3(&tReturn, { vTangent.x,vTangent.y,vTangent.z });
			return tReturn;
		}


		_float3 vPosition = { };
		_float3 vNormal = { };
		_float2 vTexcoord = { };
		_float3 vTangent = { };
	}MINDI;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, _bool bTexture);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_BoneMatrices(const CModel* pModel, class CShader* pShader, const _char* pConstantName);

	void SaveModel(HANDLE hHandle, DWORD* byte);
	HRESULT LoadModel(HANDLE hHandle, DWORD* byte, const CModel* pModel, _fmatrix PreTransformMatrix, _bool bTexture);

private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 };

	/* uint : 모델에 로드해놓은 전체 뼈 중의 인덱스를 이야기한다. */
	vector<_int>		m_BoneIndices;

	_float4x4			m_BoneMatrices[g_iMaxMeshBones] = {};
	vector<_float4x4>	m_OffsetMatrices;

	vector<_uint>		m_vecNumWeights;
	

	// 바이너리 진행용 변수 
	_uint*				m_iIndices_Origin = { nullptr };


	vector<_float4x4>	m_OffsetMatrices_Origin;

	vector<_float3>		m_vecOriginPosition;

	

	vector<vector<_int>>		m_vecVerTexId_Origin;
	vector<vector<_float>>		m_vecWeight_Origin;

	VTXMESH* m_Vertices_Origin = { nullptr };
	VTXANIMMESH* m_Vertices_Origin_Anim = { nullptr };

	vector<Vertices_Origin> vecOrigin;
	vector<_float4> vecOrigin_Color;

	CModel::TYPE eType = CModel::TYPE::TYPE_END;
	_bool m_bTexture = true; 

private:
	HRESULT	Ready_VertexBuffer_NonAnim(_fmatrix PreTransformMatrix, const aiMesh* pAIMesh);
	HRESULT	Ready_VertexBuffer_Anim(const CModel* pModel, const aiMesh* pAIMesh);

	HRESULT	Save_VertexBuffer_NonAnim(HANDLE hHandle, DWORD* byte);
	HRESULT	Save_VertexBuffer_Anim(HANDLE hHandle, DWORD* byte);

	HRESULT	Load_VertexBuffer_NonAnim(_fmatrix PreTransformMatrix, HANDLE hHandle, DWORD* byte);
	HRESULT	Load_VertexBuffer_Anim(const CModel* pModel, HANDLE hHandle, DWORD* byte);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, _bool bTexture);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, HANDLE hHandle, DWORD* byte, _fmatrix PreTransformMatrix, _bool bTexture);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END