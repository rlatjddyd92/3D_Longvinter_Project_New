#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Model.h"

BEGIN(Engine)
class CCollider;
class CShader;
END

BEGIN(Client)

class CTool : public CPartObject
{
public:
	typedef struct : public CPartObject::PARTOBJ_DESC
	{
		const _uint* pParentState = { nullptr };
		const _float4x4* pSocketBoneMatrix = { nullptr };
	}TOOL_DESC;

protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool(const CTool& Prototype);
	virtual ~CTool() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetParentState(_uint* iParent) { m_pParentState = iParent; }
	void SetSocketMatrix(const _float4x4* Matrix) { m_pSocketMatrix = Matrix; }
	void SetRenderMatrix(_float4x4& Matrix) { memcpy(&m_RenderMatrix, &Matrix, sizeof(_float4x4)); }
	void SetModelTexture(_int iMeterial, _int iType, CTexture* pTexture) 
	{ 
		if (m_pModelCom != nullptr)
			m_pModelCom->SetTexture(iMeterial, iType, pTexture);
	}

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

protected:
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _uint* m_pParentState = { nullptr };

	_bool m_bEmptyPart = false;

	_float4x4 m_RenderMatrix{};

protected:
	HRESULT Ready_Components();

public:
	static CTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END