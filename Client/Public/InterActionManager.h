#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "../Default/InterAction_Defines.h"

BEGIN(Engine)

END


// [InterActionManager]
// 1. 모든 종류의 인터렉션 객체를 보유하고 관리 
// 2. Collider를 보유한 모든 객체의 충돌 검출을 진행 
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

public:
	void Input_ActionInfo(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType = CCollider::TYPE_SPHERE, CInterAction::TERRAIN_ACTION eAction = CInterAction::TERRAIN_ACTION::ACTION_END);



private:
	vector<CInterAction*> m_vecInterAction;


public:
	static CInterActionManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END