#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Factory.h"
#include "GameInstance.h"

#pragma region ManagerClass
#include "FreeCamera.h"
#include "ImguiManager.h"
#include "Controller.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "ItemManager.h"
#include "InterActionManager.h"
#pragma endregion

BEGIN(Engine)

END

BEGIN(Client)



class CClientInstance : public CBase
{
	DECLARE_SINGLETON(CClientInstance)
	
private:
	CClientInstance();
	virtual ~CClientInstance() = default;

public: // <- �̱����� ���� �ܺ� ���ٿ� 
	// �� ���� ���� �� �ΰ���/������ ������ ���� �غ�� �Լ�
#pragma region SETTING 
	HRESULT Setting_Program_Start(ID3D11Device** pDevice, ID3D11DeviceContext** pContext, CGameInstance* pInstance); // <- ���α׷� ���� ���� ��
	HRESULT Setting_Ingame_Start(); // <- �ΰ��� ���� ��
	HRESULT Setting_Editor_Start(); // <- ������ ���� �� 
	void Release_CilentInstance(); // <- Ŭ�� �ν��Ͻ� ���� ��
#pragma endregion

#pragma region LEVEL
	HRESULT ChangeLevel(LEVELID eLevel);
	LEVELID GetNowLevel() { return m_eLevel; }
#pragma endregion



#pragma region PLAYER
	void Set_Player_Pointer(CContainer_Player* pPlayer) 
	{ 
		if (m_pPlayer)
			Safe_Release(m_pPlayer);
		m_pPlayer = pPlayer;
		Safe_AddRef(m_pPlayer);
	}
	CContainer_Player* Get_Player_Pointer() { return m_pPlayer; }
#pragma endregion

#pragma region FACTORY
	void Save_Prototype_Model_Data() { m_pFactory->Save_Prototype_Model_Data(); }
	void Make_Container_Player(_float3 Position) { m_pFactory->Make_Container_Player(Position); }

	// UI_Part
	CUIPart_Back* MakeUIPart_Back(CUIPart_Back::UIBACK_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY) { return m_pFactory->MakeUIPart_Back(eType, fX, fY, fSizeX, fSizeY); }
	CUIPart_Button* MakeUIPart_Button(CUIPart_Button::UIBUTTON_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY) { return m_pFactory->MakeUIPart_Button(eType, fX, fY, fSizeX, fSizeY); }
	CUIPart_Picture* MakeUIPart_Picture(CUIPart_Picture::UIPICTURE_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY) { return m_pFactory->MakeUIPart_Picture(eType, fX, fY, fSizeX, fSizeY); }
	CUIPart_Cell* MakeUIPart_Cell(CUIPart_Cell::UICELL_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY) { return m_pFactory->MakeUIPart_Cell(eType, fX, fY, fSizeX, fSizeY); }
	CUIPart_TextBox* MakeUIPart_TextBox(CUIPart_TextBox::UITEXTBOX_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _bool bCenter, _bool bAutoRemove = false, _float fShowTime = 0.f)
	{
		return m_pFactory->MakeUIPart_TextBox(eType, fX, fY, fSizeX, fSizeY, bCenter, bAutoRemove, fShowTime);
	}

	// UI_Page
	CUIPage_Main* MakeUIPage_Main() { return m_pFactory->MakeUIPage_Main(); }
	CUIPage_Test* MakeUIPage_Test() { return m_pFactory->MakeUIPage_Test(); }
	CUIPage_Inven* MakeUIPage_Inven() { return m_pFactory->MakeUIPage_Inven(); }
	CUIPage_Equip* MakeUIPage_Equip() { return m_pFactory->MakeUIPage_Equip(); }
	CUIPage_Crafting* MakeUIPage_Crafting() { return m_pFactory->MakeUIPage_Crafting(); }
	CUIPage_Option* MakeUIPage_Option() { return m_pFactory->MakeUIPage_Option(); }
	CUIPage_ToolTip* MakeUIPage_ToolTip() { return m_pFactory->MakeUIPage_ToolTip(); }
#pragma endregion

#pragma region TERRAINMANAGER
	void SaveMap(const _char* pPath) { m_pTerrainManager->SaveMap(pPath); }
	void LoadMap(const _char* pPath) { m_pTerrainManager->LoadMap(pPath); }

	_float3 CheckPicking(_int iMode, _int iCX = -1, _int iCY = -1, _int iCZ = -1, _bool bTop = false) { return m_pTerrainManager->CheckPicking(iMode, iCX, iCY, iCZ, bTop); }

