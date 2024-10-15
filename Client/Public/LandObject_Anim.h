#pragma once

#include "Client_Defines.h"

#include "LandObject.h"
#include "Collider.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CLandObject_Anim : public CLandObject
{
public:
	




protected:
	CLandObject_Anim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject_Anim(const CLandObject_Anim& Prototype);
	virtual ~CLandObject_Anim() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction) override;
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction) override;

public:



private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

private:
	class CShader* m_pShaderCom_Texture = { nullptr };
	class CShader* m_pShaderCom_NonTexture = { nullptr };
	vector<CModel*> m_vecModelCom;

public:
	static CLandObject_Anim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END