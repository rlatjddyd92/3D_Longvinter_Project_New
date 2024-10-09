#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "../Default/Effect_Defines.h"

BEGIN(Engine)

END


// [EffectManager]
// 1. 모든 종류의 인터렉션 객체를 보유하고 관리 
// 2. Collider를 보유한 모든 객체의 충돌 검출을 진행 
// 3. 

BEGIN(Client)

class CEffectManager : public CGameObject
{



private:
	CEffectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectManager(const CEffectManager& Prototype);
	virtual ~CEffectManager() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void MakeEffect(EFFECT_TYPE eType, _float3 fPosition);



private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();


private:
	vector<CEffect*> m_vecEffect;




public:
	static CEffectManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END