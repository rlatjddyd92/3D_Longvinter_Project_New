#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCContainer_Turret : public CLongvinter_Container
{
public:
	



protected:
	CCContainer_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCContainer_Turret(const CCContainer_Turret& Prototype);
	virtual ~CCContainer_Turret() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, CInterAction::INTER_INFO& tOpponent) override;
	virtual void Collision_Reaction_MadeInterAction(CGameObject* pPoint, INTERACTION eIndex) override;
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex) override;


protected:
	virtual void Moving_Control(_float fTimeDelta) override;
	virtual void Weapon_Control(_float fTimeDelta) override;
	virtual void Camera_Control(_float fTimeDelta) override;
	virtual void Test_Control(_float fTimeDelta) override;

	virtual void DeadAction() override;

protected: // AI 관련 함수 
	virtual void Set_AI_Status(_float fTimeDelta) override;
	void Burning();



protected:
	_uint				m_iState = {  };




private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

protected:
	CCollider* m_pColliderCom = { nullptr };

	_bool m_bExplosionActive = false;
	_float m_fExplosionWaitTime = 10.f;

	_bool m_bCanSeeTransparent = false;

public:
	static CCContainer_Turret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END