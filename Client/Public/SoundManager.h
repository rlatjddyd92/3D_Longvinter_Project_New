#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include <io.h>

#include "../Default/fmod.h"
#include "../Default/fmod.hpp"

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
	typedef struct PLAYLIST
	{
		SOUND_NAME eName = SOUND_NAME::SOUND_END;
		_float fVolume = 0.f;
		_float fVolume_Origin = 0.f;
		_float fVolume_Position = 0.f;
		_bool bPlayAuto = false;
		_bool bStart = false;
		_float3 fPosition = { -1.f,-1.f,-1.f };
	}PLIST;


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
	void Initialize_SoundFile();

public:
	void PlaySound(SOUND_NAME eSound, SOUND_CHANNEL eChannel, _float fVolume, _float3 fPosition = _float3(-1.f,-1.f,-1.f));
	void PlayBGM(TCHAR* pSoundKey, _float fVolume);
	void StopSound(SOUND_CHANNEL eID);
	void StopAll();
	void SetChannelVolume(SOUND_NAME eSound, SOUND_CHANNEL eChannel, _float fVolume, _float3 fPosition = _float3(-1.f, -1.f, -1.f));

	bool IsSoundPlaying(SOUND_CHANNEL eID);

	_float3 GetSound(SOUND_NAME* eSound, _float3 fPosition);

private:
	FMOD::Sound* LoadSound(const _char* pPath);



private:
	_int m_iPlayerChannel = 2;
	_int m_iMonsterChannel = 5;
	_int m_iIngameChannel = 5;
	_int m_iSystemChannel = 2;

private:
	vector<PLIST> m_vecPlay; 
	vector<FMOD::Sound*> m_vecSound; // 사운드 리소스 정보를 갖는 객체 
	vector<FMOD::Channel*> m_vecChannel;// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD::System* m_pSystem;						// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	unsigned int				version;



public:
	static CSoundManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END