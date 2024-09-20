#pragma once

#include "Client_Defines.h"
#include "UIObject.h"
#include "../Default/UIPage_Defines.h"

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