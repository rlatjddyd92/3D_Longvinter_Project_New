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
	for (auto& iter : m_mapInstancing_SurFace)
	{
		_vector vCamera = GET_INSTANCE->GetCameraPosition();

		_float3 fLook{};

		fLook.x = vCamera.m128_f32[0] - iter.second->m_vMat.m[3][0];
		fLook.y = vCamera.m128_f32[1] - iter.second->m_vMat.m[3][1];
		fLook.z = vCamera.m128_f32[2] - iter.second->m_vMat.m[3][2];

		_bool bGray = false;

		if (m_fRender_Length >= 0.f)
		{
			_float Length = sqrt(pow(fLook.x, 2) + pow(fLook.y, 2) + pow(fLook.z, 2));
			if (Length > m_fRender_Length)
				continue;
			else if (Length > m_fRender_Length * 0.5f)
				bGray = true;
		}


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


		CShader* pShader = nullptr;

		if (bGray)
			pShader = m_pShaderCom;
		else
			pShader = m_pShaderCom;

		if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &iter.second->m_vMat)))
			return E_FAIL;

		if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
			return E_FAIL;
		if (FAILED(pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
			return E_FAIL;

		if (FAILED(m_pTextureCom->Bind_ShadeResource(pShader, "g_DiffuseTexture", iter.second->m_iTextureNum)))
			return E_FAIL;

		if (FAILED(pShader->Begin(0)))
			return E_FAIL;


		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

	}

	return S_OK;
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
		WriteFile(hFile, &iter.fPosition, sizeof(_float3), &dwByte, nullptr);
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
		MAKEOBJ tTemp(CONTAINER::CONTAINER_END, {0.f,0.f,0.f});
		ReadFile(hFile, &tTemp.eCon_Type, sizeof(CONTAINER), &dwByte, nullptr);
		ReadFile(hFile, &tTemp.fPosition, sizeof(_float3), &dwByte, nullptr);
		m_vecObjInfo.push_back(tTemp);

		if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_PLAYER)
		{
			GET_INSTANCE->Make_Container_Player(tTemp.fPosition);
		}
		if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_ANIMAL)
		{

		}
		if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_ENEMY)
		{
			GET_INSTANCE->Make_Container_Enemy(tTemp.fPosition, ENEMY_TYPE::ENEMY_TYPE_END);
		}
		if (m_vecObjInfo.back().eCon_Type == CONTAINER::CONTAINER_NPC)
		{

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
					for (_int z = 0; z < 6; ++z)
						tTemp.m_iTextureNum[z] = TextureIndex;
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
	_uint iMinX = max(0, (fPosition.x - fRadius) / LCUBESIZE);
	_uint iMaxX = min(LMAX_X, (fPosition.x + fRadius) / LCUBESIZE);
	_uint iMinY = max(m_iBedRock, (fPosition.y - fRadius) / LCUBESIZE);
	_uint iMaxY = min(LMAX_Y, (fPosition.y + fRadius) / LCUBESIZE);
	_uint iMinZ = max(0, (fPosition.z - fRadius) / LCUBESIZE);
	_uint iMaxZ = min(LMAX_Z, (fPosition.z + fRadius) / LCUBESIZE);

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

_float3 CTerrainManager::Check_Terrain_Collision_Adjust(_float3 fCenter, _float3 fExtents, _float3 vAdjustVector, LCUBEDIRECION* eDirec)
{
	_float3 vCenter = fCenter;
	_float3 vAdjustPosition = { 0.f,0.f,0.f };
	_float vAdjustScale = 0.f;
	LCUBEDIRECION eCollisionDirec = LCUBEDIRECION::LDIREC_END;
	_float3 vExtents = fExtents;

	// 1. 객체가 존재할 수 있는 큐브 인덱스 범위 잡기 
	_uint iMinX = (vCenter.x - vExtents.x) / LCUBESIZE;
	_uint iMaxX = (vCenter.x + vExtents.x) / LCUBESIZE;
	_uint iMinY = (vCenter.y - vExtents.y) / LCUBESIZE;
	_uint iMaxY = (vCenter.y + vExtents.y) / LCUBESIZE;
	_uint iMinZ = (vCenter.z - vExtents.z) / LCUBESIZE;
	_uint iMaxZ = (vCenter.z + vExtents.z) / LCUBESIZE;

	if (iMinX * iMinY * iMaxZ < 0)
		return TERRAINERROR;

	if (iMaxX >= LMAX_X)
		return TERRAINERROR;

	if (iMaxY >= LMAX_Y + 5)
		return TERRAINERROR;

	if (iMaxZ >= LMAX_Z)
		return TERRAINERROR;

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

_float3 CTerrainManager::CheckPicking(_int iMode, _int iCX, _int iCY, _int iCZ, _bool bTop, CONTAINER eType)
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

	/*for (auto& pair : m_mapSurFace)
	{
		_float3 fTempResult = pair.second->IsPicking();
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
			
	}*/

	if (fDistance == -1)
		return fResult;

		if ((bTop == false) || (strKey[strKey.size() - 1] == _int(LCUBEDIRECION::LDIREC_TOP) + '0'))
		{
			
				if (iMode == 0)
				{
					_int iIndex[3] = { 0, };
					LCUBEDIRECION eDirec = LCUBEDIRECION::LDIREC_TOP;
					InterpretKey(strKey, &iIndex[0], &iIndex[1], &iIndex[2], &eDirec);

					if (m_pGameInstance->Get_DIMouseState(MOUSEKEYSTATE::DIMK_LBUTTON))
						Make_NewCube(iIndex[0], iIndex[1], iIndex[2], iCX, iCY, iCZ, m_iTextureIndex);
					else
						Make_DeleteCube(iIndex[0], iIndex[1], iIndex[2], iCX, iCY, iCZ);

					
				}
				if (iMode == 1)
				{
					if (eType == CONTAINER::CONTAINER_PLAYER)
						GET_INSTANCE->Make_Container_Player(fResult);

					if (eType == CONTAINER::CONTAINER_ENEMY)
						GET_INSTANCE->Make_Container_Enemy(fResult, ENEMY_TYPE::ENEMY_TYPE_END);
					
					m_vecObjInfo.push_back({ eType,fResult });
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
	

	for (auto& iterX : m_vecLcubeInfo)
	{
		for (auto& iterY : iterX)
			iterY.clear();
		iterX.clear();
	}

	m_vecLcubeInfo.clear();
	m_vecObjInfo.clear();
}
