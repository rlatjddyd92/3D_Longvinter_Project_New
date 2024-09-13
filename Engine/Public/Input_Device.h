#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device : public CBase
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID, _bool IsPressing = false)		
	{ 
		if (IsPressing)
			return m_byKeyState[byKeyID];
		else if (!m_byKeyState_Before[byKeyID])
			return m_byKeyState[byKeyID];
		else
			return _byte(0);
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse, _bool IsPressing = false) 	
	{	
		if (IsPressing)
			return m_tMouseState.rgbButtons[eMouse];
		else if (!m_tMouseState_Before.rgbButtons[eMouse])
			return m_tMouseState.rgbButtons[eMouse];
		else
			return _byte(0);
	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{			
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}
	
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };


	

private:	
	_byte					m_byKeyState[256] = {};			// 키보드에 있는 모든 키값을 저장하기 위한 변수
	_byte					m_byKeyState_Before[256] = {};			// 키보드에 있는 모든 키값을 저장하기 위한 변수
	
	DIMOUSESTATE			m_tMouseState = {};
	DIMOUSESTATE			m_tMouseState_Before = { };

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END

