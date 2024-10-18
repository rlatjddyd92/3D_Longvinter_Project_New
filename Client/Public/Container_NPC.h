#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CContainer_NPC : public CLongvinter_Container
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		NPC_TYPE eType = NPC_TYPE::NPC_END;
	} AI_NPC_Desc;



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




protected:
	CContainer_NPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CContainer_NPC(const CContainer_NPC& Prototype);
	virtual ~CContainer_NPC() = default;

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


	_int				m_iBody = _int(HUMAN_BODY::BODY_BROWN);
	_int				m_iFace = _int(HUMAN_FACE::FACE_NORMAL);

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

protected:
	CCollider* m_pColliderCom = { nullptr };
	ITEMINDEX m_eWeapon = ITEMINDEX::ITEM_END;

	NPC_TYPE m_eNPC_Type = NPC_TYPE::NPC_END;

	_bool m_bExplosionActive = false;
	_float m_fExplosionWaitTime = 10.f;

	_bool m_bCanSeeTransparent = false;

	_float m_fInterActionRange = 3.f;

public:
	static CContainer_NPC* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END