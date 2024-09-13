#include "..\Public\VIBuffer_Rect.h"
#include "GameInstance.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 4;
	m_pVerticesPos = new _float3[m_iNumVertices];
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumIndices = 6;
	m_iIndexStride = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* �������۷� �����Ѵ�. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	

	/* �������ۿ� ä���� ������ ��������ؼ� �ӽ������� ������ �Ҵ��Ѵ�. */
	VTXPOSTEX*			pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	/* �������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

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

	_ushort*		pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	/* �ε������۸� �����Ѵ�. */
	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer_Rect::isPicking(const _float4x4& WorldMatrix, _float3* pOut)
{
	m_pGameInstance->Transform_MouseRay_ToLocalSpace(WorldMatrix);

	if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[0],
		m_pVerticesPos[1],
		m_pVerticesPos[2],
		pOut))
		goto Compute_WorldPos;


	/* ���� �Ʒ� �ﰢ�� */
	if (true == m_pGameInstance->isPicked_InLocalSpace(m_pVerticesPos[0],
		m_pVerticesPos[2],
		m_pVerticesPos[3],
		pOut))
		goto Compute_WorldPos;

	return false;

Compute_WorldPos:
	XMVECTOR TempVec = XMLoadFloat3(pOut);
	XMMATRIX TempMat = XMLoadFloat4x4(&WorldMatrix);

	TempVec = XMVector3TransformCoord(TempVec, TempMat);
	*pOut = { TempVec.m128_f32[0],TempVec.m128_f32[1] ,TempVec.m128_f32[2] };

	return true;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Rect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();

}
