#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END


// [InterActionManager]
// 1. ��� ������ ���ͷ��� ��ü�� �����ϰ� ���� 
// 2. Collider�� ������ ��� ��ü�� �浹 ������ ���� 
// 3. 

BEGIN(Client)

class CInterActionManager : public CGameObject
{
private:
	CInterActionManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInterActionManager(const CInterActionManager& Prototype);
	virtual ~CInterActionManager() = default;


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

	

private:
	

public:
	static CInterActionManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END