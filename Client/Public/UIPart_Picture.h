#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Picture final : public CUIPart
{
public:
	enum UIPICTURE_TYPE
	{
		PICTUER_BGA,
		PICTUER_MAINLOGO,
		PICTUER_CASH,
		PICTUER_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UIPICTURE_TYPE eType = UIPICTURE_TYPE::PICTUER_END;
	} UIPicture_DESC;



private:
	CUIPart_Picture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Picture(const CUIPart_Picture& Prototype);
	virtual ~CUIPart_Picture() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	UIPICTURE_TYPE m_eType = UIPICTURE_TYPE::PICTUER_END;

private:
	HRESULT Ready_Components();



public:
	static CUIPart_Picture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END