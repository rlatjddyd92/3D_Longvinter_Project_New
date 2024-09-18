#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"

BEGIN(Engine)

END

BEGIN(Client)

class CItemManager : public CBase
{
private:
	CItemManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual ~CItemManager() = default;




private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CItemManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual void Free() override;
};

END