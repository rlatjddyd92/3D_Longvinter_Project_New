#include "..\Public\Mesh.h"

#include "Model.h"
#include "Shader.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CMesh::CMesh(const CMesh& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	HRESULT hr = pModel->Get_ModelType() == CModel::TYPE_NONANIM ? Ready_VertexBuffer_NonAnim(PreTransformMatrix, pAIMesh) : Ready_VertexBuffer_Anim(pModel, pAIMesh);
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER
	/* �ε��������� ������ ä���ְ� */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iIndexStride;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };
	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		aiFace		AIFace = pAIMesh->mFaces[i];

		pIndices[iNumIndices++] = AIFace.mIndices[0];
		pIndices[iNumIndices++] = AIFace.mIndices[1];
		pIndices[iNumIndices++] = AIFace.mIndices[2];
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	m_iIndices_Origin = new _uint[m_iNumIndices];
	memcpy(m_iIndices_Origin, pIndices, sizeof(_uint) * m_iNumIndices);

	/* �ε������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{


	return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(const CModel* pModel, CShader* pShader, const _char* pConstantName)
{
	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * g_iMaxMeshBones);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&m_BoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * pModel->Get_BoneCombindTransformationMatrix(m_BoneIndices[i]));
	}

	return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, m_iNumBones);
}

void CMesh::SaveModel(HANDLE hHandle, DWORD* byte)
{
	for (_uint i = 0; i < 260; ++i)
		WriteFile(hHandle, &m_szName[i], sizeof(_char), byte, nullptr);

	WriteFile(hHandle, &m_iMaterialIndex, sizeof(_uint), byte, nullptr);
	WriteFile(hHandle, &m_iNumVertices, sizeof(_uint), byte, nullptr);
	WriteFile(hHandle, &m_iNumIndices, sizeof(_uint), byte, nullptr);

	if (eType = CModel::TYPE::TYPE_NONANIM)
		Save_VertexBuffer_NonAnim(hHandle, byte);
	else 
		Save_VertexBuffer_Anim(hHandle, byte);

	for (size_t i = 0; i < m_iNumIndices; ++i)
		WriteFile(hHandle, &m_iIndices_Origin[i], sizeof(_uint), byte, nullptr);
}

HRESULT CMesh::LoadModel(HANDLE hHandle, DWORD* byte, const CModel* pModel, _fmatrix PreTransformMatrix)
{
	for (_uint i = 0; i < 260; ++i)
		ReadFile(hHandle, &m_szName[i], sizeof(_char), byte, nullptr);

	ReadFile(hHandle, &m_iMaterialIndex, sizeof(_uint), byte, nullptr);
	m_iNumVertexBuffers = 1;
	ReadFile(hHandle, &m_iNumVertices, sizeof(_uint), byte, nullptr);
	ReadFile(hHandle, &m_iNumIndices, sizeof(_uint), byte, nullptr);
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER

	HRESULT hr = pModel->Get_ModelType() == CModel::TYPE_NONANIM ? Load_VertexBuffer_NonAnim(PreTransformMatrix, hHandle, byte) : Load_VertexBuffer_Anim(pModel, hHandle, byte);
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER
	/* �ε��������� ������ ä���ְ� */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iIndexStride;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);
	m_iIndices_Origin = new _uint[m_iNumIndices];
	ZeroMemory(m_iIndices_Origin, sizeof(_uint) * m_iNumIndices);

	for (size_t i = 0; i < m_iNumIndices; ++i)
	{
		ReadFile(hHandle, &m_iIndices_Origin[i], sizeof(_uint), byte, nullptr);
		pIndices[i] = m_iIndices_Origin[i];
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	/* �ε������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_NonAnim(_fmatrix PreTransformMatrix, const aiMesh* pAIMesh)
{
	eType = CModel::TYPE::TYPE_NONANIM;

	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	vecOrigin.resize(m_iNumVertices);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
			m_vecOriginPosition.push_back(pVertices[i].vPosition);
			XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

			memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
			memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
			memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));

			vecOrigin[i].vPosition = { pVertices[i].vPosition.x, pVertices[i].vPosition.y,pVertices[i].vPosition.z };
			vecOrigin[i].vNormal = { pVertices[i].vNormal.x, pVertices[i].vNormal.y,pVertices[i].vNormal.z };
			vecOrigin[i].vTexcoord = { pVertices[i].vTexcoord.x, pVertices[i].vTexcoord.y,};
			vecOrigin[i].vTangent = { pVertices[i].vTangent.x, pVertices[i].vTangent.y,pVertices[i].vTangent.z };
		}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_VertexBuffer_Anim(const CModel* pModel, const aiMesh* pAIMesh)
{
	eType = CModel::TYPE::TYPE_ANIM;

	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	vecOrigin.resize(m_iNumVertices);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
			vecOrigin[i].vPosition = { pVertices[i].vPosition.x, pVertices[i].vPosition.y,pVertices[i].vPosition.z };
			memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
			memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
			memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));

			vecOrigin[i].vNormal = { pVertices[i].vNormal.x, pVertices[i].vNormal.y,pVertices[i].vNormal.z };
			vecOrigin[i].vTexcoord = { pVertices[i].vTexcoord.x, pVertices[i].vTexcoord.y, };
			vecOrigin[i].vTangent = { pVertices[i].vTangent.x, pVertices[i].vTangent.y,pVertices[i].vTangent.z };
		}


	/* �� ��ü�� ���� �ƴ� �� �޽��ϳ��� ������ �ִ� ���� ����. */
		m_iNumBones = pAIMesh->mNumBones;
	

	m_OffsetMatrices.resize(m_iNumBones);
	m_OffsetMatrices_Origin.resize(m_iNumBones);
	m_vecNumWeights.resize(m_iNumBones);

	m_vecVerTexId_Origin.resize(m_iNumBones);
	m_vecWeight_Origin.resize(m_iNumBones);

		for (size_t i = 0; i < m_iNumBones; i++)
		{
			/* �� �޽ð� ����ϴ� ������ ����. */
			aiBone* pAIBone = pAIMesh->mBones[i];

			/* pAIBone->mOffsetMatrix : �������� ���� �ϳ��� ���븦 �����ϴ� ���, ���� ���¿� ���� ��Ű��(������ ��ġ * �����)�۾� ��, �߰����� ������ �ʿ��� �� �ִ�.  */
			memcpy(&m_OffsetMatrices[i], &pAIBone->mOffsetMatrix, sizeof(_float4x4));
			m_OffsetMatrices_Origin[i] = m_OffsetMatrices[i];
			XMStoreFloat4x4(&m_OffsetMatrices[i], XMMatrixTranspose(XMLoadFloat4x4(&m_OffsetMatrices[i])));

			/* �� ���� ��� �ִ� ��ü �����߿� �� �޽ð� ����ϴ� �����̸��� ���� ���� �ε����� �����صд�. */
			m_BoneIndices.emplace_back(pModel->Get_BoneIndex(pAIBone->mName.data));

			/*pAIBone->mName*/
			m_vecNumWeights[i] = pAIBone->mNumWeights;
			/* �� ���� �(mNumWeights) �������� ������ �ִ°�? */

			m_vecVerTexId_Origin[i].resize(m_vecNumWeights[i]);
			m_vecWeight_Origin[i].resize(m_vecNumWeights[i]);

			for (size_t j = 0; j < pAIBone->mNumWeights; j++)
			{
				m_vecVerTexId_Origin[i][j] = pAIBone->mWeights[j].mVertexId;
				m_vecWeight_Origin[i][j] = pAIBone->mWeights[j].mWeight;

				/* �� ���� ������ �ִ� j��° ������ �ε���(pAIBone->mWeights[j].mVertexId)*/
				if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.x = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.x = pAIBone->mWeights[j].mWeight;
				}

				else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.y = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.y = pAIBone->mWeights[j].mWeight;
				}

				else if (0.f == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.z = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.z = pAIBone->mWeights[j].mWeight;
				}

				else
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndices.w = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeights.w = pAIBone->mWeights[j].mWeight;
				}
			}
		}
	

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.emplace_back(pModel->Get_BoneIndex(m_szName));

		_float4x4	OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		m_OffsetMatrices.emplace_back(OffsetMatrix);
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Save_VertexBuffer_NonAnim(HANDLE hHandle, DWORD* byte)
{
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		WriteFile(hHandle, &vecOrigin[i].vPosition, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vNormal, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vTexcoord, sizeof(_float2), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vTangent, sizeof(_float3), byte, nullptr);
	}

	

	return S_OK;
}

