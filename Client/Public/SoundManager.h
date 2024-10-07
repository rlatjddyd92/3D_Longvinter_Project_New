#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Tool.h"

// [사운드 매니저] 
// 1. 모든 사운드 리소스 관리
// 2. 소리가 나는 위치와 거리에 따른 볼륨을 계산하여 알려 준다 
// 3. 플레이어 위치에 맞춰 실제 사운드를 재생한다 

BEGIN(Engine)

END

BEGIN(Client)

class CSoundManager : public CGameObject
{
public:
	typedef struct MAKING_SOUND
	{
		SOUND_TYPE eType = SOUND_TYPE::SOUND_END;
		_float3 fPosition = { 0.f,0.f,0.f };
		_float fVolume = 0.f;
	};


protected:
	CSoundManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSoundManager(const CSoundManager& Prototype);
	virtual ~CSoundManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;



public:
	static CSoundManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END