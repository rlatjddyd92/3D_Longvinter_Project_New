#include "stdafx.h"
#include "..\Public\TerrainManager.h"

#include "GameInstance.h"
#include "ClientInstance.h"


CTerrainManager::CTerrainManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTerrainManager::CTerrainManager(const CTerrainManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTerrainManager::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CTerrainManager::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		Desc{};

	m_Components.clear();

	/* 직교퉁여을 위한 데이터들을 모두 셋하낟. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrainManager::Priority_Update(_float fTimeDelta)
{
	m_fInterval_Now -= fTimeDelta;

	if (m_fInterval_Now < 0.f)
		m_fInterval_Now = 0.f;


}

void CTerrainManager::Update(_float fTimeDelta)
{
	if (!m_CommandBuffer.empty())
		ChangeLandInfo_Normal();


	int a = 10;
}

void CTerrainManager::Late_Update(_float fTimeDelta)
{
	/* 직교투영을 위한 월드행렬까지 셋팅하게 된다. */
	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CTerrainManager::Render()
{
	_vector vCamera = GET_INSTANCE->GetCameraPosition();

	for (auto& iter : m_mapInstancing_SurFace)
	{
		_float3 fLook{};

		fLook.x = vCamera.m128_f32[0] - iter.second->m_vMat.m[3][0];
		fLook.y = vCamera.m128_f32[1] - iter.second->m_vMat.m[3][1];
		fLook.z = vCamera.m128_f32[2] - iter.second->m_vMat.m[3][2];

		_float Length = sqrt(pow(fLook.x, 2) + pow(fLook.y, 2) + pow(fLook.z, 2));

		if (m_fRender_Length >= 0.f)
		{
			if (Length > m_fRender_Length)
				continue;
		}

		_vector vPosition = { iter.second->m_vMat.m[3][0], iter.second->m_vMat.m[3][1], iter.second->m_vMat.m[3][2], iter.second->m_vMat.m[3][3] };

		if (GET_INSTANCE->IsBackOfCamera(vPosition))
			continue;
		


		LCUBEDIRECION Direc = LCUBEDIRECION(iter.first[iter.first.size() - 1] - '0');

		if (Direc == LCUBEDIRECION::LDIREC_BOTTOM)
			if (fLook.y > 0)
				continue;

		if (Direc == LCUBEDIRECION::LDIREC_TOP)
			if (fLook.y < 0)
				continue;

		if (Direc == LCUBEDIRECION::LDIREC_EAST)
			if (fLook.x < 0)
				continue;

		if (Direc == LCUBEDIRECION::LDIREC_WEST)
			if (fLook.x > 0)
				continue;

		if (Direc == LCUBEDIRECION::LDIREC_NORTH)
			if (fLook.z < 0)
				continue;

		if (Direc == LCUBEDIRECION::LDIREC_SOUTH)
			if (fLook.z > 0)
				continue;


		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &iter.second->m_vMat)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_DiffuseTexture", iter.second->m_iTextureNum)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;


	
		
	}

	return S_OK;
}

HRESULT CTerrainManager::RenderSurface(SURFACE* pSurface)
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &pSurface->m_vMat)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_DiffuseTexture", pSurface->m_iTextureNum)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;


	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
}

void CTerrainManager::SaveMap(const _char* pPath)
{
	string fileName = pPath;
	WCHAR* TempName = new WCHAR[fileName.size()];
	for (_int i = 0; i <= fileName.size(); ++i)
		TempName[i] = fileName[i];

	HANDLE hFile = CreateFile(TempName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwByte(0);

	for (_int i = 0; i < LMAX_X + 2; ++i)
		for (_int j = 0; j < LMAX_Y + 2; ++j)
			for (_int k = 0; k < LMAX_Z + 2; ++k)
			{
				WriteFile(hFile, &m_vecLcubeInfo[i][j][k].m_bLand, sizeof(_bool), &dwByte, nullptr);
				WriteFile(hFile, m_vecLcubeInfo[i][j][k].m_iTextureNum, sizeof(_int) * 6, &dwByte, nullptr);
			}

	_int iSize = m_vecObjInfo.size();
	WriteFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for (auto& iter : m_vecObjInfo)
	{
		WriteFile(hFile, &iter.eCon_Type, sizeof(CONTAINER), &dwByte, nullptr);
		WriteFile(hFile, &iter.eInter_Type, sizeof(INTERACTION), &dwByte, nullptr);
		WriteFile(hFile, &iter.fPosition, sizeof(_float3), &dwByte, nullptr);
		WriteFile(hFile, &iter.fRotate, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &iter.iIndex, sizeof(_int), &dwByte, nullptr);
	}
		

	CloseHandle(hFile);
}

void CTerrainManager::LoadMap(const _char* pPath)
{
	string fileName = pPath;
	WCHAR* TempName = new WCHAR[fileName.size() + 1];
	memset(TempName, 0, sizeof(TempName));
	for (_int i = 0; i <= fileName.size(); ++i)
		TempName[i] = fileName[i];

	HANDLE hFile = CreateFile(TempName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwByte(0); // 예외 처리 변수

	for (auto& iterX : m_vecLcubeInfo)
	{
		for (auto& iterY : iterX)
			iterY.clear();
		iterX.clear();
	}

	m_vecObjInfo.clear();

	m_vecLcubeInfo.clear();

	m_vecLcubeInfo.resize(LMAX_X + 2);

	for (int i = 0; i <= LMAX_X + 1; ++i)
	{
		m_vecLcubeInfo[i].resize(LMAX_Y + 2);
		for (int j = 0; j <= LMAX_Y + 1; ++j)
			m_vecLcubeInfo[i][j].resize(LMAX_Z + 2);
	}

	for (_int i = 0; i < LMAX_X + 2; ++i)
		for (_int j = 0; j < LMAX_Y + 2; ++j)
			for (_int k = 0; k < LMAX_Z + 2; ++k)
			{
				ReadFile(hFile, &m_vecLcubeInfo[i][j][k].m_bLand, sizeof(_bool), &dwByte, nullptr);
				ReadFile(hFile, m_vecLcubeInfo[i][j][k].m_iTextureNum, sizeof(_int) * 6, &dwByte, nullptr);

				if (m_vecLcubeInfo[i][j][k].m_bLand == true)
				{
					LCOMMAND tTemp = { LANDCOMMAND::LCOMMAND_ADD_LAND, {i,j,k} };
					memcpy(&tTemp.m_iTextureNum, m_vecLcubeInfo[i][j][k].m_iTextureNum, sizeof(_int) * 6);
					m_CommandBuffer.push_back(tTemp);
				}
			}

	_int iSize = 0;
	ReadFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < iSize; ++i)
	{
		MAKEOBJ tTemp(CONTAINER::CONTAINER_END, { 0.f,0.f,0.f }, 0.f, 0);
		ReadFile(hFile, &tTemp.eCon_Type, sizeof(CONTAINER), &dwByte, nullptr);
		ReadFile(hFile, &tTemp.eInter_Type, sizeof(INTERACTION), &dwByte, nullptr);
		ReadFile(hFile, &tTemp.fPosition, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &tTemp.fRotate, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &tTemp.iIndex, sizeof(_int), &dwByte, nullptr);
		m_vecObjInfo.push_back(tTemp);

		if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_PLAYER)
		{
			GET_INSTANCE->Make_Container_Player(tTemp.fPosition, tTemp.fRotate);
		}
		else if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_ANIMAL)
		{

		}
		else if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_ENEMY)
		{
			GET_INSTANCE->Make_Container_Enemy(tTemp.fPosition, ENEMY_TYPE::ENEMY_TYPE_END, tTemp.fRotate);
		}
		else if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_BOSS)
		{
			GET_INSTANCE->Make_Container_Boss(tTemp.fPosition, ENEMY_TYPE::ENEMY_TYPE_END, tTemp.fRotate);
		}
		else if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_END)
		{
			GET_INSTANCE->Add_InterActionObject_BySpec(tTemp.eInter_Type, nullptr, tTemp.fPosition, _float3(0.f, 0.f, 0.f), tTemp.fRotate);
		}


	}

	Safe_Delete_Array(TempName);
		
	CloseHandle(hFile);

}

