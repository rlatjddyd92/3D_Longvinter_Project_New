#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CLongvinter_Container : public CContainerObject
{

protected:
	CLongvinter_Container(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLongvinter_Container(const CLongvinter_Container& Prototype);
	virtual ~CLongvinter_Container() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:


public:
	static CLongvinter_Container* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};


END