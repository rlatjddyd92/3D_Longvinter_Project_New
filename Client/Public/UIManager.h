#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "../Default/UIPage_Defines.h"
#include "Container_Turret.h"

// [UIManager]
// 1. 각종 UI창, 시스템 클래스를 보관하고 필요에 따라 사용할 수 있게 하는 매니저 
// 2. UI 클래스들은 미리 만들어두었다가 필요에 따라 렌더 및 업데이트 진행 
// 
// 
// 
// [UI 콤포넌트] 
// 1. 버튼, 창, 텍스트 박스 등 하나의 UI 창을 구현하기 위한 요소
// 2. 상황에 따라 연결된 포인터를 통해 기능을 구현 
// 
// [UI 콘테이너] 
// 1. 인벤토리, 설정, 상점 등 유저가 사용할 수 있는 UI 창 / 객체 
// 2. 콤포넌트를 조합하여 만들어지며 하나의 콘테이너에 속한 콤포넌트들은 같이 움직인다 
// 
// 
// ※ 콘테이너는 콤포넌트를 모아 둔 집합체이며 실제 기능 구현은 콤포넌트<->다른 객체 사이에서 이뤄진다 
// 
// 
// 
// 
// 
// 

BEGIN(Client)

class CUIManager : public CUIObject
{
protected:
	CUIManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIManager(const CUIManager& Prototype);
	virtual ~CUIManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: // 커서 관련 함수
	void ActivateCursor() { m_iTextureIndex = 2; }
	void ShowInformMessage(wstring Text);


public: // 툴팁 관련 함수 
	void ShowToolTip(_float fCellX, _float fCellY, ITEMARRAY eArray, _int iIndex) { m_bShowTooltip = true;  m_pPage_ToolTip->ShowToolTip(fCellX, fCellY, eArray, iIndex); }


public: 
	_bool Show_Interaction_Function(_matrix mHost, wstring InterName, wstring Function_E = TEXT("없음"), wstring Function_F = TEXT("없음"), wstring Function_C = TEXT("없음"), _vector Adjust = { 0.f,1.f,0.f,0.f })
	{
		if (m_bActiveInteraction)
			return false;

		m_pPage_User->Show_Interaction_Function(mHost, InterName, Function_E, Function_F, Function_C, Adjust);
		m_bActiveInteraction = true;
		return true;
	}

public:
	void MakeEnemyHpBar(CLongvinter_Container* pHost);
	void MakeSymbol(CLongvinter_Container* pHost);

public: // <- 상점 관련 함수 
	void OpenShopPage(_int iShopNum);
	void OpenHackPage(CContainer_Turret* pTurret);

	_bool GetOpenShop() { return m_bIsShopOpen; }
	_bool GetOpenHack() { return m_bIsHackOpen; }

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

	void Ready_UIPage();

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_uint m_iTextureIndex = 0;

	list<CUIPage*> m_Pagelist;

	CUIPage_Main* m_pPage_Main = { nullptr };
	CUIPage_Test* m_pPage_Test = { nullptr };
	CUIPage_Inven* m_pPage_Inven = { nullptr };
	CUIPage_Equip* m_pPage_Equip = { nullptr };
	CUIPage_Crafting* m_pPage_Crafting = { nullptr };
	CUIPage_Option* m_pPage_Option = { nullptr };
	CUIPage_ToolTip* m_pPage_ToolTip = { nullptr };
	CUIPage_User* m_pPage_User = { nullptr };
	CUIPage_Shop* m_pPage_Shop = { nullptr };
	CUIPage_Hack* m_pPage_Hack = { nullptr };

	list<CUIPart_TextBox*> m_Informlist;
	list<CUIPart_Bar*> m_EnemyHplist;
	list<CUIPart_Symbol*> m_Symbollist;

	_int m_iMaxInform = 10;

	_bool m_bShowTooltip = false;

	_bool m_bActiveInteraction = false;

	_bool m_bIsShopOpen = false;
	_bool m_bIsHackOpen = false;

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END