#pragma once

#include "Client_Defines.h"
#include "InterAction.h"

BEGIN(Engine)

END

BEGIN(Client)

class CMissile : public CInterAction
{
protected:
	CMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMissile(const CMissile& Prototype);
	virtual ~CMissile() = default;


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



private:







public:
	static CMissile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END