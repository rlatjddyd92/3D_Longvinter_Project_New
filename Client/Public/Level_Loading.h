#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. �ε�ȭ���� �����ϰ� �����Ͽ� �������Ѵ�. */
/* 2. �����带 �����Ͽ� ���������� �ڿ��� �ε��Ѵ�.(CLoader�� ��ü�� �����Ѵ�.) */
/* 2-1. ���� ����� �����ϴ� �δ� ��ü�� �����ϰ� �����Ѵ�. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVELID eNextLevelID);
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVELID						m_eNextLevelID = { LEVELID::LEVEL_END };
	class CLoader*				m_pLoader = { nullptr };

private:
	HRESULT Ready_Layer_BackGround();

public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END
