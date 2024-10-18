#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart : public CUIObject
{
protected:
	CUIPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart(const CUIPart& Prototype);
	virtual ~CUIPart() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void AddRender_UIPart();
	_bool IsOnCursor() { return m_bOnCursor; }
	_bool IsPushed() { return m_bPushed[1]; }
	_bool IsPressing() { return m_bPushed[0] * m_bPushed[1]; }

	void Change_Back_Color(_float fR = -1, _float fG = -1, _float fB = -1, _float fAlpah = -1);

	_bool SetPositionByObject(_matrix mHost);

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	HRESULT Ready_Components();

	

	_uint m_iTextureIndex = 0;

	_bool m_bOnCursor = false; // <- 마우스 위치와 겹침 
	_bool m_bPushed[2] = { false,false }; // <- 마우스, 키로 작동시킴 [0] : 이전 프레임, [1] : 현재 프레임 <- 둘 다 true인 경우 Pressing

	_float3 m_fAdjust = { 0.f,0.f,0.f };

public:
	static CUIPart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END