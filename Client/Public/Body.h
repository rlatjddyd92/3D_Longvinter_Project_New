#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Model.h"

BEGIN(Engine)
class CShader;

END

BEGIN(Client)

class CBody : public CPartObject
{
public:
	typedef struct : public CPartObject::PARTOBJ_DESC
	{
		const _uint* pParentState = { nullptr };
	}BODY_DESC;

protected:
	CBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody(const CBody& Prototype);
	virtual ~CBody() = default;

public:
	const _float4x4* Get_BoneMatrix_Ptr(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetAnimSpeed(_float fSpeed) { m_pModelCom->SetAnimSpeed(fSpeed); } // <- 애니메이션 속도 조절 (1은 정상속도 0에 가까울 수록 느려지며 0이되면 재생 안됨, 아마 음수 넣으면 반대로 움직일 듯) 
	_float GetAnimSpeed() { return m_pModelCom->GetAnimSpeed(); }

	void SetModelTexture(_int iMeterial, _int iType, CTexture* pTexture)
	{
		if (m_pModelCom != nullptr)
			m_pModelCom->SetTexture(iMeterial, iType, pTexture);
	}

protected:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };

protected:
	const _uint* m_pParentState = { nullptr };
	_uint		 m_iBeforeState = 0;

	_float		 m_fAnimTime_Deposit = 0.f;

protected:
	HRESULT Ready_Components();

public:
	static CBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END