#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Button final : public CUIPart
{
public:
	enum UIBUTTON_TYPE
	{
		BUTTON_INGAME,
		BUTTON_EDITOR,
		BUTTON_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UIBUTTON_TYPE eType = UIBUTTON_TYPE::BUTTON_END;
	} UIButton_DESC;


private:
	CUIPart_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Button(const CUIPart_Button& Prototype);
	virtual ~CUIPart_Button() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	UIBUTTON_TYPE m_eType = UIBUTTON_TYPE::BUTTON_END;

private:
	HRESULT Ready_Components();



public:
	static CUIPart_Button* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END