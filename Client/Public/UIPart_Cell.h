#pragma once

#include "Client_Defines.h"
#include "UIPart.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUIPart_Cell final : public CUIPart
{
public:
	enum UICELL_TYPE
	{
		CELL_INVEN,
		CELL_END
	};


public:
	typedef struct : public CUIObject::UI_DESC
	{
		UICELL_TYPE eType = UICELL_TYPE::CELL_END;
	} UICell_DESC;



private:
	CUIPart_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIPart_Cell(const CUIPart_Cell& Prototype);
	virtual ~CUIPart_Cell() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	UICELL_TYPE m_eType = UICELL_TYPE::CELL_END;

private:
	HRESULT Ready_Components();



public:
	static CUIPart_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END