void CTerrainManager::SetBedRock(_int iX, _int iY, _int iZ)
{
	if (iX > LMAX_X) iX = LMAX_X;
	if (iY > LMAX_Y) iY = LMAX_Y;
	if (iZ > LMAX_Z) iZ = LMAX_Z;

	for (_int i = 1; i <= iX; ++i)
		for (_int j = 1; j <= iY; ++j)
			for (_int k = 1; k <= iZ; ++k)
			{
				LCOMMAND tTemp = { LANDCOMMAND::LCOMMAND_ADD_LAND, {i,j,k},{0,0,0,0,0,0} };

				tTemp.m_iTextureNum[0] = _int(LANDNAME::LNAME_GRASS);

				for (_int z = 1; z < 6; ++z)
					tTemp.m_iTextureNum[z] = _int(LANDNAME::LNAME_DIRT_RED);

				m_CommandBuffer.push_back(tTemp);
			}


}

void CTerrainManager::HighLight_Surface(_bool bLinked)
{
}

void CTerrainManager::HighLight_NewCube(_float3 fSize)
{
}

void CTerrainManager::HighLight_DeleteCube(_float3 fSize)
{
}

void CTerrainManager::Make_NewCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ, _int TextureIndex)
{
	_int iMaxX = min(LMAX_X - 1, iX + iCX);
	_int iMaxY = min(LMAX_Y - 1, iY + iCY);
	_int iMaxZ = min(LMAX_Z - 1, iZ + iCZ);

	for (_int i = iX; i < iMaxX; ++i)
		for (_int j = iY; j < iMaxY; ++j)
			for (_int k = iZ; k < iMaxZ; ++k)
				if (m_vecLcubeInfo[i][j][k].m_bLand == false)
				{
					LCOMMAND tTemp = {};
					tTemp.m_eType = LANDCOMMAND::LCOMMAND_ADD_LAND;
					tTemp.m_vIndex[0] = i;
					tTemp.m_vIndex[1] = j;
					tTemp.m_vIndex[2] = k;


					if (TextureIndex != _int(LANDNAME::LNAME_GRASS))
					{
						for (_int z = 0; z < 6; ++z)
							tTemp.m_iTextureNum[z] = TextureIndex;
					}
					else
					{
						tTemp.m_iTextureNum[0] = TextureIndex;

						for (_int z = 1; z < 6; ++z)
							tTemp.m_iTextureNum[z] = _int(LANDNAME::LNAME_DIRT_RED);
					}


					m_CommandBuffer.push_back(tTemp);
				}
	
}

void CTerrainManager::Make_DeleteCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ)
{
	_int iMaxX = min(LMAX_X - 1, iX + iCX);
	_int iMaxY = min(LMAX_Y - 1, iY + iCY);
	_int iMaxZ = min(LMAX_Z - 1, iZ + iCZ);

	for (_int i = iX; i < iMaxX; ++i)
		for (_int j = iY; j < iMaxY; ++j)
			for (_int k = iZ; k < iMaxZ; ++k)
				if (m_vecLcubeInfo[i][j][k].m_bLand == true)
				{
					LCOMMAND tTemp = {};
					tTemp.m_eType = LANDCOMMAND::LCOMMAND_REMOVE_LAND;
					tTemp.m_vIndex[0] = i;
					tTemp.m_vIndex[1] = j;
					tTemp.m_vIndex[2] = k;
					m_CommandBuffer.push_back(tTemp);
				}
}

void CTerrainManager::Change_Surface(_bool bLinked)
{
}

void CTerrainManager::Destroy_Terrain_Explosion(_float3 fPosition, _float fRadius)
{
	_int iMinX = max(0, (fPosition.x - fRadius) / LCUBESIZE);
	_int iMaxX = min(LMAX_X, (fPosition.x + fRadius) / LCUBESIZE);
	_int iMinY = max(m_iBedRock, (fPosition.y - fRadius) / LCUBESIZE);
	_int iMaxY = min(LMAX_Y, (fPosition.y + fRadius) / LCUBESIZE);
	_int iMinZ = max(0, (fPosition.z - fRadius) / LCUBESIZE);
	_int iMaxZ = min(LMAX_Z, (fPosition.z + fRadius) / LCUBESIZE);

	Adjust_Index(&iMinX, &iMinY, &iMinZ);
	Adjust_Index(&iMaxX, &iMaxY, &iMaxZ);

	for (_int i = iMinX; i <= iMaxX;++i)
		for (_int j = iMinY; j <= iMaxY; ++j)
			for (_int k = iMinZ; k <= iMaxZ; ++k)
				if (m_vecLcubeInfo[i][j][k].m_bLand == true)
				{
					_float3 fCubePosition = { i + 0.5f, j + 0.5f, k + 0.5f };
					_float fDistance = sqrt(pow(fCubePosition.x - fPosition.x, 2) + pow(fCubePosition.y - fPosition.y, 2) + pow(fCubePosition.z - fPosition.z, 2));

					if (fDistance < fRadius + (LCUBESIZE * 0.5f))
					{
						LCOMMAND tTemp = {};
						tTemp.m_eType = LANDCOMMAND::LCOMMAND_REMOVE_LAND;
						tTemp.m_vIndex[0] = i;
						tTemp.m_vIndex[1] = j;
						tTemp.m_vIndex[2] = k;
						for (_int i = 0; i < 6; ++i)
							tTemp.m_iTextureNum[i] = 5;

						m_CommandBuffer.push_back(tTemp);
					}
				}
}

