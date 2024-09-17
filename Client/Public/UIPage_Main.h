#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_Main : public CUIPage
{
protected:
	CUIPage_Main(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Main(const CUIPage_Main& Prototype);
	virtual ~CUIPage_Main() = default;

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
	virtual _bool Check_Cursor() override;

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

private: // <- 보유한 UI Part 목록

	CUIPart_Button* m_pButton_Ingame = { nullptr };
	CUIPart_Button* m_pButton_Editor = { nullptr };
	CUIPart_Picture* m_pPicture_BGA = { nullptr };
	CUIPart_Picture* m_pPicture_MainLogo = { nullptr };


public:
	static CUIPage_Main* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END