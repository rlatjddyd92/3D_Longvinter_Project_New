#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_ToolTip : public CUIPage
{
protected:
	CUIPage_ToolTip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_ToolTip(const CUIPage_ToolTip& Prototype);
	virtual ~CUIPage_ToolTip() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void AddRender_UIPage() override;
	virtual void Ready_UIPart() override;

	virtual _bool Key_Action() override;


public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

	CUIPart_Back* m_pBack_Window = { nullptr };

private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;

public:
	static CUIPage_ToolTip* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END