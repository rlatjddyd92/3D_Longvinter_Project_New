#pragma once

#include "Client_Defines.h"

#include "Effect_Particle.h"
#include "Collider.h"

BEGIN(Engine)

END

BEGIN(Client)
class CLongvinter_Container;

class CParticle_Fire : public CEffect_Particle
{
public:


protected:
	CParticle_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Fire(const CParticle_Fire& Prototype);
	virtual ~CParticle_Fire() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Bind_WorldMatrix(class CShader* pShader, const _char* pContantName);


private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



protected:
	_float4x4				m_WorldMatrix = {};
	_float					m_fShowTime = 0.f;

protected:


public:
	static CParticle_Fire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END