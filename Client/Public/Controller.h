#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


// [Controller]
// 1. 단순히 게임에 등장하는 객체가 아닌 유저의 게임 조작을 모두 총괄하는 객체
// 2. Container_Player, 
// 

BEGIN(Client)

class CController : public CGameObject
{
private:
	CController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController(const CController& Prototype);
	virtual ~CController() = default;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	void ChangeMode(CONTROLMODE eInput) { m_eMode = eInput; }
	CONTROLMODE GetMode() { return m_eMode; }

private:
	CONTROLMODE		m_eMode = CONTROLMODE::CMODE_MAIN;

public:
	static CController* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END