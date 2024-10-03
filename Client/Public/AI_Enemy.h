#pragma once

#include "Client_Defines.h"
#include "Container_Enemy.h"

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
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex) override;
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex) override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CAI_Enemy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;




};

END