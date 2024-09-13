#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"
#include "../Default/Factory_Header.h"
#include "../Default/UIPart_Defines.h"
#include "../Default/UIPage_Defines.h"

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
	HRESULT Ready_Prototype_Model(CModel::TYPE eType, const _wstring strTag, const _char* pPath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
#pragma endregion



#pragma region UIPart
	CUIPart_Back* MakeUIPart_Back(CUIPart_Back::UIBACK_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Button* MakeUIPart_Button(CUIPart_Button::UIBUTTON_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	CUIPart_Picture* MakeUIPart_Picture(CUIPart_Picture::UIPICTURE_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	
#pragma endregion

#pragma region UIPage
	CUIPage_Main* MakeUIPage_Main();
#pragma endregion



	// Container 제작 
#pragma region Container_Player
	void Make_Container_Player(_float3 Position);
#pragma endregion

#pragma region Container_NPC

#pragma endregion

#pragma region Container_Enemy

#pragma endregion

#pragma region Container_Animal

#pragma endregion

#pragma region Container_Item

#pragma endregion

#pragma region Container_InterAction

#pragma endregion

#pragma region Container_Effect

#pragma endregion

private: // <- 프로토타입 제작
#pragma region Program_Start
	HRESULT Ready_Prototype_Texture();
	HRESULT Ready_Prototype_Part();
	HRESULT Ready_Prototype_Container();
	HRESULT Ready_Prototype_Shader();
	HRESULT Ready_Prototype_Buffer(); 
	HRESULT Ready_Prototype_UIPart();
	HRESULT Ready_Prototype_UIPage();

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