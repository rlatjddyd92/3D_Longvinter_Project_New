#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CContainer_Enemy : public CLongvinter_Container
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		ENEMY_TYPE eType = ENEMY_TYPE::ENEMY_TYPE_END;
	} AI_Enemy_Desc;



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
	enum STATE {
		STATE_EAT = 3,
		STATE_THROW_WAIT = 6,
		STATE_SLOW_WALK = 8,
		STATE_HIT = 9,
		STATE_HANDGUN = 10,
		STATE_GUN = 11,
		STATE_SIT = 12,
		STATE_GIVE = 13,
		STATE_THROW = 14,
		STATE_GRANADE = 15,
		STATE_WALK_R = 40,
		STATE_WALK_L = 41,
		STATE_AIM = 51,
		STATE_CHAINSAW = 56,
		STATE_IDEL = 58,
		STATE_WALK = 64,
		STATE_HI = 70,
		STATE_END
	};
	enum WEAPON_TYPE
	{
		WEAPON_MAIN,
		WEAPON_SUB,
		WEAPON_THROW,
		WEAPON_END
	};
	



protected:
	CContainer_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainer_Enemy(const CContainer_Enemy& Prototype);
	virtual ~CContainer_Enemy() = default;

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

protected: // AI 관련 함수 
	virtual void Set_AI_Status(_float fTimeDelta) override;
	void Burning();

protected:
	_uint				m_iState = {  };

	// test 
	_int				m_iBody = 2;
	_int				m_iFace = _int(HUMAN_FACE::FACE_NORMAL);

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

protected:
	CCollider* m_pColliderCom = { nullptr };
	ITEMINDEX m_eWeapon = ITEMINDEX::ITEM_MACHINEGUN;

	ENEMY_TYPE m_eEnemy_Type = ENEMY_TYPE::ENEMY_TYPE_END;

public:
	static CContainer_Enemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END