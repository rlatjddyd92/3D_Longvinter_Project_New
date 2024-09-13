#pragma once

#include "Base.h"
#include "Transform.h"

/* 모든 게임내에 사용되는 게임 오브젝트들의 부모 클래스다. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{

	} GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	class CComponent* Find_Component(const _wstring& strComponentTag);
	_bool GetDead() { return m_bDead; }
	_bool GetOff() { return m_bOff; }
	_bool GetRender() { return m_bRender; }
	void SetOn() { m_bOff = false; } // <- Obj를 off하면 외부에서만 on이 가능하므로 추가 

protected:
	void SetDead() { m_bDead = true; }
	void SetOff(_bool bInput) { m_bOff = bInput; }
	void SetRender(_bool bInput) { m_bRender = bInput; }

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CTransform*			m_pTransformCom = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	_bool						m_bDead = false;
	_bool						m_bOff = false; // <- 오브젝트를 삭제하지는 않으나 작동을 정지할 때 사용
	_bool						m_bRender = true; // <- 오브젝트 렌더 진행 여부를 설정, OFF시켜도 다른 기능은 작동 

	_bool						m_bChangeColor[3] = { false,false,false };
	_float						m_fRGB[3] = { 0.f,0.f,0.f };

protected:
	map<const _wstring, class CComponent*>			m_Components;

protected:
	
	HRESULT Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END