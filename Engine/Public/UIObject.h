#pragma once

#include "GameObject.h"

/* ���������� �ʿ��� �ټ��� ��ü�� ������ �� �ִ�.  */
/* ���������� �ʿ��� �����͵�� ����� �������ִ� ��Ȱ */
BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_float	fX{}, fY{}, fSizeX{}, fSizeY{}, fTime{};
	} UI_DESC;

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_UIPosition(_float fX, _float fY) { m_fX = fX; m_fY = fY; }
	void Get_UIPosition(_float* fX, _float* fY) { *fX = m_fX; *fY = m_fY; }
	void Move_UI(_float fX, _float fY) { m_fX += fX; m_fY += fY; }
	void Set_UISize(_float fSizeX, _float fSizeY) { m_fSizeX = fSizeX;  m_fSizeY = fSizeY; }
	void Set_UIShowTime(_float fTime) { m_fShowTime = fTime; }

protected:
	_float				m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};
	_uint				m_iDepth = {};

	_float				m_fViewWidth{}, m_fViewHeight{};

	_float				m_fShowTime = 0.f;



public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END