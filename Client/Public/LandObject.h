#pragma once

#include "Client_Defines.h"

#include "InterAction.h"
#include "Collider.h"

BEGIN(Engine)

END

BEGIN(Client)
class CLongvinter_Container;


class CLandObject : public CInterAction
{
public:
	typedef struct LANDOBJ_INFO
	{
		CTransform* pTransform{};
		CCollider* pCollider{};
		CLongvinter_Container* pHost = { nullptr }; // <- 이 객체를 만든 대상

		_float fTime = 0.f;
		_bool bActive = true;
		_bool bDead = false;
	}LOBJ_INFO;




protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& Prototype);
	virtual ~CLandObject() = default;


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

protected:
	INTERACTION m_eType = INTERACTION::INTER_END;
	_bool m_bSettingComplete = false;


public:
	static CLandObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END