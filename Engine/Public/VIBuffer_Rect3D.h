#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect3D final : public CVIBuffer
{
private:
	CVIBuffer_Rect3D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect3D(const CVIBuffer_Rect3D& Prototype);
	virtual ~CVIBuffer_Rect3D() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual _bool isPicking(const _float4x4& WorldMatrix, _float3* pOut) override;

public:
	static CVIBuffer_Rect3D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END