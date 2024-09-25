#include "stdafx.h"
#include "..\Public\ClientInstance.h"
#include "Level_Loading.h"


IMPLEMENT_SINGLETON(CClientInstance)

CClientInstance::CClientInstance()
{
}

HRESULT CClientInstance::Setting_Program_Start(ID3D11Device** pDevice, ID3D11DeviceContext** pContext, CGameInstance* pInstance)
{
	m_pGameInstance = pInstance;
	m_pDevice = *pDevice;
	m_pContext = *pContext;

	// Factory
	m_pFactory = CFactory::Create(m_pDevice, m_pContext, pInstance);
	
	

	// Camera
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FreeCamera"), CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	CFreeCamera::CAMERA_FREE_DESC		Desc{};
	Desc.fSensor = 0.2f;
	Desc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 30.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FreeCamera"), &Desc)))
		return E_FAIL;
	m_pCamera = dynamic_cast<CFreeCamera*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Camera"), -1));
	

	// Contoller
	/*if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Controller"), CController::Create(pDevice, pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Controller"), TEXT("Prototype_GameObject_Controller"))))
		return E_FAIL;
	m_pController = dynamic_cast<CController*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Camera"), -1));
	Safe_AddRef(m_pController);*/

	// TerrainManager
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TerrainManager"), CTerrainManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// PhysicsManager
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PhysicsManager"), CPhysicsManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_PhysicsManager"), TEXT("Prototype_GameObject_PhysicsManager"))))
		return E_FAIL;
	m_pPhysicsManager = dynamic_cast<CPhysicsManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_PhysicsManager"), -1));




	// UIManager
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIManager"), CUIManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIManager"), TEXT("Prototype_GameObject_UIManager"))))
		return E_FAIL;
	m_pUIManager = dynamic_cast<CUIManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIManager"), -1));


	// ItemManager
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ItemManager"), CItemManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_ItemManager"), TEXT("Prototype_GameObject_ItemManager"))))
		return E_FAIL;
	m_pItemManager = dynamic_cast<CItemManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_ItemManager"), -1));



	return S_OK;
}

HRESULT CClientInstance::Setting_Ingame_Start()
{
	

	// TerrainManager
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_SINGLE), TEXT("Layer_TerrainManager"), TEXT("Prototype_GameObject_TerrainManager"))))
		return E_FAIL;
	m_pTerrainManager = dynamic_cast<CTerrainManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_SINGLE), TEXT("Layer_TerrainManager"), -1));
	m_pTerrainManager->LoadMap("../Bin/Resources/MapData/Map_1.dat");

	m_pCamera->SetCameraMode(CFreeCamera::CAMERAMODE::CAMERA_THIRD);

	/* 게임플레이 레벨에 필요한 광원을 준비한다. */
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 5.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(10.f, 3.f, 10.f, 1.f);
	//LightDesc.fRange = 7.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.4f, 0.2f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(20.f, 3.f, 10.f, 1.f);
	//LightDesc.fRange = 7.f;
	//LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.2f, 0.4f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;
}

HRESULT CClientInstance::Setting_Editor_Start()
{
	m_pCamera->SetCameraMode(CFreeCamera::CAMERAMODE::CAMERA_EDITOR);

	// TerrainManager
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_SINGLE), TEXT("Layer_TerrainManager"), TEXT("Prototype_GameObject_TerrainManager"))))
		return E_FAIL;
	m_pTerrainManager = dynamic_cast<CTerrainManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_SINGLE), TEXT("Layer_TerrainManager"), -1));
	

	// ImguiManager
	  // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_ImguiManager"), CImguiManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	else
		ImGui_ImplDX11_Init(m_pDevice, m_pContext);
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_EDITOR), TEXT("Layer_ImguiManager"), TEXT("Prototype_ImguiManager"))))
		return E_FAIL;
	else
		m_pImguiManager = dynamic_cast<CImguiManager*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_EDITOR), TEXT("Layer_ImguiManager"), -1));

	/* 게임플레이 레벨에 필요한 광원을 준비한다. */
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 5.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 3.f, 10.f, 1.f);
	LightDesc.fRange = 7.f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = /*_float4(0.4f, 0.2f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 3.f, 10.f, 1.f);
	LightDesc.fRange = 7.f;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = /*_float4(0.2f, 0.4f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

void CClientInstance::Release_CilentInstance()
{
	Safe_Release(m_pFactory);
	
	Safe_Release(m_pController);

	if (m_pImguiManager)
	{
		// Cleanup
		Safe_Release(m_pImguiManager);
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		
	}

	Safe_Release(m_pPlayer);

	GET_INSTANCE->Destroy_Instance();


}

HRESULT CClientInstance::ChangeLevel(LEVELID eLevel)
{
	m_eLevel = eLevel;
	return m_pGameInstance->Change_Level(_uint(LEVELID::LEVEL_LOADING), CLevel_Loading::Create(m_pDevice, m_pContext, eLevel));
}

void CClientInstance::Free()
{
	__super::Free();

	
}
