#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"
#include "../Default/Factory_Header.h"
#include "../Default/UIPart_Defines.h"
#include "../Default/UIPage_Defines.h"
#include "../Default/InterAction_Defines.h"
#include "../Default/Effect_Defines.h"

BEGIN(Engine)

END

BEGIN(Client)

class CFactory : public CBase
{
private:
	CFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual ~CFactory() = default;

public: // <- 외부에서 Factory 사용을 위한 함수
	HRESULT Setting_Program_Start();
	HRESULT Setting_Ingame_Start();
	HRESULT Setting_Editor_Start();
#pragma region Model_Save
	void Save_Prototype_Model_Data(); // <- Model을 Data 파일로 저장 
#pragma endregion
#pragma region Model_Load
	HRESULT Ready_Prototype_Model_Single(CModel::TYPE eType, _bool bTexture, const _wstring strTag, const _char* pPath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
#pragma endregion



#pragma region UIPart
	CUIPart_Back* MakeUIPart_Back(CUIPart_Back::UIBACK_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Button* MakeUIPart_Button(CUIPart_Button::UIBUTTON_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Picture* MakeUIPart_Picture(CUIPart_Picture::UIPICTURE_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Cell* MakeUIPart_Cell(CUIPart_Cell::UICELL_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Bar* MakeUIPart_Bar(CUIPart_Bar::UIBAR_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, CLongvinter_Container* pHost = nullptr);
	CUIPart_TextBox* MakeUIPart_TextBox(CUIPart_TextBox::UITEXTBOX_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _bool bCenter, _bool bAutoRemove = false, _float fShowTime = 0.f, CLongvinter_Container* pHost = nullptr);
	CUIPart_Symbol* MakeUIPart_Symbol(CUIPart_Symbol::UISYMBOL_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, CLongvinter_Container* pHost = nullptr);
#pragma endregion

#pragma region UIPage
	CUIPage_Main* MakeUIPage_Main();
	CUIPage_Test* MakeUIPage_Test();
	CUIPage_Inven* MakeUIPage_Inven();
	CUIPage_Equip* MakeUIPage_Equip();
	CUIPage_Crafting* MakeUIPage_Crafting();
	CUIPage_Option* MakeUIPage_Option();
	CUIPage_ToolTip* MakeUIPage_ToolTip();
	CUIPage_User* MakeUIPage_User();
	CUIPage_Shop* MakeUIPage_Shop();
	CUIPage_Hack* MakeUIPage_Hack();
	CUIPage_Talk* MakeUIPage_Talk();
#pragma endregion



	// Container 제작 
#pragma region Container_Player
	void Make_Container_Player(_float3 Position, _float fAngle = 0.f);
#pragma endregion

#pragma region Container_NPC

#pragma endregion

#pragma region Container_Enemy
	void Make_Container_Enemy(_float3 Position, ENEMY_TYPE eType, _float fAngle = 0.f);
	void Make_Container_NPC(_float3 Position, NPC_TYPE eType, _float fAngle = 0.f);
	void Make_Container_Turret(_float3 Position, _float fAngle = 0.f);
#pragma endregion


#pragma region Container_Etc
	CPartObject* Make_Tool_Head(_int iHeadIndex, _uint* pParentState, const _float4x4* pMatrix, const _float4x4* pParent);
#pragma endregion

private: // <- 프로토타입 제작
#pragma region Program_Start
	HRESULT Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Font();
	HRESULT Ready_Prototype_Part();
	HRESULT Ready_Prototype_Container();
	HRESULT Ready_Prototype_Model();
	HRESULT Ready_Prototype_Shader();
	HRESULT Ready_Prototype_Buffer();
	HRESULT Ready_Prototype_UIPart();
	HRESULT Ready_Prototype_UIPage();
	HRESULT Ready_Prototype_InterAction();
	HRESULT Ready_Prototype_Effect();

#pragma endregion


	

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	list<CModel*> m_ModelPrototypelist;
	list<_char*> m_ModelFilePath;

	

public:
	static CFactory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual void Free() override;
};

END