#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CContainer_Turret : public CLongvinter_Container
{
public:
	



protected:
	CContainer_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainer_Turret(const CContainer_Turret& Prototype);
	virtual ~CContainer_Turret() = default;

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

	void SetHack(_int iFunc);
	
	_bool GetHack(_int iFunc) { return m_bHack[iFunc]; }

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
	_bool				m_bHack[3] = { false,false,false };



private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

protected:
	CCollider* m_pColliderCom = { nullptr };

	_bool m_bExplosionActive = false;
	_float m_fExplosionWaitTime = 10.f;

	_bool m_bCanSeeTransparent = false;

public:
	static CContainer_Turret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END