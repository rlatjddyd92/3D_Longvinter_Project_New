#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_TextBox final : public CUIPart
{
public:
	enum UITEXTBOX_TYPE
	{
		TEXTBOX_NORMAL,
		TEXTBOX_NOTICE,
		TEXTBOX_SELECT,
		TEXTBOX_INTER_NAME,
		TEXTBOX_INTER_FUNCTION,
		TEXTBOX_CASH,
		TEXTBOX_3D,
		TEXTBOX_SCRIPT,
		TEXTBOX_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UITEXTBOX_TYPE eType = UITEXTBOX_TYPE::TEXTBOX_END;
		_bool bCenter = false;
		_bool AutoRemove = false;
		CLongvinter_Container* pHost = { nullptr };
	} UITextBox_DESC;



private:
	CUIPart_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_TextBox(const CUIPart_TextBox& Prototype);
	virtual ~CUIPart_TextBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetText(wstring Text)
	{ 
		m_Text = Text;
	}

	void SetCash(_int iCash) { m_iCash = iCash; }

	void SetFont(wstring Font) 
	{
		m_Font = Font;
	}
	void SetSize(_float fSize) { m_fSize = fSize; }
	void SetCenter(_bool bCenter) { m_bCenter = bCenter; }
	void SetColor(XMVECTOR vColor) { XMStoreFloat4(&m_fTextColor, vColor); }

protected:
	UITEXTBOX_TYPE m_eType = UITEXTBOX_TYPE::TEXTBOX_END;
	wstring m_Text = TEXT("none");

	_int m_iCash = 0; // <- TextCash Àü¿ë 

	_float m_fSize = 1.f;
	wstring m_Font = TEXT("Font_Test1");

	_bool m_bCenter = false;

	_float4 m_fTextColor = { 1.f,1.f,1.f,1.f };

	_bool m_bAutoRemove = false;

	CLongvinter_Container* m_pHost = { nullptr };

private:
	HRESULT Ready_Components();



public:
	static CUIPart_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END