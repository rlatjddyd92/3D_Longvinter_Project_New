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
public: 
	typedef struct CONTAINER_INTERACTION_INFO
	{
		CONTAINER eType;
		CLongvinter_Container* pPoint;
		CCollider* pCollider;
	}CON_INTER_INFO;

	


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
	void Add_InterActionObject(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType = CCollider::TYPE_SPHERE, CInterAction::TERRAIN_ACTION eAction = CInterAction::TERRAIN_ACTION::ACTION_END, _float fAngle = 0.f);
	void Input_ContainerColliderPointer(CONTAINER eContanerType, CLongvinter_Container* pHost, CCollider* pCollider);
	void Add_InterActionObject_BySpec(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fAngle = 0.f);

	void Delete_LastInterAction(INTERACTION eInterType);

private:
	void Check_Collision_InterAction(INTERACTION eFirst, INTERACTION eSecond);
	void Check_Collision_InterAction_Container(INTERACTION eInter, CONTAINER eContainer);
	void Check_Collision_Container(CONTAINER eFirst, CONTAINER eSecond);


private:
	vector<CInterAction*> m_vecInterAction;

	vector<list<CON_INTER_INFO*>> m_vecConInterlist;





public:
	static CInterActionManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END