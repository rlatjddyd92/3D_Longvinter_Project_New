#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_Test : public CUIPage
{
protected:
	CUIPage_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Test(const CUIPage_Test& Prototype);
	virtual ~CUIPage_Test() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void AddRender_UIPage_Test();
	virtual void Ready_UIPart();

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();



public:
	static CUIPage_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END