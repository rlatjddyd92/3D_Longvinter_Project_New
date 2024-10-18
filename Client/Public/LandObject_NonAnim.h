#pragma once

#include "Client_Defines.h"

#include "LandObject.h"
#include "Collider.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)


class CLandObject_NonAnim : public CLandObject
{
public:





protected:
	CLandObject_NonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject_NonAnim(const CLandObject_NonAnim& Prototype);
	virtual ~CLandObject_NonAnim() = default;


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
	HRESULT SetLandObject(INTERACTION eIndex);

private:
	void DestroyAction(INTER_INFO* pAction);

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

private:
	class CShader* m_pShaderCom = { nullptr };
	vector<CModel*> m_vecModelCom;

	_int m_iItemPageKey = -1; // <- 아이템 보따리 전용
	_bool m_bTexture = false;

	_float m_fMakeEffect = 0.1f;
	_float m_fMakeFire = 1.f;

	// 몬스터 메이커 
	_int m_iMonsterMake = 20;
	_bool m_bActive = false;

public:
	static CLandObject_NonAnim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END