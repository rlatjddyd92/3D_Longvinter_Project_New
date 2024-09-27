#pragma once

#include "Client_Defines.h"
#include "Explosion.h"

BEGIN(Engine)

END

BEGIN(Client)

class CExplosion_Normal : public CExplosion
{
protected:
	CExplosion_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CExplosion_Normal(const CExplosion_Normal& Prototype);
	virtual ~CExplosion_Normal() = default;


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
	static CExplosion_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END


