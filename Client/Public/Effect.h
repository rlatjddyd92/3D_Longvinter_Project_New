#pragma once

#include "Client_Defines.h"

#include "GameObject.h"
#include "Collider.h"

BEGIN(Engine)

END

BEGIN(Client)
class CLongvinter_Container;

class CEffect : public CGameObject
{
public:
	typedef struct EFFECTINFO
	{
		_float3 fPosition = { 0.f,0.f,0.f };
		_float3 fDirec = { 0.f,0.f,0.f };
		_float4 fColor = { 0.f,0.f,0.f,0.f };
		_float fFly = 0.f;
		_float fPower = 0.f;
		_float fPowerDecrease = 0.f;
		_float fTime = 0.f;
		_bool bDead = false;
	}E_INFO;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& Prototype);
	virtual ~CEffect() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	void AddEffectBySpec();

protected:


private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();



protected:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	list<E_INFO*> m_EffectInfolist;


	_bool m_bSpec_TerrainDelete = true;
	E_INFO m_tSpec{};


public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;



};

END