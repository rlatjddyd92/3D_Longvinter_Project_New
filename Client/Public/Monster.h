#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CMonster final : public CGameObject
{
private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CShader*				m_pShaderCom = { nullptr };	
	class CModel*				m_pModelCom = { nullptr };

public:
	void SetTransForm(CTransform::STATE eState, XMVECTOR vecNew);
	_float3 GetTransForm(CTransform::STATE eState) { 
		XMVECTOR vTemp = m_pTransformCom->Get_State(eState); 
		return { vTemp.m128_f32[0], vTemp.m128_f32[1] , vTemp.m128_f32[2] };
	}

	void Turn_Monster(_fvector vAxis, _float fTimeDelta)
	{
		m_pTransformCom->Turn(vAxis, fTimeDelta);
	}

	void Scaling_Monster(_float fScale)
	{
		m_fScale_Sum *= fScale;
		m_pTransformCom->Set_Scaled(m_fScale_Sum, m_fScale_Sum, m_fScale_Sum);
	}

	void Change_Animation(_int iInput);


private:
	HRESULT Ready_Components();

	_float m_fRadian_Sum[3] = { 0.f,0.f,0.f };
	_float m_fScale_Sum = 1.f;


public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END