HRESULT CMesh::Save_VertexBuffer_Anim(HANDLE hHandle, DWORD* byte)
{
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		WriteFile(hHandle, &vecOrigin[i].vPosition, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vNormal, sizeof(_float3), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vTexcoord, sizeof(_float2), byte, nullptr);
		WriteFile(hHandle, &vecOrigin[i].vTangent, sizeof(_float3), byte, nullptr);
	}

	WriteFile(hHandle, &m_iNumBones, sizeof(_uint), byte, nullptr);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* pAIBone->mOffsetMatrix : �������� ���� �ϳ��� ���븦 �����ϴ� ���, ���� ���¿� ���� ��Ű��(������ ��ġ * �����)�۾� ��, �߰����� ������ �ʿ��� �� �ִ�.  */
		WriteFile(hHandle, &m_OffsetMatrices_Origin[i], sizeof(_float4x4), byte, nullptr);

		/* �� ���� ��� �ִ� ��ü �����߿� �� �޽ð� ����ϴ� �����̸��� ���� ���� �ε����� �����صд�. */
		WriteFile(hHandle, &m_BoneIndices[i], sizeof(_uint), byte, nullptr);

		/*pAIBone->mName*/
		WriteFile(hHandle, &m_vecNumWeights[i], sizeof(_uint), byte, nullptr);

		/* �� ���� �(mNumWeights) �������� ������ �ִ°�? */

		for (size_t j = 0; j < m_vecNumWeights[i]; j++)
		{
			WriteFile(hHandle, &m_vecVerTexId_Origin[i][j], sizeof(_int), byte, nullptr);
			WriteFile(hHandle, &m_vecWeight_Origin[i][j], sizeof(_float), byte, nullptr);
		}
	}
	return S_OK;
}

