#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Tool.h"

// [���� �Ŵ���] 
// 1. ��� ���� ���ҽ� ����
// 2. �Ҹ��� ���� ��ġ�� �Ÿ��� ���� ������ ����Ͽ� �˷� �ش� 
// 3. �÷��̾� ��ġ�� ���� ���� ���带 ����Ѵ� 

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