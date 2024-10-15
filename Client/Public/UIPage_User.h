#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_User : public CUIPage
{
public: 
	enum USER_VIEW_PART
	{
		PART_INVEN_BUTTON,
		PART_EQUIP_BUTTON,
		PART_CRAFTING_BUTTON,
		PART_OPTION_BUTTON,
		PART_HP_BAR,
		PART_END
	};



protected:
	CUIPage_User(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_User(const CUIPage_User& Prototype);
	virtual ~CUIPage_User() = default;

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

	vector<CUIPart*> m_vecPart;
private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;

public:
	static CUIPage_User* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END