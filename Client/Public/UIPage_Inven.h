#pragma once

#include "Client_Defines.h"
#include "UIPage.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_Inven : public CUIPage
{
protected:
	CUIPage_Inven(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Inven(const CUIPage_Inven& Prototype);
	virtual ~CUIPage_Inven() = default;

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

	CUIPart_Button* m_pButton_Close = { nullptr };
	CUIPart_Back* m_pBack_Window = { nullptr };
	CUIPart_Back* m_pBack_Window_Header = { nullptr };

	vector<CUIPart_Cell*> m_vecInvenCell;

private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;

	// Inven 설정 
	_float m_fInvenCellSize = INVEN_CELLSIZE;
	_int m_iInvenCol = INVEN_COL;
	_int m_iInvenRow = INVEN_ROW;
public:
	static CUIPage_Inven* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END