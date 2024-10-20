#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "../Default/UIPage_Defines.h"
#include "Container_Turret.h"

// [UIManager]
// 1. ���� UIâ, �ý��� Ŭ������ �����ϰ� �ʿ信 ���� ����� �� �ְ� �ϴ� �Ŵ��� 
// 2. UI Ŭ�������� �̸� �����ξ��ٰ� �ʿ信 ���� ���� �� ������Ʈ ���� 
// 
// 
// 
// [UI ������Ʈ] 
// 1. ��ư, â, �ؽ�Ʈ �ڽ� �� �ϳ��� UI â�� �����ϱ� ���� ���
// 2. ��Ȳ�� ���� ����� �����͸� ���� ����� ���� 
// 
// [UI �����̳�] 
// 1. �κ��丮, ����, ���� �� ������ ����� �� �ִ� UI â / ��ü 
// 2. ������Ʈ�� �����Ͽ� ��������� �ϳ��� �����̳ʿ� ���� ������Ʈ���� ���� �����δ� 
// 
// 
// �� �����̳ʴ� ������Ʈ�� ��� �� ����ü�̸� ���� ��� ������ ������Ʈ<->�ٸ� ��ü ���̿��� �̷����� 
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

public: // Ŀ�� ���� �Լ�
	void ActivateCursor() { m_iTextureIndex = 2; }
	void ShowInformMessage(wstring Text);


public: // ���� ���� �Լ� 
	void ShowToolTip(_float fCellX, _float fCellY, ITEMARRAY eArray, _int iIndex) { m_bShowTooltip = true;  m_pPage_ToolTip->ShowToolTip(fCellX, fCellY, eArray, iIndex); }


public: 
	_bool Show_Interaction_Function(_matrix mHost, wstring InterName, wstring Function_E = TEXT("����"), wstring Function_F = TEXT("����"), wstring Function_C = TEXT("����"), _vector Adjust = { 0.f,1.f,0.f,0.f })
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

public: // <- ���� ���� �Լ� 
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