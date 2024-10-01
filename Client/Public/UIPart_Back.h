#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Back final : public CUIPart
{
public:
	enum UIBACK_TYPE
	{
		BACK_DEFAULT,
		BACK_INGAME_WINDOW, // <- 인게임 기본 윈도우
		BACK_INGAME_WINDOW_HEADER, // <- 인게임 윈도우 머리 부분
		BACK_INGAME_TOOLTIP, // 툴팁용 윈도우
		BACK_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UIBACK_TYPE eType = UIBACK_TYPE::BACK_END;
	} UIBack_DESC;


private:
	CUIPart_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Back(const CUIPart_Back& Prototype);
	virtual ~CUIPart_Back() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	

protected:
	UIBACK_TYPE m_eType = UIBACK_TYPE::BACK_END;

private:
	HRESULT Ready_Components();

public:
	static CUIPart_Back* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END