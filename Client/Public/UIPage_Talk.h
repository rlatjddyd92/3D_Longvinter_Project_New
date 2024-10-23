#pragma once

#include "Client_Defines.h"
#include "UIPage.h"
#include "Container_NPC.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPage_Talk : public CUIPage
{
public: 
	struct SCRIPT
	{
		_tchar* pName;
		_tchar* pScript;
		_tchar* pSelect;
	};



protected:
	CUIPage_Talk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPage_Talk(const CUIPage_Talk& Prototype);
	virtual ~CUIPage_Talk() = default;

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

	void SetHost(CContainer_NPC* pHost)
	{
		m_pHost = pHost;
		if (m_pHost != nullptr)
			SetOn();
	}

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	void Ready_Script();

	CUIPart_Button* m_pButton_Close = { nullptr };
	CUIPart_Back* m_pBack_Window = { nullptr };
	CUIPart_Back* m_pBack_Window_Header = { nullptr };
	
	CUIPart_Back* m_pBack_Select = { nullptr };

	CContainer_NPC* m_pHost = { nullptr };

private:
	// 창 이동, 크기 조절 관련 변수 
	_bool m_bMoving = false;
	_float m_fBeforeX = 0.f;
	_float m_fBeforeY = 0.f;

	vector<vector<SCRIPT*>> m_vecScript;

	_int m_iFunction = 3;

	_float m_fGap = 10.f;
	_float m_fFunc_X = 400.f;
	_float m_fFunc_Y = 50.f;


public:
	static CUIPage_Talk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END