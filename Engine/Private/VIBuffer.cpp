#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & Prototype)
	: CComponent{ Prototype }
	, m_pVB { Prototype.m_pVB }
	, m_pIB { Prototype.m_pIB }
	, m_iNumVertexBuffers { Prototype.m_iNumVertexBuffers }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_iNumIndices { Prototype.m_iNumIndices }
	, m_iIndexStride { Prototype.m_iIndexStride }
	, m_eIndexFormat { Prototype.m_eIndexFormat }
	, m_eTopology { Prototype.m_eTopology }
	, m_pVerticesPos{ Prototype.m_pVerticesPos }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);	
}

HRESULT CVIBuffer::Initialize_Prototype()
{	
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Bind_Buffers()
{
	/* ������ �ε����� ���� ���� �׷����������� �ʿ��� ������ ��ġ�� �Է½��ѳ��´�. */

	ID3D11Buffer*		pVertexBuffers[] = {
		m_pVB, 		
		/*m_pVBInstance, */
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride,
		/*m_pVBInstance,*/
	};

	_uint		iOffsets[] = {
		0,
		/*0,*/
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* �� ������ ��� ������� ������ �־��. */
	// m_pContext->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	/* 1. ������ �ε����� ���ؼ� ���� �׸������ؼ��� ��ġ�� ���������ִ� ��(x) -> �츮�� ���� ��ġ���� ��ȯ, �븻�� ������, �ؽ����� ���ø�. (Shader)*/
	/* 2. ��ġ�� ���� �����ϴ� ������ɷ��������������λ������ ��������� ������������������ ���ؼ� �׷����ϤѤ��� ��Ȳ. (Shader) */
	/* 3. �̷��� ���� �ۼ��� ���̴��� ���� ���� �׸������ϴ� ������ �� �޾��� �� �ִ� ��Ȳ�ΰ�? �ľ��ؾ��� �ʿ䰡 �ִ�. */
	/* ���� �׸������ϴ� ������ ������ �̿��Ϸ����ϴ� ���̴��� �Է��� �����Ѱ�? */

	//ID3D11InputLayout*			pILO;
	//m_pDevice->CreateInputLayout(
	//	�� ������ �������������� �־��, 
	//	�� ������ ��� ��������������� �־��,
	//	���� ����Ϸ����ϴ� ���̴��� ���� �Է� ���� ����, 
	//	���̴����� �ް����ϴ� ������ ũ��, 
	//	&pILO);

	//m_pContext->IASetInputLayout(pILO);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer ** ppOut)
{
	/* D3D11_BUFFER_DESC : �����ϰ����ϴ� ������ �Ӽ�(ũ�⸦ �����Ʈ��?, ���۸� ����? ����?) �� �����Ѵ�. */
	/* D3D11_SUBRESOURCE_DATA : ����� void* �� ������ �ִ�. void*�� ���� �����ϰ����� �����ۿ� ���� ��� ���� �̸��Ҵ��Ͽ� ä���, �Ҵ��� ������ �ָּ� �������ֳ�.  */
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppOut);
}

void CVIBuffer::Free()
{
	__super::Free();
	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
