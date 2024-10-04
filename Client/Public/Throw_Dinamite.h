#pragma once

#include "Client_Defines.h"
#include "Missile.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CThrow_Dinamite : public CMissile
{




private:
	CThrow_Dinamite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CThrow_Dinamite(const CThrow_Dinamite& Prototype);
	virtual ~CThrow_Dinamite() = default;


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
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };






public:
	static CThrow_Dinamite* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END