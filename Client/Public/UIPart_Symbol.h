#pragma once

#include "Client_Defines.h"
#include "UIPart.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Symbol final : public CUIPart
{
public:
	enum UISYMBOL_TYPE
	{
		SYMBOL_STUN,
		SYMBOL_PANIC,
		SYMBOL_SEARCHING,
		SYMBOL_QUEST,
		SYMBOL_FIGHT,
		SYMBOL_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UISYMBOL_TYPE eType = UISYMBOL_TYPE::SYMBOL_END;
		CLongvinter_Container* pHost = { nullptr };
	} UISymbol_DESC;



private:
	CUIPart_Symbol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Symbol(const CUIPart_Symbol& Prototype);
	virtual ~CUIPart_Symbol() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	UISYMBOL_TYPE m_eType = UISYMBOL_TYPE::SYMBOL_END;
	CLongvinter_Container* m_pHost = { nullptr };

private:
	HRESULT Ready_Components();



public:
	static CUIPart_Symbol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END