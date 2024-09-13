#include "stdafx.h"

#include "KeyManager.h"

#include "GameInstance.h"

CKeyManager::CKeyManager()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

_bool CKeyManager::Key_Pressing(_int _iKey)
{
	if (GetKeyState(_iKey) & 0x8000)
	{
		m_queuePushedKey.push(_iKey);
		return true;
	}
		

	return false;
}

_bool CKeyManager::Key_Down(_int _iKey)
{
	// 이전에 눌린 적이 없고, 현재 눌린 상태
	if ((!m_bKeyState[_iKey]) && (GetKeyState(_iKey) & 0x8000))
	{
		m_queuePushedKey.push(_iKey);
		return true;
	}
	if ((m_bKeyState[_iKey]) && (GetKeyState(_iKey) & 0x8000))
	{
		m_queuePushedKey.push(_iKey);
		return false;
	}
	
	return false;
}

_bool CKeyManager::Key_Up(_int _iKey)
{
	// 이전에 눌린 적이 있고, 현재는 눌리지 않은 상태

	if ((m_bKeyState[_iKey]) && !(GetKeyState(_iKey) & 0x8000))
	{

		return true;
	}

	return false;
}

void CKeyManager::Key_Update()
{
	for (auto& iter : m_bKeyState)
		iter = false;

	while (!m_queuePushedKey.empty())
	{
		m_bKeyState[m_queuePushedKey.front()] = true;
		m_queuePushedKey.pop();
	}
}

CKeyManager* CKeyManager::Create()
{
	CKeyManager* pInstance = new CKeyManager;

	if (pInstance == nullptr)
	{
		MSG_BOX(TEXT("Failed to Created : CKeyManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKeyManager::Free()
{
	__super::Free();
}
