#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
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

class CUIManager : public CGameObject
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

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

	void Ready_UIPage();

private:
	CUIPage_Main* m_pPage_Main = { nullptr };
	

public:
	static CUIManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END