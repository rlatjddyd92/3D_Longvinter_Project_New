#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "../Default/UIPart_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage : public CUIObject
{
protected:
	CUIPage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage(const CUIPage& Prototype);
	virtual ~CUIPage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void AddRender_UIPage();
	virtual void Ready_UIPart();

	virtual _bool Key_Action();
	
	_bool IsOnCursor() { return m_bOnCursor; }
	_bool IsPushed() { return m_bPushed[1]; }
	_bool IsPressing() { return m_bPushed[0] * m_bPushed[1]; }

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

protected:
	_bool m_bOnCursor = false; // <- 마우스 위치와 겹침 
	_bool m_bPushed[2] = { false,false }; // <- 마우스, 키로 작동시킴 [0] : 이전 프레임, [1] : 현재 프레임 <- 둘 다 true인 경우 Pressing


public:
	static CUIPage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END