HRESULT CMesh::Load_VertexBuffer_NonAnim(_fmatrix PreTransformMatrix, HANDLE hHandle, DWORD* byte)
{
	m_iVertexStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	vecOrigin.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		ReadFile(hHandle, &pVertices[i].vPosition, sizeof(XMFLOAT3), byte, nullptr);
		vecOrigin[i].vPosition = { pVertices[i].vPosition.x, pVertices[i].vPosition.y,pVertices[i].vPosition.z };
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		ReadFile(hHandle, &pVertices[i].vNormal, sizeof(XMFLOAT3), byte, nullptr);
		ReadFile(hHandle, &pVertices[i].vTexcoord, sizeof(XMFLOAT2), byte, nullptr);
		ReadFile(hHandle, &pVertices[i].vTangent, sizeof(XMFLOAT3), byte, nullptr);

		vecOrigin[i].vNormal = { pVertices[i].vNormal.x, pVertices[i].vNormal.y,pVertices[i].vNormal.z };
		vecOrigin[i].vTexcoord = { pVertices[i].vTexcoord.x, pVertices[i].vTexcoord.y, };
		vecOrigin[i].vTangent = { pVertices[i].vTangent.x, pVertices[i].vTangent.y,pVertices[i].vTangent.z };
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Load_VertexBuffer_Anim(const CModel* pModel, HANDLE hHandle, DWORD* byte)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	vecOrigin.resize(m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		ReadFile(hHandle, &pVertices[i].vPosition, sizeof(XMFLOAT3), byte, nullptr);
		vecOrigin[i].vPosition = { pVertices[i].vPosition.x, pVertices[i].vPosition.y,pVertices[i].vPosition.z };
		ReadFile(hHandle, &pVertices[i].vNormal, sizeof(XMFLOAT3), byte, nullptr);
		ReadFile(hHandle, &pVertices[i].vTexcoord, sizeof(XMFLOAT2), byte, nullptr);
		ReadFile(hHandle, &pVertices[i].vTangent, sizeof(XMFLOAT3), byte, nullptr);

		vecOrigin[i].vNormal = { pVertices[i].vNormal.x, pVertices[i].vNormal.y,pVertices[i].vNormal.z };
		vecOrigin[i].vTexcoord = { pVertices[i].vTexcoord.x, pVertices[i].vTexcoord.y, };
		vecOrigin[i].vTangent = { pVertices[i].vTangent.x, pVertices[i].vTangent.y,pVertices[i].vTangent.z };
	}

	/* �� ��ü�� ���� �ƴ� �� �޽��ϳ��� ������ �ִ� ���� ����. */
	ReadFile(hHandle, &m_iNumBones, sizeof(_uint), byte, nullptr);

	m_OffsetMatrices.resize(m_iNumBones);
	m_OffsetMatrices_Origin.resize(m_iNumBones);
	m_vecNumWeights.resize(m_iNumBones);
	m_vecVerTexId_Origin.resize(m_iNumBones);
	m_vecWeight_Origin.resize(m_iNumBones);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* pAIBone->mOffsetMatrix : �������� ���� �ϳ��� ���븦 �����ϴ� ���, ���� ���¿� ���� ��Ű��(������ ��ġ * �����)�۾� ��, �߰����� ������ �ʿ��� �� �ִ�.  */
		ReadFile(hHandle, &m_OffsetMatrices[i], sizeof(_float4x4), byte, nullptr);
		m_OffsetMatrices_Origin[i] = m_OffsetMatrices[i];
		XMStoreFloat4x4(&m_OffsetMatrices[i], XMMatrixTranspose(XMLoadFloat4x4(&m_OffsetMatrices[i])));

		/* �� ���� ��� �ִ� ��ü �����߿� �� �޽ð� ����ϴ� �����̸��� ���� ���� �ε����� �����صд�. */
		_uint iBoneIndices = 0;
		ReadFile(hHandle, &iBoneIndices, sizeof(_uint), byte, nullptr);
		m_BoneIndices.emplace_back(iBoneIndices);

		/*pAIBone->mName*/
		ReadFile(hHandle, &m_vecNumWeights[i], sizeof(_uint), byte, nullptr);

		/* �� ���� �(mNumWeights) �������� ������ �ִ°�? */

		m_vecVerTexId_Origin[i].resize(m_vecNumWeights[i]);
		m_vecWeight_Origin[i].resize(m_vecNumWeights[i]);

		for (size_t j = 0; j < m_vecNumWeights[i]; j++)
		{
			ReadFile(hHandle, &m_vecVerTexId_Origin[i][j], sizeof(_int), byte, nullptr);
			ReadFile(hHandle, &m_vecWeight_Origin[i][j], sizeof(_float), byte, nullptr);

			/* �� ���� ������ �ִ� j��° ������ �ε���(pAIBone->mWeights[j].mVertexId)*/
			if (0.f == pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.x)
			{
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendIndices.x = i;
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.x = m_vecWeight_Origin[i][j];
			}

			else if (0.f == pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.y)
			{
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendIndices.y = i;
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.y = m_vecWeight_Origin[i][j];
			}

			else if (0.f == pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.z)
			{
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendIndices.z = i;
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.z = m_vecWeight_Origin[i][j];
			}

			else
			{
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendIndices.w = i;
				pVertices[m_vecVerTexId_Origin[i][j]].vBlendWeights.w = m_vecWeight_Origin[i][j];
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_BoneIndices.emplace_back(pModel->Get_BoneIndex(m_szName));

		_float4x4	OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		m_OffsetMatrices.emplace_back(OffsetMatrix);
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModel, pAIMesh, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, HANDLE hHandle, DWORD* byte, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->LoadModel(hHandle, byte, pModel, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();
	m_vecOriginPosition.clear();
	m_BoneIndices.clear();
	m_OffsetMatrices.clear();
	m_vecNumWeights.clear();
	Safe_Delete_Array(m_iIndices_Origin);
	m_OffsetMatrices_Origin.clear();
	m_vecOriginPosition.clear();
	m_vecVerTexId_Origin.clear();
	m_vecWeight_Origin.clear();
	Safe_Delete_Array(m_Vertices_Origin);
	Safe_Delete_Array(m_Vertices_Origin_Anim);
}