void CTerrainManager::Set_Object()
{
}

void CTerrainManager::Delete_Object()
{
}

void CTerrainManager::MakeTestMap()
{
	
}

_wstring CTerrainManager::MakeKey(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec)
{
	_wstring strKey = {};

	strKey += 'x';
	for (_int i = 4; i >= 0; --i)
	{
		strKey += (iX / pow(10, i)) + '0';
		iX %= _int(pow(10, i));
	}
	strKey += 'y';
	for (_int i = 4; i >= 0; --i)
	{
		strKey += (iY / pow(10, i)) + '0';
		iY %= _int(pow(10, i));
	}
	strKey += 'z';
	for (_int i = 4; i >= 0; --i)
	{
		strKey += (iZ / pow(10, i)) + '0';
		iZ %= _int(pow(10, i));
	}
	strKey += 'd';
	strKey += _int(eDirec) + '0';

	return strKey;
}

void CTerrainManager::InterpretKey(_wstring Key, _int* iX, _int* iY, _int* iZ, LCUBEDIRECION* eDirec)
{
	_int iIndex = 1;
	for (_int i = 4; i >= 0; --i)
	{
		*iX += (Key[iIndex] - '0') * _int(pow(10, i));
		++iIndex;
	}
	++iIndex;
	for (_int i = 4; i >= 0; --i)
	{
		*iY += (Key[iIndex] - '0') * _int(pow(10, i));
		++iIndex;
	}
	++iIndex;
	for (_int i = 4; i >= 0; --i)
	{
		*iZ += (Key[iIndex] - '0') * _int(pow(10, i));
		++iIndex;
	}
	++iIndex;
	*eDirec = LCUBEDIRECION((Key[iIndex] - '0'));
}

void CTerrainManager::SetPosition_Surface(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec, _float4x4* fSurface)
{
	XMVECTOR NewPosition = { LCUBESIZE * 0.5f,  LCUBESIZE * 0.5f ,  LCUBESIZE * 0.5f };
	NewPosition += {iX* LCUBESIZE, iY* LCUBESIZE, iZ* LCUBESIZE};

	if (eDirec == LCUBEDIRECION::LDIREC_TOP)
	{
		m_pTransformCom->Rotation(_fvector({ 1.f, 0.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { 0, LCUBESIZE * 0.5f, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_BOTTOM)
	{
		m_pTransformCom->Rotation(_fvector({ -1.f, 0.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { 0, -LCUBESIZE * 0.5f, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_WEST)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, 1.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { -LCUBESIZE * 0.5f, 0, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_EAST)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, -1.f, 0.f }), _float(XMConvertToRadians(90.f)));
		NewPosition += { LCUBESIZE * 0.5f, 0, 0 };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_NORTH)
	{
		m_pTransformCom->Rotation(_fvector({ 0.f, -1.f, 0.f }), _float(XMConvertToRadians(180.f)));
		NewPosition += { 0, 0, LCUBESIZE * 0.5f };
	}
	else if (eDirec == LCUBEDIRECION::LDIREC_SOUTH)
	{
		NewPosition += { 0, 0, -LCUBESIZE * 0.5f };
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, NewPosition);

	memcpy(fSurface, &m_pTransformCom->Get_WorldMatrix(), sizeof(_float4x4));
	_float4x4 vTemp{};
	XMStoreFloat4x4(&vTemp, XMMatrixIdentity());
	m_pTransformCom->Set_WorldMatrix(vTemp);
}

_float3 CTerrainManager::IsPicking_Instancing(SURFACE* pSurface)
{
	_float3 vPickPos{ -1,-1,-1 };

	m_pVIBufferCom->isPicking(pSurface->m_vMat, &vPickPos);
	return vPickPos;
}

void CTerrainManager::Adjust_Index(_int* iX, _int* iY, _int* iZ)
{
	if (*iX < 0.f) *iX = 0.f;
	if (*iY < 0.f) *iY = 0.f;
	if (*iZ < 0.f) *iZ = 0.f;

	*iX = min(*iX, m_vecLcubeInfo.size() - 1);
	*iY = min(*iY, m_vecLcubeInfo[*iX].size() - 1);
	*iZ = min(*iZ, m_vecLcubeInfo[*iX][*iY].size() - 1);
	*iX = max(*iX, 0);
	*iY = max(*iY, 0);
	*iZ = max(*iZ, 0);
}

void CTerrainManager::Delete_LastObject()
{
	if (m_vecObjInfo.empty())
		return;

	MAKEOBJ tTemp = m_vecObjInfo.back();

	if (tTemp.eInter_Type != INTERACTION::INTER_END)
	{
		GET_INSTANCE->Delete_LastInterAction(tTemp.eInter_Type);
		m_vecObjInfo.pop_back();
	}
	else if (tTemp.eCon_Type == CONTAINER::CONTAINER_PLAYER)
	{
		CGameObject* pTemp = m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), -1);
		m_pGameInstance->Delete_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), pTemp);
		m_vecObjInfo.pop_back();
	}
	else if (tTemp.eCon_Type == CONTAINER::CONTAINER_ENEMY)
	{
		CGameObject* pTemp = m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), -1);
		m_pGameInstance->Delete_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), pTemp);
		m_vecObjInfo.pop_back();
	}
	else if (tTemp.eCon_Type == CONTAINER::CONTAINER_BOSS)
	{
		CGameObject* pTemp = m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Boss"), -1);
		m_pGameInstance->Delete_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Boss"), pTemp);
		m_vecObjInfo.pop_back();
	}




}

