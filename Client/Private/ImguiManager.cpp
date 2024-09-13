#include "stdafx.h"
#include "..\Public\ImguiManager.h"

#include "GameInstance.h"
#include "Monster.h"
#include "ClientInstance.h"

CImguiManager::CImguiManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CImguiManager::CImguiManager(const CImguiManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CImguiManager::Initialize_Prototype()
{
	


	return S_OK;
}

HRESULT CImguiManager::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		Desc{};

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CImguiManager::Priority_Update(_float fTimeDelta)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 800), 0);

	ImGui::Begin("Editor Main");                          // Create a window called "Hello, world!" and append into it.

	if (ImGui::Button("SAVE_1")) { GET_INSTANCE->SaveMap("../Bin/Resources/MapData/Map_1.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("SAVE_2")) { GET_INSTANCE->SaveMap("../Bin/Resources/MapData/Map_2.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("SAVE_3")) { GET_INSTANCE->SaveMap("../Bin/Resources/MapData/Map_3.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("SAVE_4")) { GET_INSTANCE->SaveMap("../Bin/Resources/MapData/Map_4.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("SAVE_5")) { GET_INSTANCE->SaveMap("../Bin/Resources/MapData/Map_5.dat"); }


	if (ImGui::Button("LOAD_1")) { GET_INSTANCE->LoadMap("../Bin/Resources/MapData/Map_1.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("LOAD_2")) { GET_INSTANCE->LoadMap("../Bin/Resources/MapData/Map_2.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("LOAD_3")) { GET_INSTANCE->LoadMap("../Bin/Resources/MapData/Map_3.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("LOAD_4")) { GET_INSTANCE->LoadMap("../Bin/Resources/MapData/Map_4.dat"); }
	ImGui::SameLine();
	if (ImGui::Button("LOAD_5")) { GET_INSTANCE->LoadMap("../Bin/Resources/MapData/Map_5.dat"); }

	if (ImGui::InputInt3("BaseSize", m_iMax))
	{
		
	}

	if (ImGui::Button("Make Base"))
	{
		GET_INSTANCE->SetBedRock(m_iMax[0], m_iMax[1], m_iMax[2]);
	}
	
	ImGui::Combo("SelectMode", &m_iSelectMode, szMode, IM_ARRAYSIZE(szMode));

	if (m_iSelectMode == 0)
	{
		ImGui::Text("MakeTerrain");
		ImGui::InputInt3("LandSize", m_iLand);
		ImGui::Combo("SelectTexTure", &m_iSelectTexture, szTexture, IM_ARRAYSIZE(szTexture));
		GET_INSTANCE->ChangeIndex(m_iSelectTexture);
	}
	else if (m_iSelectMode == 1)
	{
		ImGui::Combo("SelectObject", &m_iSelectObject, szObject, IM_ARRAYSIZE(szObject));
	}
	else if (m_iSelectMode == 2)
	{
		if (ImGui::PushButtonRepeat)
		if (ImGui::Button("TURN_X"))
		{
			GET_INSTANCE->Turn_Model({ 1.f,0.f,0.f }, fTimeDelta);
		}
		ImGui::SameLine();
		if (ImGui::Button("TURN_Y"))
		{
			GET_INSTANCE->Turn_Model({ 0.f,1.f,0.f }, fTimeDelta);
		}
		ImGui::SameLine();
		if (ImGui::Button("TURN_Z"))
		{
			GET_INSTANCE->Turn_Model({ 0.f,0.f,1.f }, fTimeDelta);
		}
		
		if (ImGui::Button("Scale_Up"))
		{
			GET_INSTANCE->Scaling_Model(1.01f);
		}
		ImGui::SameLine();
		if (ImGui::Button("Scale_Down"))
		{
			GET_INSTANCE->Scaling_Model(0.99f);
		}

		if (ImGui::InputInt("BaseSize", &m_iAnimation))
		{

		}

		if (ImGui::Button("Change_Animation"))
		{
			GET_INSTANCE->Change_Animation(m_iAnimation);
		}


		if (ImGui::Button("Camera_First"))
		{
			GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERAMODE::CAMERA_FIRST);
		}
		ImGui::SameLine();
		if (ImGui::Button("Camera_Third"))
		{
			GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERAMODE::CAMERA_THIRD);
		}
		ImGui::SameLine();
		if (ImGui::Button("Camera_Editor"))
		{
			GET_INSTANCE->SetCameraMode(CFreeCamera::CAMERAMODE::CAMERA_EDITOR);
		}


	}

	if (ImGui::Button("SAVE_MODEL"))
	{
		GET_INSTANCE->Save_Prototype_Model_Data();
	}

	

	ImGui::End();


	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON))
	{
		_bool bTop = m_iSelectMode == 1;

		GET_INSTANCE->CheckPicking(m_iSelectMode, m_iLand[0], m_iLand[1], m_iLand[2], bTop);
	}
}

void CImguiManager::Update(_float fTimeDelta)
{

}

void CImguiManager::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CImguiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return S_OK;
}

HRESULT CImguiManager::Ready_Components()
{
	
	return S_OK;
}

CImguiManager* CImguiManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImguiManager* pInstance = new CImguiManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CImguiManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CImguiManager::Clone(void* pArg)
{
	CImguiManager* pInstance = new CImguiManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CImguiManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImguiManager::Free()
{
	__super::Free();


}
