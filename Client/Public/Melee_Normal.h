#pragma once

#include "Client_Defines.h"
#include "Melee.h"

BEGIN(Engine)

END

BEGIN(Client)

class CMelee_Normal : public CMelee
{
private:
	CMelee_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMelee_Normal(const CMelee_Normal& Prototype);
	virtual ~CMelee_Normal() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction) override;
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction) override;
	

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



private:







public:
	static CMelee_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END