void CTerrainManager::ShowIndex(_int iX, _int iZ, _matrix mHost)
{
	_matrix mProj = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ));
	_matrix mView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));

	_vector vResult = { 0.f,  0.f,  0.f, 0.f };

	// 투영 좌표 계산
	vResult = XMVector3Transform(vResult, mHost);
	vResult = XMVector3Transform(vResult, mView);
	vResult = XMVector3Transform(vResult, mProj);



	// W나누기
	_float4 fResult{};
	XMStoreFloat4(&fResult, vResult);

	if (fResult.w < 0.f)
		return;


	_float m_fX = fResult.x / fResult.w;
	_float m_fY = fResult.y / fResult.w;

	// 스크린 좌표로 변환
	m_fX = ((m_fX + 1.f) * 0.5) * 1280.f;
	m_fY = ((1.f - m_fY) * 0.5) * 720.f;

	
	string strX = to_string(iX);
	string strZ = to_string(iZ);

	strX += ":";
	strX += strZ;

	_tchar* tIndex = new _tchar[strX.size() + 1];
	
	for (_int i = 0; i < strX.size() + 1; ++i)
		tIndex[i] = strX[i];

	m_pGameInstance->Render_Text(TEXT("Font_Test1"), tIndex, { m_fX , m_fY,0.f , 0.f }, 0.5f);

	Safe_Delete_Array(tIndex);
}


_float3 CTerrainManager::Check_Terrain_Collision_Adjust(_float3 fCenter, _float3 fExtents, _float3 vAdjustVector, LCUBEDIRECION* eDirec)
{
	_float3 vCenter = fCenter;
	_float3 vAdjustPosition = { 0.f,0.f,0.f };
	_float vAdjustScale = 0.f;
	LCUBEDIRECION eCollisionDirec = LCUBEDIRECION::LDIREC_END;
	_float3 vExtents = fExtents;

	// 1. 객체가 존재할 수 있는 큐브 인덱스 범위 잡기 
	_int iMinX = (vCenter.x - vExtents.x) / LCUBESIZE;
	_int iMaxX = (vCenter.x + vExtents.x) / LCUBESIZE;
	_int iMinY = (vCenter.y - vExtents.y) / LCUBESIZE;
	_int iMaxY = (vCenter.y + vExtents.y) / LCUBESIZE;
	_int iMinZ = (vCenter.z - vExtents.z) / LCUBESIZE;
	_int iMaxZ = (vCenter.z + vExtents.z) / LCUBESIZE;

	Adjust_Index(&iMinX, &iMinY, &iMinZ);
	Adjust_Index(&iMaxX, &iMaxY, &iMaxZ);

	/*if ((iMinX < 0.f) + (iMinY < 0.f) + (iMinZ < 0.f) > 0)
		return TERRAINERROR;

	if (iMaxX >= LMAX_X)
		return TERRAINERROR;

	if (iMaxY >= LMAX_Y)
		return TERRAINERROR;

	if (iMaxZ >= LMAX_Z)
		return TERRAINERROR;*/

	// 2. 겹치는 지형 큐브가 있는 지 확인 
	for (_uint i = iMinX; i <= iMaxX; ++i)
		for (_uint j = iMinY; j <= iMaxY; ++j)
			for (_uint k = iMinZ; k <= iMaxZ; ++k)
			{
				if (!m_vecLcubeInfo[i][j][k].m_bLand) continue;

				_float fTempX = 0.f;
				_float fTempY = 0.f;
				_float fTempZ = 0.f;

				if (vAdjustVector.x > 0)
					fTempX = -((vCenter.x - vExtents.x) - (_float(i + 1) * LCUBESIZE));
				else if (vAdjustVector.x < 0)
					fTempX = ((_float(i) * LCUBESIZE) - (vCenter.x + vExtents.x));

				if (vAdjustVector.y > 0)
					fTempY = -((vCenter.y - vExtents.y) - (_float(j + 1) * LCUBESIZE));
				else if (vAdjustVector.y < 0)
					fTempY = ((_float(j) * LCUBESIZE) - (vCenter.y + vExtents.y));

				if (vAdjustVector.z > 0)
					fTempZ = -((vCenter.z - vExtents.z) - (_float(k + 1) * LCUBESIZE));
				else if (vAdjustVector.z < 0)
					fTempZ = ((_float(k) * LCUBESIZE) - (vCenter.z + vExtents.z));


				_float3 vTempPosition = { 0.f,0.f,0.f };
				_float vTempScale = 0.f;
				LCUBEDIRECION eTempDIrec = LCUBEDIRECION::LDIREC_END;

				_float3 fTemp = { fTempX, fTempX * (vAdjustVector.y / vAdjustVector.x), fTempX * (vAdjustVector.z / vAdjustVector.x) };

				if ((vCenter.y - vExtents.y) + fTemp.y <= (_float(j + 1) * LCUBESIZE) || ((_float(j) * LCUBESIZE) <= (vCenter.y + vExtents.y) + fTemp.y))
					if ((vCenter.z - vExtents.z) + fTemp.z <= (_float(k + 1) * LCUBESIZE) || ((_float(k) * LCUBESIZE) <= (vCenter.z + vExtents.z) + fTemp.z))
					{
						_float fNowTempScale = pow(fTemp.x, 2) + pow(fTemp.y, 2) + pow(fTemp.z, 2);

						if ((!vTempScale) || (vTempScale > fNowTempScale))
						{
							vTempScale = fNowTempScale;
							vTempPosition = fTemp;

							if (eDirec != nullptr)
							{
								if (vTempPosition.x < 0)
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_WEST;
								}
								else
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_EAST;
								}

							}
						}
					}

				fTemp = { fTempY * (vAdjustVector.x / vAdjustVector.y) , fTempY, fTempY * (vAdjustVector.z / vAdjustVector.y) };

				if ((vCenter.x - vExtents.x) + fTemp.x <= (_float(i + 1) * LCUBESIZE) || ((_float(i) * LCUBESIZE) <= (vCenter.x + vExtents.x) + fTemp.x))
					if ((vCenter.z - vExtents.z) + fTemp.z <= (_float(k + 1) * LCUBESIZE) || ((_float(k) * LCUBESIZE) <= (vCenter.z + vExtents.z) + fTemp.z))
					{
						_float fNowTempScale = pow(fTemp.x, 2) + pow(fTemp.y, 2) + pow(fTemp.z, 2);

						if ((!vTempScale) || (vTempScale > fNowTempScale))
						{
							vTempScale = fNowTempScale;
							vTempPosition = fTemp;

							if (eDirec != nullptr)
							{
								if (vTempPosition.y < 0)
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_BOTTOM;
								}
								else
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_TOP;
								}
							}
						}
					}

				fTemp = { fTempZ * (vAdjustVector.x / vAdjustVector.z) , fTempZ * (vAdjustVector.y / vAdjustVector.z), fTempZ };

				if ((vCenter.x - vExtents.x) + fTemp.x <= (_float(i + 1) * LCUBESIZE) || ((_float(i) * LCUBESIZE) <= (vCenter.x + vExtents.x) + fTemp.x))
					if ((vCenter.y - vExtents.y) + fTemp.y <= (_float(j + 1) * LCUBESIZE) || ((_float(j) * LCUBESIZE) <= (vCenter.y + vExtents.y) + fTemp.y))
					{
						_float fNowTempScale = pow(fTemp.x, 2) + pow(fTemp.y, 2) + pow(fTemp.z, 2);

						if ((!vTempScale) || (vTempScale > fNowTempScale))
						{
							vTempScale = fNowTempScale;
							vTempPosition = fTemp;

							if (eDirec != nullptr)
							{
								if (vTempPosition.z < 0)
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_SOUTH;
								}
								else
								{
									eTempDIrec = LCUBEDIRECION::LDIREC_NORTH;
								}
							}
						}
					}

				if ((!vAdjustScale) || (vAdjustScale < vTempScale))
				{
					vAdjustScale = vTempScale;
					vAdjustPosition = vTempPosition;

					if (eDirec != nullptr)
						eCollisionDirec = eTempDIrec;
				}

				if (abs(vAdjustVector.x) + abs(vAdjustVector.y) + abs(vAdjustVector.z) > 1.f)
					printf_s("[보정] %f %f %f [결과] %f %f %f \n", vAdjustVector.x, vAdjustVector.y, vAdjustVector.z, vAdjustPosition.x, vAdjustPosition.y, vAdjustPosition.z);
			}

	if (vAdjustPosition.x > 0)
		vAdjustPosition.x += GAPFROMTERRAIN;
	else if (vAdjustPosition.x < 0)
		vAdjustPosition.x -= GAPFROMTERRAIN;

	if (vAdjustPosition.y > 0)
		vAdjustPosition.y += GAPFROMTERRAIN;
	else if (vAdjustPosition.y < 0)
		vAdjustPosition.y -= GAPFROMTERRAIN;

	if (vAdjustPosition.z > 0)
		vAdjustPosition.z += GAPFROMTERRAIN;
	else if (vAdjustPosition.z < 0)
		vAdjustPosition.z -= GAPFROMTERRAIN;

	if (eCollisionDirec == LCUBEDIRECION::LDIREC_END)
		return TERRAINERROR;

	if (eDirec != nullptr)
		*eDirec = eCollisionDirec;
	

	return vAdjustPosition;
}

