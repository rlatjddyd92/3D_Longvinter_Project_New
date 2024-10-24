#include "stdafx.h"
#include "SoundManager.h"
#include "ClientInstance.h"

CSoundManager::CSoundManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CSoundManager::CSoundManager(const CSoundManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSoundManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSoundManager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	FMOD::System_Create(&m_pSystem);
	m_pSystem->getVersion(&version);
	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	m_vecChannel.resize(_int(SOUND_CHANNEL::CH_END));
	m_vecPlay.resize(_int(SOUND_CHANNEL::CH_END));
	m_vecPlay[_int(SOUND_CHANNEL::CH_INGAME_EXPLOSION)].bPlayAuto = true;
	Initialize_SoundFile();


	return S_OK;
}

void CSoundManager::Priority_Update(_float fTimeDelta)
{
	for (_int i = 0; i < _int(SOUND_CHANNEL::CH_SYSTEM_BGM); ++i)
	{
		_bool bPlay = false;
		m_vecPlay[i].fVolume_Origin = m_vecPlay[i].fVolume;

		if (!m_vecPlay[i].bPlayAuto)
			m_vecPlay[i].fVolume = 0.f;
		
		m_vecPlay[i].bStart = false;
		m_vecChannel[i]->isPlaying(&bPlay);
		if (!bPlay)
		{

			m_vecPlay[i].eName = SOUND_NAME::SOUND_END;
			m_vecPlay[i].fVolume_Origin = 0.f;
			m_vecPlay[i].fPosition = { -1.f,-1.f,-1.f };
		
		}
			
	}

	_bool bPlay = false;
	m_vecChannel[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)]->isPlaying(&bPlay);

	if (!bPlay)
	{
		FMOD_RESULT result = m_pSystem->playSound(m_vecSound[_int(m_vecPlay[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)].eName)], nullptr, false, &m_vecChannel[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)]);
		if (result != FMOD_OK)
			return;

		FMOD::Channel* channel = m_vecChannel[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)];
		channel->setVolume(m_vecPlay[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)].fVolume / 1.f);
		m_pSystem->update();
	}
	
}

void CSoundManager::Update(_float fTimeDelta)
{
	
}

void CSoundManager::Late_Update(_float fTimeDelta)
{
	for (_int i = 0; i < _int(SOUND_CHANNEL::CH_SYSTEM_BGM); ++i)
	{
		if (m_vecPlay[i].bStart)
		{
			m_vecChannel[i]->stop();
			FMOD_RESULT result = m_pSystem->playSound(m_vecSound[_int(m_vecPlay[i].eName)], nullptr, false, &m_vecChannel[i]);
			if (result != FMOD_OK)
				return;

			FMOD::Channel* channel = m_vecChannel[i];
			channel->setVolume(m_vecPlay[i].fVolume / 1.f);
			m_pSystem->update();
		}
		else
		{
			FMOD::Channel* channel = m_vecChannel[i];
			channel->setVolume(m_vecPlay[i].fVolume / 1.f);
			m_vecPlay[i].fVolume_Origin = m_vecPlay[i].fVolume;
			m_pSystem->update();
		}
	}



}

HRESULT CSoundManager::Render()
{
	
	return S_OK;
}

void CSoundManager::Initialize_SoundFile()
{
	
	m_vecSound.resize(_int(SOUND_NAME::SOUND_END));
	m_vecSound[_int(SOUND_NAME::SOUND_EXPLOSION_HUGE)] = LoadSound("../Bin/Resources/Sound/WAV_LandmineExplosion_01.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_FIRE)] = LoadSound("../Bin/Resources/Sound/WAV_Fire.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_SHOT_MACHINEGUN)] = LoadSound("../Bin/Resources/Sound/WAV_rifle_shot_03_v3.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_SHOT_SHOTGUN)] = LoadSound("../Bin/Resources/Sound/Wav_AutomaticShotgun_Shot_01.ogg");

	m_vecSound[_int(SOUND_NAME::SOUND_JUMP)] = LoadSound("../Bin/Resources/Sound/WAV_FootstepStone_03.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_THROW_GRANADE)] = LoadSound("../Bin/Resources/Sound/WAV_Coin.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_WALK)] = LoadSound("../Bin/Resources/Sound/WAV_FootstepGrass_01.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_BGM_BATTLE)] = LoadSound("../Bin/Resources/Sound/WAV_MusicCombat.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_BGM_NORMAL)] = LoadSound("../Bin/Resources/Sound/WAV_Forest01.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_HACK)] = LoadSound("../Bin/Resources/Sound/Wav_Automation_02.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_PAGE_OPEN)] = LoadSound("../Bin/Resources/Sound/WAV_InventoryOpen_01.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_WARNING)] = LoadSound("../Bin/Resources/Sound/Clang.wav");
	m_vecSound[_int(SOUND_NAME::SOUND_WEAPONSLOT)] = LoadSound("../Bin/Resources/Sound/WAV_DrawWeapon.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_BEEP)] = LoadSound("../Bin/Resources/Sound/WAV_TurretBeep.ogg");
	m_vecSound[_int(SOUND_NAME::SOUND_MELEE)] = LoadSound("../Bin/Resources/Sound/Dagger.wav");
	

	m_vecPlay[_int(SOUND_CHANNEL::CH_INGAME_EXPLOSION)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_MONSTER_GUN)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_MONSTER_MELEE)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_MONSTER_SHOTGUN)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_PLAYER_ACT)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_PLAYER_WEAPON)].bPlayAuto = true;
	m_vecPlay[_int(SOUND_CHANNEL::CH_MONSTER_ACT)].bPlayAuto = true;
}

