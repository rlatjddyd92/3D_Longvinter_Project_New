#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CSurFace final : public CGameObject
{
private:
	CSurFace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSurFace(const CSurFace& Prototype);
	virtual ~CSurFace() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_int GetTextureIndex() { return m_iTextureIndex; }
	void SetTextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }
	void RemoveSurFace() { __super::SetDead(); }
	void SetPosition(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec, CTexture* ManagerTexture);
	_float3 IsPicking();


public:
	void SetTransForm(CTransform::STATE eState, XMVECTOR vecNew);

private:
	HRESULT Ready_Components();
	class CShader* m_pShaderCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CTexture* m_pManagerTexture = { nullptr };
	_int m_iTextureIndex = 0;

public:
	static CSurFace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END