_bool CTerrainManager::Check_OnGround(_float3 fCenter, _float3 fExtents)
{
	_float fX = (fCenter.x - fExtents.x);
	_uint iMinX = (fCenter.x - fExtents.x) / LCUBESIZE;
	_uint iMaxX = (fX + fExtents.x * 2.f) / LCUBESIZE;

	_float fY = (fCenter.y - fExtents.y);
	_uint iY = (fCenter.y - fExtents.y) / LCUBESIZE;
	if (((fY - iY) >= 0) && ((fY - iY) < GAPFROMTERRAIN * 1.1f))
		--iY;

	_float fZ = (fCenter.z - fExtents.z);
	_uint iMinZ = (fCenter.z - fExtents.z) / LCUBESIZE;
	_uint iMaXZ = (fZ + fExtents.z * 2.f) / LCUBESIZE;

	if ((iMinX < 0) || (iMaxX >= LMAX_X))
		return true;

	if ((iY < 0) || (iY >= LMAX_Y + 5))
		return true;

	if ((iMinZ < 0) || (iMaXZ >= LMAX_Z))
		return true;

	for (_uint i = iMinX; i <= iMaxX; ++i)
		for (_uint j = iMinZ; j <= iMaXZ; ++j)
			if (m_vecLcubeInfo[i][iY][j].m_bLand)
				return true;

	return false;
}

_bool CTerrainManager::Check_Wall(_float3 fCenter, _float3 fLook, _float fRange)
{
	fLook.x *= fRange;
	fLook.y *= fRange;
	fLook.z *= fRange;

	_float3 fPosition{};
	XMStoreFloat3(&fPosition, XMLoadFloat3(&fCenter) + XMLoadFloat3(&fLook));



	return Check_IsTerrain(fPosition);
}

_bool CTerrainManager::Check_Terrain_Collision(_float3 fCenter, _float3 fExtents)
{
	_float3 vCenter = fCenter;
	_float3 vExtents = fExtents;

	// 1. 객체가 존재할 수 있는 큐브 인덱스 범위 잡기 
	_uint iMinX = (vCenter.x - vExtents.x) / LCUBESIZE;
	_uint iMaxX = (vCenter.x + vExtents.x) / LCUBESIZE;
	_uint iMinY = (vCenter.y - vExtents.y) / LCUBESIZE;
	_uint iMaxY = (vCenter.y + vExtents.y) / LCUBESIZE;
	_uint iMinZ = (vCenter.z - vExtents.z) / LCUBESIZE;
	_uint iMaxZ = (vCenter.z + vExtents.z) / LCUBESIZE;

	if (iMinX * iMinY * iMaxZ < 0)
		return false;

	if (iMaxX >= LMAX_X)
		return false;

	if (iMaxY >= LMAX_Y)
		return false;

	if (iMaxZ >= LMAX_Z)
		return false;

	// 2. 겹치는 지형 큐브가 있는 지 확인 
	for (_uint i = iMinX; i <= iMaxX; ++i)
		for (_uint j = iMinY; j <= iMaxY; ++j)
			for (_uint k = iMinZ; k <= iMaxZ; ++k)
				if (m_vecLcubeInfo[i][j][k].m_bLand == true)
				{
					if ((fCenter.x - fExtents.x > _float((i + 1) * LCUBESIZE)) || (fCenter.x + fExtents.x < _float((i)*LCUBESIZE)))
						if ((fCenter.y - fExtents.y > _float((j + 1) * LCUBESIZE)) || (fCenter.y + fExtents.y < _float((j)*LCUBESIZE)))
							if ((fCenter.z - fExtents.z > _float((k + 1) * LCUBESIZE)) || (fCenter.z + fExtents.z < _float((k)*LCUBESIZE)))
								return false;
					return true;
				}

	return false;
}