	void SetBedRock(_int iX, _int iY, _int iZ) { m_pTerrainManager->SetBedRock(iX, iY, iZ); }
	void HighLight_Surface(_bool bLinked) { m_pTerrainManager->HighLight_Surface(bLinked); } // <- bLinked�� true�� ��� �Ѳ����� ĥ�� ��, ������ ����Ǵ� ǥ���� ǥ�� 
	void HighLight_NewCube(_float3 fSize) { m_pTerrainManager->HighLight_NewCube(fSize); }
	void HighLight_DeleteCube(_float3 fSize) { m_pTerrainManager->HighLight_DeleteCube(fSize); }
	void Make_NewCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ, _int TextureIndex) { m_pTerrainManager->Make_NewCube(iX, iY, iZ, iCX, iCY, iCZ, TextureIndex); }
	void Make_DeleteCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ) { m_pTerrainManager->Make_DeleteCube(iX, iY, iZ, iCX, iCY, iCZ); }
	void Change_Surface(_bool bLinked) { m_pTerrainManager->Change_Surface(bLinked); } // <- bLinked�� true�� ��� �ؽ��� ��ȣ�� ���� ���� ǥ������ ����� ��� ǥ���� �ؽ��ĸ� ��ü 

	void Set_Object() { m_pTerrainManager->Set_Object(); }
	void Delete_Object() { m_pTerrainManager->Delete_Object(); }

	void MakeTestMap() { m_pTerrainManager->MakeTestMap(); }

	void ChangeIndex(_int iIndex) { m_pTerrainManager->ChangeIndex(iIndex); }

	// �� ���� 
	void Turn_Model(XMVECTOR fAxis, _float fTimeDelta)
	{

	}
	void Scaling_Model(_float Scale)
	{

	}
	void Change_Animation(_int iInput)
	{

	}

	_float3 Check_Terrain_Collision(_float3 fCenter, _float3 fExtents, _float3 vAdjustVector, LCUBEDIRECION* eDirec) { return m_pTerrainManager->Check_Terrain_Collision(fCenter, fExtents, vAdjustVector, eDirec); }
	_bool Check_OnGround(_float3 fCenter, _float3 fExtents) { return m_pTerrainManager->Check_OnGround(fCenter, fExtents); }
	void Destroy_Terrain_Explosion(_float3 fPosition, _float fRadius) { m_pTerrainManager->Destroy_Terrain_Explosion(fPosition, fRadius); }

#pragma endregion

#pragma region CAMERA
	void SetCameraMode(CFreeCamera::CAMERAMODE eInput) { m_pCamera->SetCameraMode(eInput); }
	void ShakeCamera(_float fDeltaTime) { m_pCamera->ShakeCamera(fDeltaTime); }
	_vector GetCameraPosition() { return m_pCamera->GetCameraPosition(); }
#pragma endregion



#pragma region PHYSICS
	void Set_Gravity_Direction(_float3 fDirection) { m_pPhysicsManager->Set_Gravity_Direction(fDirection); }
	void Set_Gravity_Power(_float fPower) { m_pPhysicsManager->Set_Gravity_Power(fPower); }
	_float3 Get_Gravity_Direction() { return m_pPhysicsManager->Get_Gravity_Direction(); }
	_float Get_Gravity_Power() { return m_pPhysicsManager->Get_Gravity_Power(); }

	CPhysicsManager::P_RESULT Total_Physics(CTransform& Transform, CCollider& Collder, _bool IsGravity, _bool IsTerrainCollision, _bool IsSlideControl, _float fTimeDelta)
	{
		return m_pPhysicsManager->Total_Physics(Transform, Collder, IsGravity, IsTerrainCollision, IsSlideControl, fTimeDelta);
	} // ��� ���� ����� ����(IsGravity - �߷� ����, IsTerrainCollision - ���� �浹 ����, IsSlideControl - ���� �浹 �� �ε巯�� ó��)

	CPhysicsManager::P_RESULT Make_P_Result(CTransform& Transform, CCollider& Collder) { return m_pPhysicsManager->Make_P_Result(Transform, Collder); } // ��� ��� ��Ͽ� ����ü ���� 
	void Update_By_P_Result(CTransform* Transform, CCollider* Collder, CPhysicsManager::P_RESULT tResult) { m_pPhysicsManager->Update_By_P_Result(Transform, Collder, tResult); }

	void Gravity(CPhysicsManager::P_RESULT* tResult, _bool IsTerrainCollision, _float fTimeDelta) { m_pPhysicsManager->Gravity(tResult, IsTerrainCollision, fTimeDelta); }
	void PushedPower(CPhysicsManager::P_RESULT* tResult, _float fTimeDelta) { m_pPhysicsManager->PushedPower(tResult, fTimeDelta); }// Ǫ�� �Ŀ� ���, ���� ȸ�� ���� ��� �ֱ� 
	void CheckTerrainCollision(CPhysicsManager::P_RESULT* tResult, _bool IsSlideControl) { m_pPhysicsManager->CheckTerrainCollision(tResult, IsSlideControl); } // ���� �浹�� ��� 
