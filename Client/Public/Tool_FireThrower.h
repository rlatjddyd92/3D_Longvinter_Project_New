#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Tool.h"

// [Tool_FireThrower]
// Container의 Part 소켓을 사용하지 않을 때 빈 부분을 채우는 용도 

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CTool_FireThrower : public CTool
{

private:
	CTool_FireThrower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool_FireThrower(const CTool_FireThrower& Prototype);
	virtual ~CTool_FireThrower() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

	_float m_fTest_X = 0.f;
	_float m_fTest_Y = 0.f;
	_float m_fTest_Z = 0.f;

public:
	static CTool_FireThrower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END