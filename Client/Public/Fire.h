#pragma once

#include "Client_Defines.h"
#include "Missile.h"

BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer;
END

BEGIN(Client)

class CFire : public CMissile
{
	


private:
	CFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFire(const CFire& Prototype);
	virtual ~CFire() = default;


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
	

	_float m_fMakeEffect = 0.1f;


	


public:
	static CFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END