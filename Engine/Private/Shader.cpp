#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CShader::CShader(const CShader& Prototype)
	: CComponent{ Prototype }
	, m_pEffect{ Prototype.m_pEffect }
	, m_iNumPasses{ Prototype.m_iNumPasses }
	, m_InputLayouts{ Prototype.m_InputLayouts }
{
	Safe_AddRef(m_pEffect);

	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uint iNumElements)
{
	_uint			iHlslFlag = { 0 };

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob* pErrorMsg = { nullptr };

	/* ���� �ۼ��� ���̴� ������ �����Ͽ� ��üȭ(ID3DX11Effect*) �ߴ�. */
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc{};

	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	for (size_t i = 0; i < m_iNumPasses; i++)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(0);
		if (nullptr == pPass)
			return E_FAIL;

		D3DX11_PASS_DESC	PassDesc{};

		pPass->GetDesc(&PassDesc);

		/*
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format;
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/



		/* D3D11_INPUT_ELEMENT_DESC : ������ �����ϴ� ������� �ϳ��� ������ ǥ���ϴ� ����ü .*/

		//D3D11_INPUT_ELEMENT_DESC	ElementDesc[] =
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};

		ID3D11InputLayout* pInputLayout;

		if (FAILED(m_pDevice->CreateInputLayout(pElementDesc, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.emplace_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
	if (nullptr == pPass)
		return E_FAIL;

	pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	///* Ư�� �����鿡 ���ؼ��� ������ �ʰų� ��ȯ������ ������ �����Ѵ�.  */
	//pVariable->SetRawValue(void* , 0, �ѱ�����ϴ� �������� ũ��.);	

	/* ���� ���������� ����̾��ٸ� ��� Ÿ�Կ� ���� �ڵ��� �ٽ� ���´�. */
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	///* Ư�� �����鿡 ���ؼ��� ������ �ʰų� ��ȯ������ ������ �����Ѵ�.  */
	//pVariable->SetRawValue(void* , 0, �ѱ�����ϴ� �������� ũ��.);	

	/* ���� ���������� ����̾��ٸ� ��� Ÿ�Կ� ���� �ڵ��� �ٽ� ���´�. */
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumSRVs)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResourceArray(ppSRVs, 0, iNumSRVs);
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Bind_ChangeColor(const _char* pConstantName_IsChange, const _char* pConstantName_fRGB, _bool* bChange, _float* pRGB)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName_IsChange);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectScalarVariable* pIsChange = pVariable->AsScalar();
	if (nullptr == pIsChange)
		return E_FAIL;

	if (FAILED(pIsChange->SetBoolArray(bChange, 0, 3)))
		return E_FAIL;

	ID3DX11EffectVariable* pVariableRGB = m_pEffect->GetVariableByName(pConstantName_fRGB);
	if (nullptr == pVariableRGB)
		return E_FAIL;

	ID3DX11EffectScalarVariable* pfRGB = pVariableRGB->AsScalar();
	if (nullptr == pfRGB)
		return E_FAIL;

	if (FAILED(pfRGB->SetFloatArray((float*)pRGB, 0, 3)))
		return E_FAIL;

	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElementDesc, iNumElements)))
	{
		MSG_BOX(TEXT("Failed to Created : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CShader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CShader::Free()
{
	__super::Free();

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);
	m_InputLayouts.clear();

	Safe_Release(m_pEffect);


}
