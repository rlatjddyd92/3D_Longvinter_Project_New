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

class CUIPart_Bar final : public CUIPart
{
public:
	enum UIBAR_TYPE
	{
		BAR_PLAYER_HP,
		BAR_ENEMY_HP,
		BAR_PROGRESS,
		BAR_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UIBAR_TYPE eType = UIBAR_TYPE::BAR_END;
		CLongvinter_Container* pHost = { nullptr };
	} UIBar_DESC;


private:
	CUIPart_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Bar(const CUIPart_Bar& Prototype);
	virtual ~CUIPart_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;



protected:
	UIBAR_TYPE m_eType = UIBAR_TYPE::BAR_END;
	CLongvinter_Container* m_pHost = { nullptr };

	_float m_fFill_X = 0.f;
	_float m_fFill_SizeX = 0.f;

private:
	HRESULT Ready_Components();

public:
	static CUIPart_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END