#pragma once

#include "Client_Defines.h"
#include "InterAction.h"

BEGIN(Engine)

END

BEGIN(Client)

class CMelee : public CInterAction
{
protected:
	CMelee(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMelee(const CMelee& Prototype);
	virtual ~CMelee() = default;


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







public:
	static CMelee* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END
