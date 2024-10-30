#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_Shop : public CUIPage
{
protected:
	CUIPage_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Shop(const CUIPage_Shop& Prototype);
	virtual ~CUIPage_Shop() = default;

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

	void OpenShop(_int iShopNum)
	{
		__super::SetOn();
		m_iNowShopNum = iShopNum;
	}

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

	CUIPart_Button* m_pButton_Close = { nullptr };
	CUIPart_Back* m_pBack_Window = { nullptr };
	CUIPart_Back* m_pBack_Window_Header = { nullptr };

private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;

	_int m_iNowShopNum = -1;

public:
	static CUIPage_Shop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END