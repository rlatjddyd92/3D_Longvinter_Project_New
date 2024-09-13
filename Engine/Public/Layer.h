#pragma once

#include "Base.h"

/* 객체들을 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Priority_Update(_float fTimeDelta);
	HRESULT Update(_float fTimeDelta);
	HRESULT Late_Update(_float fTimeDelta);

	CGameObject* Get_GameObject(_int iIndex = 0);

	HRESULT Delete_GameObject(class CGameObject* pGameObject);
	HRESULT Delete_GameObject(_int iIndex);
	_int Get_Size() const { return m_GameObjects.size(); }

public:
	class CComponent* Find_Component(const _wstring& strComponentTag, _int iIndex);
	class CGameObject* Find_Object(_int iIndex);

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END