#pragma once

#include "Client_Defines.h"
#include "Missile.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CThrow_Object : public CMissile
{




private:
	CThrow_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CThrow_Object(const CThrow_Object& Prototype);
	virtual ~CThrow_Object() = default;


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
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };






public:
	static CThrow_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END