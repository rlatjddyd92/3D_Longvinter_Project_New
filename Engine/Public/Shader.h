#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);

public:
	/* �� ���̴��� ����Ǿ��ִ� Ư�� ���������� ���� ������. */
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumSRVs);
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength);
	HRESULT Bind_ChangeColor(const _char* pConstantName_IsChange, const _char* pConstantName_fRGB, _bool* bChange, _float* pRGB);
	HRESULT Bind_ChangeAlpah(const _char* pConstantName_IsChange, const _char* pConstantName_fAlpah, _bool* bChange, _float* pAlpah);

private:
	ID3DX11Effect* m_pEffect = { nullptr };
	_uint						m_iNumPasses = { 0 };
	vector<ID3D11InputLayout*>	m_InputLayouts;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElementDesc, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END