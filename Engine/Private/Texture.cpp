#include "..\Public\Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{

}

CTexture::CTexture(const CTexture & Prototype)
	: CComponent { Prototype }
	, m_iNumTextures { Prototype.m_iNumTextures }
	, m_SRVs { Prototype.m_SRVs }
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar * pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	m_SRVs.reserve(iNumTextures);

	_tchar			szFullPath[MAX_PATH] = TEXT("");
	_tchar			szExt[MAX_PATH] = TEXT("");

	/* D:\������\145\3D\Framework\Client\Bin\Resources\Textures\11.dds */
	/* ..\Bin\Resources\Textures\11.dds */
	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	for (size_t i = 0; i < m_iNumTextures; i++)
	{
		wsprintf(szFullPath, pTextureFilePath, i);

		ID3D11ShaderResourceView* pSRV = { nullptr };

		HRESULT		hr = { 0 };

		/* ���� �ؽ��ĸ� �ε��� �� ID3D11Texture2D�� �޾ƿ��� �ʴ´�?! */
		/* ID3D11Texture2D�� �� �� �ִ� ��?  */
		/* 1. : ���� ����� �� �ִ� �ؽ���(View)�� ������. */
		/* 2. : �ؽ����� ������ ���� �����ϱ����ؼ� Lock, unLock(Map, unmap)�� �����Ѵ�. */
		/* 3. : ���� ���Ϸ� �����Ѵ�. */

		/* ���̴��� �ø��� ���ø��ϱ����� �ؽ��ĵ�!!! */

		if (false == lstrcmp(TEXT(".dds"), szExt))			
		{
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}
		else if (false == lstrcmp(TEXT(".tga"), szExt))
			return E_FAIL;

		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);
		}

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.emplace_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShadeResource(CShader * pShader, const _char * pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);	
}

HRESULT CTexture::Bind_ShadeResources(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRVs(pConstantName, &m_SRVs.front(), m_iNumTextures);
}

CTexture * CTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CTexture*		pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent * CTexture::Clone(void * pArg)
{
	CTexture*		pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