HRESULT CTerrainManager::Ready_Components()
{
	m_vecLcubeInfo.resize(LMAX_X + 2);

	for (int i = 0; i <= LMAX_X + 1; ++i)
	{
		m_vecLcubeInfo[i].resize(LMAX_Y + 2);
		for (int j = 0; j <= LMAX_Y + 1; ++j)
			m_vecLcubeInfo[i][j].resize(LMAX_Z + 2);
	}

	/* FOR.Com_Texture */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LandTexture"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxSurFace"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxSurFace"),
		TEXT("Com_Shader_1"), reinterpret_cast<CComponent**>(&m_pShaderCom_Gray))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect3D"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Sky"), TEXT("Prototype_GameObject_Sky"));

	m_pSky = static_cast<CSky*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Sky"), -1));

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(0.5f, 1.0f, 0.5f);
	ColliderDesc.vCenter = _float3(0.0f, ColliderDesc.vExtents.y, 0.0f);

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CTerrainManager::ChangeLandInfo_Normal()
{
	for (auto& iter : m_CommandBuffer)
	{
		if (iter.m_eType == LANDCOMMAND::LCOMMAND_ADD_LAND)
			m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand = true;
		if (iter.m_eType == LANDCOMMAND::LCOMMAND_REMOVE_LAND)
			m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand = false;

		for (int i = 0; i < 6; ++i)
			m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2]].m_iTextureNum[i] = iter.m_iTextureNum[i];
	}

	for (auto& iter : m_CommandBuffer)
	{
		if (iter.m_eType == LANDCOMMAND::LCOMMAND_ADD_LAND)
		{
			_wstring strKey = MakeKey(iter.m_vIndex[0], iter.m_vIndex[1], iter.m_vIndex[2], LCUBEDIRECION::LDIREC_TOP);
			queue<LCUBEDIRECION> queueNew;
			queue<LCUBEDIRECION> queueDelete;

			if ((iter.m_vIndex[0] > 0) && (m_vecLcubeInfo[iter.m_vIndex[0] - 1][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_WEST);
			else 
				queueDelete.push(LCUBEDIRECION::LDIREC_EAST);
			if ((iter.m_vIndex[1] > 0) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1] - 1][iter.m_vIndex[2]].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_BOTTOM);
			else
				queueDelete.push(LCUBEDIRECION::LDIREC_TOP);
			if ((iter.m_vIndex[2] > 0) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2] - 1].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_SOUTH);
			else
				queueDelete.push(LCUBEDIRECION::LDIREC_NORTH);
			if ((iter.m_vIndex[0] < LMAX_X + 1) && (m_vecLcubeInfo[iter.m_vIndex[0] + 1][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_EAST);
			else
				queueDelete.push(LCUBEDIRECION::LDIREC_WEST);
			if ((iter.m_vIndex[1] < LMAX_Y + 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1] + 1][iter.m_vIndex[2]].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_TOP);
			else
				queueDelete.push(LCUBEDIRECION::LDIREC_BOTTOM);
			if ((iter.m_vIndex[2] < LMAX_Z + 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2] + 1].m_bLand == false))
				queueNew.push(LCUBEDIRECION::LDIREC_NORTH);
			else
				queueDelete.push(LCUBEDIRECION::LDIREC_SOUTH);

			while (!queueNew.empty())
			{
				LCUBEDIRECION eNow = queueNew.front();
				queueNew.pop();
				// 인스턴싱 
				SURFACE* pNew = new SURFACE;
				SetPosition_Surface(iter.m_vIndex[0], iter.m_vIndex[1], iter.m_vIndex[2], eNow, &pNew->m_vMat);
				pNew->m_iTextureNum = iter.m_iTextureNum[_int(eNow)];

				strKey[strKey.size() - 1] = _int(eNow) + '0';

				m_mapInstancing_SurFace.insert({ strKey ,pNew });




				//// 기존 방식 
				//m_pGameInstance->Add_CloneObject_ToLayer(_int(LEVELID::LEVEL_SINGLE), TEXT("Layer_SurFace"), TEXT("Prototype_GameObject_SurFace"));
				//CSurFace* pNew = dynamic_cast<CSurFace*>(m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_SINGLE), TEXT("Layer_SurFace"), -1));
				//pNew->SetPosition(iter.m_vIndex[0], iter.m_vIndex[1], iter.m_vIndex[2], eNow, m_pTextureCom);
				//pNew->SetTextureIndex(iter.m_iTextureNum[_int(eNow)]);

				//strKey[strKey.size() - 1] = _int(eNow) + '0';
				//m_mapSurFace.insert({ strKey ,pNew });
				//Safe_AddRef(pNew);
			}

			while (!queueDelete.empty())
			{
				LCUBEDIRECION eNow = queueDelete.front();
				queueDelete.pop();

				_int iX = eNow == LCUBEDIRECION::LDIREC_WEST ? 1 : 0;
				_int iY = eNow == LCUBEDIRECION::LDIREC_BOTTOM ? 1 : 0;
				_int iZ = eNow == LCUBEDIRECION::LDIREC_SOUTH ? 1 : 0;
				if (!iX)
					iX = eNow == LCUBEDIRECION::LDIREC_EAST ? -1 : 0;
				if (!iY)
					iY = eNow == LCUBEDIRECION::LDIREC_TOP ? -1 : 0;
				if (!iZ)
					iZ = eNow == LCUBEDIRECION::LDIREC_NORTH ? -1 : 0;

				strKey = MakeKey(iter.m_vIndex[0] + iX, iter.m_vIndex[1] + iY, iter.m_vIndex[2] + iZ, eNow);

				if (m_mapInstancing_SurFace.find(strKey) != m_mapInstancing_SurFace.end())
				{
					Safe_Delete(m_mapInstancing_SurFace.find(strKey)->second);
					m_mapInstancing_SurFace.erase(m_mapInstancing_SurFace.find(strKey));
				}


				/*if (m_mapSurFace.find(strKey) != m_mapSurFace.end())
				{
					m_mapSurFace.find(strKey)->second->RemoveSurFace();
					Safe_Release(m_mapSurFace.find(strKey)->second);
					m_mapSurFace.erase(m_mapSurFace.find(strKey));
				}*/
			}
		}

		if (iter.m_eType == LANDCOMMAND::LCOMMAND_REMOVE_LAND)
		{
			_wstring strKey = MakeKey(iter.m_vIndex[0], iter.m_vIndex[1], iter.m_vIndex[2],LCUBEDIRECION::LDIREC_TOP);
			for (_int i = 0; i < 6; ++i)
			{
				

				if (m_mapInstancing_SurFace.find(strKey) != m_mapInstancing_SurFace.end())
				{
					Safe_Delete(m_mapInstancing_SurFace.find(strKey)->second);
					m_mapInstancing_SurFace.erase(m_mapInstancing_SurFace.find(strKey));
				}

				strKey[strKey.size() - 1] = strKey[strKey.size() - 1] + 1;

				/*if (m_mapSurFace.find(strKey) != m_mapSurFace.end())
				{
					m_mapSurFace.find(strKey)->second->RemoveSurFace();
					Safe_Release(m_mapSurFace.find(strKey)->second);
					m_mapSurFace.erase(m_mapSurFace.find(strKey));
				}*/
			}
					
			queue<LCUBEDIRECION> queueNew;

			if ((iter.m_vIndex[0] > 1) && (m_vecLcubeInfo[iter.m_vIndex[0] - 1][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_EAST);
			if ((iter.m_vIndex[1] > 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1] - 1][iter.m_vIndex[2]].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_TOP);
			if ((iter.m_vIndex[2] > 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2] - 1].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_NORTH);
			if ((iter.m_vIndex[0] < LMAX_X + 1) && (m_vecLcubeInfo[iter.m_vIndex[0] + 1][iter.m_vIndex[1]][iter.m_vIndex[2]].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_WEST);
			if ((iter.m_vIndex[1] < LMAX_Y + 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1] + 1][iter.m_vIndex[2]].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_BOTTOM);
			if ((iter.m_vIndex[2] < LMAX_Z + 1) && (m_vecLcubeInfo[iter.m_vIndex[0]][iter.m_vIndex[1]][iter.m_vIndex[2] + 1].m_bLand == true))
				queueNew.push(LCUBEDIRECION::LDIREC_SOUTH);

			while (!queueNew.empty())
			{
				LCUBEDIRECION eNow = queueNew.front();
				queueNew.pop();
				_int iX = eNow == LCUBEDIRECION::LDIREC_WEST ? 1 : 0;
				_int iY = eNow == LCUBEDIRECION::LDIREC_BOTTOM ? 1 : 0;
				_int iZ = eNow == LCUBEDIRECION::LDIREC_SOUTH ? 1 : 0;
				if (!iX)
					iX = eNow == LCUBEDIRECION::LDIREC_EAST ? -1 : 0;
				if (!iY)
					iY = eNow == LCUBEDIRECION::LDIREC_TOP ? -1 : 0;
				if (!iZ)
					iZ = eNow == LCUBEDIRECION::LDIREC_NORTH ? -1 : 0;

				strKey = MakeKey(iter.m_vIndex[0] + iX, iter.m_vIndex[1] + iY, iter.m_vIndex[2] + iZ, eNow);

				// 인스턴싱 
				SURFACE* pNew = new SURFACE;
				SetPosition_Surface(iter.m_vIndex[0] + iX, iter.m_vIndex[1] + iY, iter.m_vIndex[2] + iZ, eNow, &pNew->m_vMat);
				pNew->m_iTextureNum = iter.m_iTextureNum[_int(eNow)];
				m_mapInstancing_SurFace.insert({ strKey ,pNew });

				/*m_pGameInstance->Add_CloneObject_ToLayer(_int(LEVELID::LEVEL_SINGLE), TEXT("Layer_SurFace"), TEXT("Prototype_GameObject_SurFace"));
				CSurFace* pNew = dynamic_cast<CSurFace*>(m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_SINGLE), TEXT("Layer_SurFace"), -1));
				pNew->SetPosition(iter.m_vIndex[0] + iX, iter.m_vIndex[1] + iY, iter.m_vIndex[2] + iZ, eNow, m_pTextureCom);
				pNew->SetTextureIndex(iter.m_iTextureNum[_int(eNow)]);
				m_mapSurFace.insert({ strKey ,pNew });
				Safe_AddRef(pNew);*/ 
			}

		}

		if (iter.m_eType == LANDCOMMAND::LCOMMAND_CHANGE_TEXTURE)
		{
			for (_int i = 0; i < 6; ++i)
			{
				if (iter.m_iTextureNum[i] == -1)
					continue;
				_wstring strKey = MakeKey(iter.m_vIndex[0], iter.m_vIndex[1], iter.m_vIndex[2], LCUBEDIRECION(i));

				if (m_mapInstancing_SurFace.find(strKey) != m_mapInstancing_SurFace.end())
					m_mapInstancing_SurFace.find(strKey)->second->m_iTextureNum = iter.m_iTextureNum[i];



				/*if (m_mapSurFace.find(strKey) != m_mapSurFace.end())
					m_mapSurFace.find(strKey)->second->SetTextureIndex(iter.m_iTextureNum[i]);*/
			}
		}
	}

	m_CommandStack.push_front(m_CommandBuffer);
	m_CommandBuffer.clear();
}

