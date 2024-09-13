#include "..\Public\VIBuffer_Terrain.h"
#include "GameInstance.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & Prototype)
	: CVIBuffer{ Prototype }
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }

{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong			dwByte = {};

	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER			fh{};
	BITMAPINFOHEADER			ih{};

	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	_uint*						pPixel = new _uint[m_iNumVerticesX * m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_uint) * m_iNumVerticesX * m_iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);	

	m_iNumVertexBuffers = 1;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
 	m_pVerticesPos = new _float3[m_iNumVertices];
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
#pragma region VERTEX_BUFFER
	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3(j, (pPixel[iIndex] & 0x000000ff) / 10.0f, i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}
#pragma endregion

#pragma region INDEX_BUFFER
	
	_uint*		pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint			iIndex = i * m_iNumVerticesX + j;
			
			_uint			iIndices[] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			
			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	for (size_t i = 0; i < m_iNumVertices; i++)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));	
#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	/* �ε��������� ������ ä���ְ� */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iIndexStride;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	/* �ε������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pPixel);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer_Terrain::isPicking(const _float4x4& WorldMatrix, _float3* pOut)
{
	/* ���콺 ���̿� ���������� ���÷� ������. */
	m_pGameInstance->Transform_MouseRay_ToLocalSpace(WorldMatrix);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX, /* ����*/
				iIndex + m_iNumVerticesX + 1, /* ���� */
				iIndex + 1, /* ���� */
				iIndex /* �޾� */
			};

			/* ������ �� �ﰢ�� */
			if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[iIndices[0]],
				m_pVerticesPos[iIndices[1]],
				m_pVerticesPos[iIndices[2]],
				pOut))
				goto Compute_WorldPos;


			/* ���� �Ʒ� �ﰢ�� */
			if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[iIndices[0]],
				m_pVerticesPos[iIndices[2]],
				m_pVerticesPos[iIndices[3]],
				pOut))
				goto Compute_WorldPos;
		}
	}
	return false;

Compute_WorldPos:
	XMVECTOR TempVec = XMLoadFloat3(pOut);
	XMMATRIX TempMat = XMLoadFloat4x4(&WorldMatrix);

	TempVec = XMVector3TransformCoord(TempVec, TempMat);
	*pOut = { TempVec.m128_f32[0],TempVec.m128_f32[1] ,TempVec.m128_f32[2] };

	return true;
}

_float CVIBuffer_Terrain::Compute_Height(const _float3& vLocalPos)
{
	/* ��ü�� �����ϴ� �׸� ������ ���� �ϴ� �ε����� ���ߴ�. */
	_uint		iIndex = _uint(vLocalPos.z) * m_iNumVerticesX + _uint(vLocalPos.x);

	/* �����ϴ��̆����� �������� �׹����� �������� �ΉK������ ������.  */
	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX, /* ����*/
		iIndex + m_iNumVerticesX + 1, /* ���� */
		iIndex + 1, /* ���� */
		iIndex /* �޾� */
	};

	_float		fWidth = vLocalPos.x - m_pVerticesPos[iIndices[0]].x;
	_float		fDepth = m_pVerticesPos[iIndices[0]].z - vLocalPos.z;

	XMVECTOR TempVec[4] = { XMLoadFloat3(&m_pVerticesPos[iIndices[0]]),XMLoadFloat3(&m_pVerticesPos[iIndices[1]]),XMLoadFloat3(&m_pVerticesPos[iIndices[2]]), XMLoadFloat3(&m_pVerticesPos[iIndices[3]]) };

	XMVECTOR TempPlane = {0,};

	/* ������ �� �ﰢ���ȿ��־�. */
	if (fWidth > fDepth)
	{
		TempPlane = XMPlaneFromPoints(TempVec[0], TempVec[1], TempVec[2]);
	}
	/* ���� �Ʒ� �ﰢ���ȿ��־�. */
	else
	{
		TempPlane = XMPlaneFromPoints(TempVec[0], TempVec[2], TempVec[3]);
	}

	/* ax + by + cz + d */
	/* y = (-ax - cz - d) / b */

	return (-TempPlane.m128_f32[0] * vLocalPos.x - TempPlane.m128_f32[2] * vLocalPos.z - TempPlane.m128_f32[3]) / TempPlane.m128_f32[1];
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

}
