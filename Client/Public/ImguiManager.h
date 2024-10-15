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
	_char* szMode[3] = { "Terrain", "Object", "Move_Model"};
	_int m_iSelectMode = 0;
	_char* szObject[5] = { "Player","NPC", "Enemy","Boss", "LandObject"};
	_int m_iSelectObject = 0;
	_char* szTexture[8] = { "0","1" ,"2" ,"3" ,"4", "5", "6", "7"};
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
	};
	_int m_iSelectLandObject = _int(INTERACTION::INTER_END);

	_int m_iAnimation = 0;

	_int m_iRenderLimit = -1;

	// Class

public:
	static CImguiManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END