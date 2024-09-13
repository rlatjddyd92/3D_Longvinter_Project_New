#pragma once

#include "GameObject.h"

/* ���� ������ ���� ��, ��������� �����, �����ν��Ͻ� �ȿ� �ִ� ���������ο� ������ �ش�.  */
/* ������� ������ ���� �����͵��� ������ �մ�. ����� == cTransform */

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vEye;
		_float4		vAt;

		_float		fFovy = {};
		_float		fAspect = {};
		_float		fNear = {};
		_float		fFar = {};
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float				m_fFovy = {};
	_float				m_fAspect = {};
	_float				m_fNear = {};
	_float				m_fFar = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END