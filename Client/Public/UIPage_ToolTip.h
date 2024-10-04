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
public:
	

	enum TOOLTIP_COLOR
	{
		COLOR_NORMAL,
		COLOR_PLUS,
		COLOR_MINUS,
		COLOR_END
	};

	typedef struct PART_SIZE
	{
		_float m_fSize = 0.f;
		_float m_fTextScale = 0.f;
		_float m_fGap = 0.f;
	}P_SIZE;



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


	void ShowToolTip(_float fCellX, _float fCellY, ITEMARRAY eArray, _int iIndex);

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

	CUIPart_Back* m_pBack_Window = { nullptr };
	vector<CUIPart_TextBox*> m_vecText;

private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;



private: // 툴팁 디자인 관련 변수 
	_float m_fGap_Top = 10.f;
	_float m_fGap_Left = 10.f;

	vector<PART_SIZE> m_vecSize;

	_int m_iTagNum = 1;
	_int m_iTagMaxNum = (_int(TOOLTIP_TEXT::TEXT_TAG_10) - _int(TOOLTIP_TEXT::TEXT_TAG_1)) + 1;

	_float m_fSizeY_NoneTag = 0.f;

	ITEMARRAY m_eNowArray = ITEMARRAY::ARRAY_END;
	_int m_iNowIndex = -1;


	
public:
	static CUIPage_ToolTip* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END