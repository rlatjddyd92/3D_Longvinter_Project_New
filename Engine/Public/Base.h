#pragma once

#include "Engine_Defines.h"

/* 1. ��� Ŭ������ �θ� �Ǵ� Ŭ�����Դϴ�. (Super Class) */
/* 2. ���۷��� ī��Ʈ�� �����Ѵ�. */
/* 2-1. ���۷��� ī��Ʈ�� ������Ų��. AddRef() */
/* 2-2. ���۷��� ī��Ʈ�� ���ҽ�Ų��. or �����Ѵ�. Release() */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	unsigned int AddRef();
	unsigned int Release();

private:
	unsigned int				m_iRefCnt = { 0 };

public:
	/* �������õ� �Լ��� �ʿ����. */

	/* �������õ� �Լ�.*/
	virtual void Free();

};

END