void CSoundManager::PlaySound(SOUND_NAME eSound, SOUND_CHANNEL eChannel, _float fVolume, _float3 fPosition)
{
	m_vecPlay[_int(eChannel)].fVolume_Position = fVolume;

	if (fPosition.x != -1.f)
	{
		_vector vDistance = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION) - XMLoadFloat3(&fPosition);
		_float fDistance = sqrt(pow(vDistance.m128_f32[0], 2) + pow(vDistance.m128_f32[1], 2) + pow(vDistance.m128_f32[2], 2));
		fVolume -= fDistance * 0.5f;
	}

	if (fVolume <= 0.f)
	{
		m_vecPlay[_int(eChannel)].fVolume_Position = 0.f;
		return;
	}
		
	
	m_vecPlay[_int(eChannel)].bStart = true;
	m_vecPlay[_int(eChannel)].eName = eSound;
	m_vecPlay[_int(eChannel)].fVolume = fVolume;
	
	m_vecPlay[_int(eChannel)].fVolume_Origin = fVolume;
	m_vecPlay[_int(eChannel)].fPosition = fPosition;

	if (eChannel == SOUND_CHANNEL::CH_SYSTEM_BGM)
		m_vecChannel[_int(SOUND_CHANNEL::CH_SYSTEM_BGM)]->stop();
}

void CSoundManager::PlayBGM(TCHAR* pSoundKey, _float fVolume)
{
}

void CSoundManager::StopSound(SOUND_CHANNEL eID)
{

}

void CSoundManager::StopAll()
{
}

void CSoundManager::SetChannelVolume(SOUND_NAME eSound, SOUND_CHANNEL eChannel, _float fVolume, _float3 fPosition)
{
	m_vecPlay[_int(eChannel)].fVolume_Position = fVolume;

	if (fPosition.x != -1.f)
	{
		_vector vDistance = GET_INSTANCE->Get_Player_Pointer()->GetTransform(CTransform::STATE_POSITION) - XMLoadFloat3(&fPosition);
		_float fDistance = sqrt(pow(vDistance.m128_f32[0], 2) + pow(vDistance.m128_f32[1], 2) + pow(vDistance.m128_f32[2], 2));
		fVolume -= fDistance * 0.5f;
	}

	if (fVolume <= 0.f)
	{
		m_vecPlay[_int(eChannel)].fVolume_Position = 0.f;
		return;
	}

	_bool bPlay = false;
	m_vecChannel[_int(eChannel)]->isPlaying(&bPlay);
	if (!bPlay)
	{
		m_vecPlay[_int(eChannel)].bStart = true;
		m_vecPlay[_int(eChannel)].eName = eSound;
		m_vecPlay[_int(eChannel)].fVolume = fVolume;
		m_vecPlay[_int(eChannel)].fPosition = fPosition;
		m_vecPlay[_int(eChannel)].fVolume_Origin = fVolume;
	}
	else if (m_vecPlay[_int(eChannel)].fVolume < fVolume)
	{
		m_vecPlay[_int(eChannel)].fVolume = fVolume;
		m_vecPlay[_int(eChannel)].fPosition = fPosition;
	}

	
}


bool CSoundManager::IsSoundPlaying(SOUND_CHANNEL eID)
{
	return false;
}

_float3 CSoundManager::GetSound(SOUND_NAME* eSound, _float3 fPosition)
{
	_float fVolume = 0.f;
	_float3 fDestination = { -1.f,-1.f,-1.f };
	*eSound = SOUND_NAME::SOUND_END;

	for (auto& iter : m_vecPlay)
	{
		if (iter.fPosition.x >= 0.f)
		{
			_vector vDirec = XMLoadFloat3(&iter.fPosition) - XMLoadFloat3(&fPosition);
			_float fDistance = sqrt(pow(vDirec.m128_f32[0], 2) + pow(vDirec.m128_f32[1], 2) + pow(vDirec.m128_f32[2], 2));

			if (iter.fVolume_Position - fDistance * 0.5 > fVolume)
			{
				fVolume = iter.fVolume_Position - fDistance * 0.5;
				*eSound = iter.eName;
				fDestination = iter.fPosition;
			}
		}
	}

	return fDestination;
}

FMOD::Sound* CSoundManager::LoadSound(const _char* pPath)
{
	FMOD::Sound* pSound = nullptr;
	FMOD_RESULT eRes = m_pSystem->createSound(pPath, FMOD_DEFAULT, 0, &pSound);
	return pSound;
}


CSoundManager* CSoundManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSoundManager* pInstance = new CSoundManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSoundManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSoundManager::Clone(void* pArg)
{
	CSoundManager* pInstance = new CSoundManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CSoundManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoundManager::Free()
{
	__super::Free();

	for (auto& iter : m_vecSound)
		iter->release();

	m_vecSound.clear();

	m_vecChannel.clear();

	m_pSystem->release();
	m_pSystem = nullptr;
	
	m_vecPlay.clear();
}
