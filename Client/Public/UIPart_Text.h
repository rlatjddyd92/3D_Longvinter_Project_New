#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Text final : public CUIPart
{
public:
	enum UITEXT_TYPE
	{
		TEXT_DEFAULT,
		TEXT_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UITEXT_TYPE eType = UITEXT_TYPE::TEXT_END;
	} UITEXT_DESC;


private:
	CUIPart_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Text(const CUIPart_Text& Prototype);
	virtual ~CUIPart_Text() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	UITEXT_TYPE m_eType = UITEXT_TYPE::TEXT_END;

private:
	HRESULT Ready_Components();



public:
	static CUIPart_Text* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END