#pragma once

#include "Client_Defines.h"
#include "Longvinter_Container.h"
#include "GameObject.h"
#include "Collider.h"

BEGIN(Engine)

END

BEGIN(Client)

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


public:
	void Add_ActionInfo(CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType = CCollider::TYPE_SPHERE, TERRAIN_ACTION eAction = TERRAIN_ACTION::ACTION_END);
	


private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



protected:
	list<INTERACTION_INFO*> m_Actionlist;


public:
	static CInterAction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END