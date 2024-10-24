#pragma once

#include "Client_Defines.h"
#include "Container_Enemy.h"
#include "Container_Turret.h"

// [AI ��ü]
// 1. NPC/���� ���ۿ� ��ü
// 2. �����̳ʸ� ��ӹ޴´� -> ��� ���� �����ϵ��� ���� 
// 3. �� �����̳ʿ� �´� ���·� ���� ���� 
// 
// 
// 
// 
// 
//


BEGIN(Engine)

END

BEGIN(Client)



class CAI_Enemy : public CContainer_Enemy
{
private:
	CAI_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAI_Enemy(const CAI_Enemy& Prototype);
	virtual ~CAI_Enemy() = default;

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

protected: // AI ���� �Լ� 
	virtual void Set_AI_Status(_float fTimeDelta) override;

	CContainer_Turret* pTurret = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CAI_Enemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END