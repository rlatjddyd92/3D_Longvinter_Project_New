#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "../Default/UIPage_Defines.h"

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

	void ActivateCursor() { m_iTextureIndex = 2; }

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

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END