#pragma endregion

#pragma region UIMANAGER
	void ActivateCursor() { m_pUIManager->ActivateCursor(); }
	void ShowInformMessage(wstring Text) { m_pUIManager->ShowInformMessage(Text); }
	void ShowToolTip(_float fCellX, _float fCellY, ITEMARRAY eArray, _int iIndex) { m_pUIManager->ShowToolTip(fCellX, fCellY, eArray, iIndex); }
#pragma endregion



#pragma region ITEM
	const CItemManager::TINFO& GetItemInfo(ITEMINDEX eIndex) { return m_pItemManager->GetItemInfo(eIndex); }
	const CItemManager::TINFO& GetInvenInfo(_uint iInvenNum) { return m_pItemManager->GetInvenInfo(iInvenNum); }
	const CItemManager::TINFO& GetEquipInfo(EQUIPSLOT eSlot) { return m_pItemManager->GetEquipInfo(eSlot); }
	CTexture* GetItemInvenTexture(ITEMINDEX eIndex) { return m_pItemManager->GetItemInvenTexture(eIndex); }

	const CItemManager::TINFO PickItem(ITEMARRAY eArray, _int iIndex) { return m_pItemManager->PickItem(eArray, iIndex); }
	void CancelPick() { m_pItemManager->CancelPick(); }
	HRESULT PutInItem(ITEMARRAY eArray, _int iIndex) { return m_pItemManager->PutInItem(eArray, iIndex); }

	ITEMINDEX GetPickedItemIndex() { return m_pItemManager->GetPickedItemIndex(); }

	void InputRenderlist(ITEMINDEX eIndex, _uint* pParentState, const _float4x4* pMatrix, _float4x4& pParent) { return m_pItemManager->InputRenderlist(eIndex, pParentState, pMatrix, pParent); }

	wstring Get_TypeName(ITEMTYPE eType) { return m_pItemManager->Get_TypeName(eType); }
	wstring Get_TagName(ITEMTAG eType) { return m_pItemManager->Get_TagName(eType); }
	_bool Get_TagState(ITEMINDEX iIndex, ITEMTAG eType) { return m_pItemManager->Get_TagState(iIndex, eType);}

#pragma endregion


#pragma region INTERACTION
	void Input_ActionInfo(INTERACTION eInterType, CLongvinter_Container* pHost, _float3 fPosition, _float3 fPushedDirec, _float fPushedPower, _float fExtent, _float fDecreasePushedPower, CCollider::TYPE eColliderType = CCollider::TYPE_SPHERE, CInterAction::TERRAIN_ACTION eAction = CInterAction::TERRAIN_ACTION::ACTION_END)
	{
		m_pInterActionManager->Input_ActionInfo(eInterType, pHost, fPosition, fPushedDirec, fPushedPower, fExtent, fDecreasePushedPower, eColliderType, eAction);
	}
#pragma endregion

private: // <- ���� ���� ������ ��� 
	CGameInstance*			m_pGameInstance = { nullptr };
	CFreeCamera*			m_pCamera = { nullptr };
	CController*			m_pController = { nullptr };
	CFactory*				m_pFactory = { nullptr };
	CImguiManager*			m_pImguiManager = { nullptr };
	CTerrainManager*		m_pTerrainManager = { nullptr };
	CContainer_Player*		m_pPlayer = { nullptr };
	CPhysicsManager*		m_pPhysicsManager = { nullptr };
	CUIManager*				m_pUIManager = { nullptr };
	CItemManager*			m_pItemManager = { nullptr };
	CInterActionManager*	m_pInterActionManager = { nullptr };

private: // <- ���α׷� ���°���
	_bool					m_bLevelChanging = false;
	LEVELID					m_eLevel = LEVELID::LEVEL_END;


private: // <- ����̽� 
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END