void CTerrainManager::ChangeLandInfo_Backward()
{
}

_float3 CTerrainManager::CheckPicking(_int iMode, _int iCX, _int iCY, _int iCZ, _bool bTop, CONTAINER eType, INTERACTION eInter, _int iRotate, _int iIndex)
{
	_vector vCamera = GET_INSTANCE->GetCameraPosition();
	_double fDistance = -1.f;
	_wstring strKey = {};
	_float3 fResult{ -1,-1,-1 };

	for (auto& pair : m_mapInstancing_SurFace)
	{
		_float3 fTempResult = IsPicking_Instancing(pair.second);
		if (fTempResult.x == -1)
			continue;

		_vector fTemp = vCamera - XMLoadFloat3(&fTempResult);
		_double dTempDistance = sqrt((pow(fTemp.m128_f32[0], 2) + pow(fTemp.m128_f32[1], 2) + pow(fTemp.m128_f32[2], 2)));

		if ((fDistance == -1.f) || (fDistance > dTempDistance))
		{
			fDistance = dTempDistance;
			strKey = pair.first;
			fResult = fTempResult;
		}
	}


	if (fDistance == -1)
		return fResult;

		if ((bTop == false) || (strKey[strKey.size() - 1] == _int(LCUBEDIRECION::LDIREC_TOP) + '0'))
		{
			
				if (iMode == 0)
				{
					if (m_fInterval_Now > 0.f)
						return fResult;

					_int iIndex[3] = { 0,0,0 };
					LCUBEDIRECION eDirec = LCUBEDIRECION::LDIREC_TOP;
					InterpretKey(strKey, &iIndex[0], &iIndex[1], &iIndex[2], &eDirec);

					if ((m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON)) || (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON, true)))
					{
						if (eDirec == LCUBEDIRECION::LDIREC_TOP)
						{
							iIndex[1] += 1;

							if (iIndex[0] > 2)
								iIndex[0] -= iCX / 2;

							if (iIndex[2] > 2)
								iIndex[2] -= iCZ / 2;
						}
						if (eDirec == LCUBEDIRECION::LDIREC_BOTTOM)
						{
							iIndex[1] -= iCY;

							if (iIndex[0] > 2)
								iIndex[0] -= iCX / 2;

							if (iIndex[2] > 2)
								iIndex[2] -= iCZ / 2;
						}
						if (eDirec == LCUBEDIRECION::LDIREC_NORTH)
						{
							iIndex[2] += 1;

							if (iIndex[0] > 2)
								iIndex[0] -= iCX / 2;

							if (iIndex[1] > 2)
								iIndex[1] -= iCY / 2;
						}
						if (eDirec == LCUBEDIRECION::LDIREC_SOUTH)
						{
							iIndex[2] -= iCZ;

							if (iIndex[0] > 2)
								iIndex[0] -= iCX / 2;

							if (iIndex[1] > 2)
								iIndex[1] -= iCY / 2;
						}
						if (eDirec == LCUBEDIRECION::LDIREC_WEST)
						{
							iIndex[0] -= iCX;

							if (iIndex[1] > 2)
								iIndex[1] -= iCY / 2;

							if (iIndex[2] > 2)
								iIndex[2] -= iCZ / 2;
						}
						if (eDirec == LCUBEDIRECION::LDIREC_EAST)
						{
							iIndex[0] += 1;

							if (iIndex[1] > 2)
								iIndex[1] -= iCY / 2;

							if (iIndex[2] > 2)
								iIndex[2] -= iCZ / 2;
						}

						Adjust_Index(&iIndex[0], &iIndex[1], &iIndex[2]);



						Make_NewCube(iIndex[0], iIndex[1], iIndex[2], iCX, iCY, iCZ, m_iTextureIndex);
					}
						
					else
						Make_DeleteCube(iIndex[0], iIndex[1], iIndex[2], iCX, iCY, iCZ);

					m_fInterval_Now = m_fInterval;
				}
				if (iMode == 1)
				{
					if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_RBUTTON))
					{
						Delete_LastObject();
					}
					else
					{
						if (eType == CONTAINER::CONTAINER_PLAYER)
						{
							if (GET_INSTANCE->Get_Player_Pointer() == nullptr)
							{
								GET_INSTANCE->ShowInformMessage(TEXT("플레이어 캐릭터 신규 생성"));
								GET_INSTANCE->Make_Container_Player(fResult, iRotate);
							}
							else
							{
								GET_INSTANCE->ShowInformMessage(TEXT("플레이어 캐릭터 위치 변경"));

								CGameObject* pTemp = m_pGameInstance->Get_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), -1);
								m_pGameInstance->Delete_CloneObject_ByLayer(_int(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), pTemp);
								m_vecObjInfo.pop_back();

								GET_INSTANCE->Make_Container_Player(fResult, iRotate);

							}
							


						}
						else if (eType == CONTAINER::CONTAINER_ENEMY)
							GET_INSTANCE->Make_Container_Enemy(fResult, ENEMY_TYPE::ENEMY_TYPE_END, iRotate);
						else if (eType == CONTAINER::CONTAINER_BOSS)
							GET_INSTANCE->Make_Container_Boss(fResult, ENEMY_TYPE::ENEMY_TYPE_END, iRotate);
						else if (eType == CONTAINER::CONTAINER_END)
						{
							if (eInter != INTERACTION::INTER_END)
								GET_INSTANCE->Add_InterActionObject_BySpec(eInter, nullptr, fResult, { 0.f,0.f,0.f }, iRotate);
						}

						if (eType != CONTAINER::CONTAINER_END)
						{
							MOBJ tNew(eType, fResult, _float(iRotate), iIndex);
							m_vecObjInfo.push_back(tNew);
						}
						else
						{
							MOBJ tNew(eInter, fResult, _float(iRotate), iIndex);
							m_vecObjInfo.push_back(tNew);
						}
					}
				}
		}
	
		return fResult;
}

