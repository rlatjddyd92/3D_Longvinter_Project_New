#pragma once

#include "Client_Defines.h"

#include "GameObject.h"
#include "Collider.h"

BEGIN(Engine)

END

BEGIN(Client)
class CLongvinter_Container;

class CInterAction : public CGameObject
{
public:
	enum TERRAIN_ACTION
	{
		ACTION_BOUNCE,
		ACTION_EXPLOSION,
		ACTION_DEAD,
		ACTION_END
	};

	typedef struct INTERACTION_INFO
	{
		CTransform* pTransform{};
		CCollider* pCollider{};
		TERRAIN_ACTION eAction = TERRAIN_ACTION::ACTION_END;
		CLongvinter_Container* pHost = { nullptr };

		_float fTime = 0.f;
		_bool bActive = false;
		_bool bDead = false;
	}INTER_INFO;

	


protected:
	CInterAction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInterAction(const CInterAction& Prototype);
	virtual ~CInterAction() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Collision_Reaction_InterAction(CGameObject* pPoint, INTERACTION eIndex, INTER_INFO* pAction);
	virtual void Collision_Reaction_Container(CGameObject* pPoint, CONTAINER eIndex, INTER_INFO* pAction);

public:
	void Add_InterActionObject(CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType = CCollider::TYPE_SPHERE, TERRAIN_ACTION eAction = TERRAIN_ACTION::ACTION_END);
	list<INTERACTION_INFO*>* Get_Actionlist() {return &m_Actionlist;}
	void Add_InterActionObject_BySpec(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec);
	_int Get_ColliderType() { return m_iColliderType; }

protected:
	HRESULT Bind_WorldMatrix(class CShader* pShader, const _char* pContantName);


private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



protected:
	list<INTERACTION_INFO*> m_Actionlist;
	_float4x4				m_WorldMatrix = {};
	_float					m_fShowTime = 0.f;

protected:
	_float3					m_fSpec_Extent = { 0.f,0.f,0.f };
	_float					m_fSpec_Scale = 1.f;
	_float					m_fSpec_PushedPower = 0.f;
	_float					m_fSpec_PushedPower_Decrease = 0.f;
	_int					m_iColliderType = 0;

public:
	static CInterAction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END