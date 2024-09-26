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

	void Input_Item(_int iItemIndex)
	{
		if (m_eType != CELL_INVEN)
			return;

		if (m_iItemIndex == -1)
		{
			m_fSizeX = INVEN_TEXTURE;
			m_fSizeY = INVEN_TEXTURE;
			m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = false;
		}
		m_iItemIndex = iItemIndex;
	}

	void Empty_Cell()
	{
		if (m_eType != CELL_INVEN)
			return;

		if (m_iItemIndex != -1)
		{
			m_fSizeX = INVEN_CELLSIZE;
			m_fSizeY = INVEN_CELLSIZE;
			m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		}
		m_iItemIndex = -1;
	}

	void Set_Picked(_bool bIsPicked)
	{
		if (m_iItemIndex == -1)
			return;

		m_bPicked = bIsPicked;
		if (m_bPicked)
			m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = true;
		else
			m_bChangeColor[0] = m_bChangeColor[1] = m_bChangeColor[2] = false;
	}

protected:
	UICELL_TYPE m_eType = UICELL_TYPE::CELL_END;

	HRESULT Inven_Render();

private:
	HRESULT Ready_Components();
	_int m_iItemIndex = -1; // <- -1은 비어 있는 상태 
	_bool m_bPicked = false;

public:
	static CUIPart_Cell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END