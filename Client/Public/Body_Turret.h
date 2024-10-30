#pragma once

#include "Client_Defines.h"
#include "Body.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Turret final : public CBody
{

private:
	CBody_Turret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Turret(const CBody_Turret& Prototype);
	virtual ~CBody_Turret() = default;

public:
	const _float4x4* Get_BoneMatrix_Ptr(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	class CModel* m_pModelCom_Body = { nullptr };

	_float4x4 m_fParentMatrix_Origin{};

public:
	static CBody_Turret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END