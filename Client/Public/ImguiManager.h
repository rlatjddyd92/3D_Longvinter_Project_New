#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Terrain.h"
#include "KeyManager.h"
#include "TerrainManager.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)


class CImguiManager final : public CGameObject
{
private:
	CImguiManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CImguiManager(const CImguiManager& Prototype);
	virtual ~CImguiManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	

private:
	HRESULT Ready_Components();


	//test
	_float fTest = 0.f;
	_int iTest = 0;

	// 상태 제어
	_int m_iMax[3] = {0,0,0}; // <- 맵의 기반 설정
	_int m_iLand[3] = { 0,0,0 };
	_char* szMode[4] = { "Terrain", "Object", "Move_Model", "Exp" };
	_int m_iSelectMode = 0;
	_char* szObject[5] = { "Player","NPC", "Enemy","Turret", "LandObject"};
	_int m_iSelectObject = 0;
	_char* szTexture[LTEXTUREMAX]
		= {
		"LNAME_PAVEDROCK",
	"LNAME_ROCK",
	"LNAME_PAVEDROCK_LIGHT",
	"LNAME_GRASS",
	"LNAME_GRASS_FALL",
	"LNAME_DIRT_RED",
	"LNAME_DIRT_BROWN_1",
	"LNAME_DIRT_BROWN_2",
	"LNAME_DESERT_ROCK_LIGHT",
	"LNAME_DESERT_ROCK_AND_DIRT",
	"LNAME_DESERT_ROCK",
	"LNAME_DESERT_SAND_WHITE",
	"LNAME_DESERT_SAND",
	"LNAME_DESERT_SAND_LIGHT",
	"LANME_WALL_ROCK_1",
	"LANME_ROCK_LIGHT",
	"LANME_ROCK_WHITE",
	"LNAME_GRASS_LIGHT",
	"LNAME_GRASS_LIGHT_FLOWER",
	"LANME_WALL_ROCK_2",
	"LNAME_GRASS_ROCK",
	"LNAME_WALL_BRICK",
	"LNAME_WALL_NORMAL",
	"LNAME_WALL_WOOD",
	"LANME_WALL_DARK",
	"LNAME_WALL_OLD"
	};
	_int m_iSelectTexture = 0;


	_char* szLandObject[_int(INTERACTION::INTER_END)]
		= {
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"없음",
"ITEM",
"Tree",
"AppleTree",
"Rock",
"Bush",
"Door",
"MonsterMaker"
	};
	_int m_iSelectLandObject = _int(INTERACTION::INTER_END);

	_int m_iAnimation = 0;

	_int m_iRenderLimit = -1;

	_int m_iRotate = 0;

	_char* szNPC[_int(NPC_TYPE::NPC_END)];

	_int m_iSelectNPC = _int(NPC_TYPE::NPC_END);

	_int m_iIsExp = 0;

	// Class

public:
	static CImguiManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END