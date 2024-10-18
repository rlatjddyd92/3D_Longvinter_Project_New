#pragma once

#include "Client_Defines.h"
#include "Container_NPC.h"

// [AI 객체]
// 1. NPC/몬스터 조작용 객체
// 2. 콘테이너를 상속받는다 -> 모든 조작 가능하도록 개발 
// 3. 각 콘테이너에 맞는 형태로 개별 제작 
// 
// 
// 
// 
// 
//


BEGIN(Engine)

END

BEGIN(Client)



class CAI_NPC : public CContainer_NPC
{
private:
	CAI_NPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAI_NPC(const CAI_NPC& Prototype);
	virtual ~CAI_NPC() = default;

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

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CAI_NPC* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END