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

public:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	HRESULT Ready_Components();
	_uint m_iTextureIndex = 0;

	_bool m_bOnCursor = false; // <- ���콺 ��ġ�� ��ħ 
	_bool m_bPushed[2] = { false,false }; // <- ���콺, Ű�� �۵���Ŵ [0] : ���� ������, [1] : ���� ������ <- �� �� true�� ��� Pressing

public:
	static CUIPart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END