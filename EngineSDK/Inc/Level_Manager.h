#pragma once

#include "Base.h"

/* 1. ���� �Ҵ��� ������ �ּҸ� ��� �ִ´�. */
/* 2. ���� �Ҵ��� ������ �����Ѵ�. */
/* 3. ���ο� ������ ��ü �� ���� ������ �ı����ֳ�. + ���� ������ �ڿ�(���ҽ���, ��ü��) �� �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNextLevel);

private:
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	_uint					m_iLevelIndex = {};

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END