_float3 CTerrainManager::CheckPicking()
{
	_vector vCamera = GET_INSTANCE->GetCameraPosition();
	_double fDistance = -1.f;
	_wstring strKey = {};
	_float3 fResult{ -1,-1,-1 };

	for (auto& pair : m_mapInstancing_SurFace)
	{
		_float3 fTempResult = IsPicking_Instancing(pair.second);
		if (fTempResult.x == -1)
			continue;

		_vector fTemp = vCamera - XMLoadFloat3(&fTempResult);
		_double dTempDistance = sqrt((pow(fTemp.m128_f32[0], 2) + pow(fTemp.m128_f32[1], 2) + pow(fTemp.m128_f32[2], 2)));

		if ((fDistance == -1.f) || (fDistance > dTempDistance))
		{
			fDistance = dTempDistance;
			strKey = pair.first;
			fResult = fTempResult;
		}
	}

	return fResult;
}

CTerrainManager* CTerrainManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrainManager* pInstance = new CTerrainManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrainManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CTerrainManager::Clone(void* pArg)
{
	CTerrainManager* pInstance = new CTerrainManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrainManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainManager::Free()
{
	__super::Free();

	for (auto& pair : m_mapSurFace)
		Safe_Release(pair.second);

	for (auto& pair : m_mapInstancing_SurFace)
		Safe_Delete(pair.second);

	for (auto& iter : m_CommandStack)
		iter.clear();

	m_mapSurFace.clear();
	m_CommandStack.clear();
	m_CommandBuffer.clear();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom_Gray);
	Safe_Release(m_pColliderCom);
	//Safe_Release(m_pSky);

	for (auto& iterX : m_vecLcubeInfo)
	{
		for (auto& iterY : iterX)
			iterY.clear();
		iterX.clear();
	}

	m_vecLcubeInfo.clear();
	m_vecObjInfo.clear();
}
