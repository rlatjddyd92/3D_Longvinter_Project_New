#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CContainer_Player : public CLongvinter_Container
{
public:
	enum PARTID 
	{ 
		PART_BODY, // 
		PART_HEAD, // BackpackSocket
		PART_BACKPACK,
		PART_GUN,
		PART_ARM_LEFT, // MCH-upper_arm_parent_socket_L
		PART_ARM_RIGHT, // MCH-upper_arm_parent_socket_R
		PART_LEG_LEFT, // MCH-thigh_parent_socket_L
		PART_LEG_RIGHT, //MCH-thigh_parent_socket_R
		PART_END
	};
	
	enum WEAPON_TYPE
	{
		WEAPON_MAIN,
		WEAPON_SUB,
		WEAPON_THROW,
		WEAPON_END
	};
private:
	CContainer_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainer_Player(const CContainer_Player& Prototype);
	virtual ~CContainer_Player() = default;

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
	virtual void DeadAction() override;


protected:
	virtual void Moving_Control(_float fTimeDelta) override;
	virtual void Weapon_Control(_float fTimeDelta) override;
	virtual void Camera_Control(_float fTimeDelta) override;
	virtual void Test_Control(_float fTimeDelta) override;

protected: // AI 관련 함수 
	virtual void Set_AI_Status(_float fTimeDelta) override;



private:
	_uint				m_iState = {  };


	// test 
	_int				m_iBody = 0;
	_int				m_iFace = 0;
	
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

	CCollider* m_pColliderCom = { nullptr };
	WEAPON_TYPE m_eWeaponType = WEAPON_TYPE::WEAPON_END;

public:
	static CContainer_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END