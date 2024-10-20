#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "../Default/LandObject_Defines.h"

BEGIN(Engine)

END



BEGIN(Client)

class CLandObjectManager : public CGameObject
{
public:






private:
	CLandObjectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObjectManager(const CLandObjectManager& Prototype);
	virtual ~CLandObjectManager() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



private:


private:
	



public:
	static CLandObjectManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END