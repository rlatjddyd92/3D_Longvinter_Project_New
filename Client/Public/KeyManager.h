#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CKeyManager : public CBase
{
private:
	CKeyManager();
	virtual ~CKeyManager() = default;

public:
	_bool			Key_Pressing(_int _iKey);
	_bool			Key_Down(_int _iKey);
	_bool			Key_Up(_int _iKey);	// 눌렀다 뗐을 때, true가 되는 함수
	void			Key_Update();

public:
	static CKeyManager* Create();
	virtual void Free() override;

private:
	_bool				m_bKeyState[VK_MAX];
	queue<_int>			m_queuePushedKey;
};

END