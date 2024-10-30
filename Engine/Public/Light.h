#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	void SetDead() { m_bDead = true; }
	_bool GetDead() { return m_bDead; }
	void SetFrame(_int iFrame) { m_iFrame = iFrame; }
	_int GetFrame() { return m_iFrame; }

private:
	LIGHT_DESC				m_LightDesc{};
	_int					m_iFrame = 0;
	_bool					m_bDead = false;

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

END