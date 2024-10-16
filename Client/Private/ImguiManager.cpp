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

	//for (_int i = 0; i < _int(INTERACTION::INTER_END); ++i)
	//	szLandObject[i] = { "없음" };

	//szLandObject[_int(INTERACTION::INTER_APPLETREE)] = { "사과나무" };
	//szLandObject[_int(INTERACTION::INTER_ROCK)] = { "바위" };
	//szLandObject[_int(INTERACTION::INTER_BUSH)] = { "덤불" };
	//szLandObject[_int(INTERACTION::INTER_ITEM)] = { "아이템" };
	//szLandObject[_int(INTERACTION::INTER_TREE)] = { "나무" };


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

	if (ImGui::InputInt("RenderLimit", &m_iRenderLimit))
	{

	}

	if (ImGui::Button("Apply"))
	{
		GET_INSTANCE->Set_Render_Length(_float(m_iRenderLimit));
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
		ImGui::InputInt("Angle", &m_iRotate);

		if ((m_iRotate < 0) || (m_iRotate >= 360))
			m_iRotate = 0;

		if (m_iSelectObject == 4)
		{
			ImGui::Combo("LandObject", &m_iSelectLandObject, szLandObject, IM_ARRAYSIZE(szLandObject));
		}
	}
	else if (m_iSelectMode == 2)
	{
		
	}

	if (ImGui::Button("SAVE_MODEL"))
	{
		GET_INSTANCE->Save_Prototype_Model_Data();
	}

	

	ImGui::End();


	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON))
	{
		_bool bTop = m_iSelectMode == 1;

		if (m_iSelectObject != 4)
			GET_INSTANCE->CheckPicking(m_iSelectMode, m_iLand[0], m_iLand[1], m_iLand[2], bTop, CONTAINER(m_iSelectObject), INTERACTION::INTER_END, m_iRotate);
		else if (szLandObject[_int(m_iSelectLandObject)] != "없음")
			GET_INSTANCE->CheckPicking(m_iSelectMode, m_iLand[0], m_iLand[1], m_iLand[2], bTop, CONTAINER::CONTAINER_END, INTERACTION(m_iSelectLandObject), m_iRotate);
	}

	if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_RBUTTON))
	{
		_bool bTop = m_iSelectMode == 1;

		if (m_iSelectObject != 4)
			GET_INSTANCE->CheckPicking(m_iSelectMode, m_iLand[0], m_iLand[1], m_iLand[2], bTop, CONTAINER(m_iSelectObject));
		else if (szLandObject[_int(m_iSelectLandObject)] != "없음")
			GET_INSTANCE->CheckPicking(m_iSelectMode, m_iLand[0], m_iLand[1], m_iLand[2], bTop, CONTAINER::CONTAINER_END, INTERACTION(m_iSelectLandObject));
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
