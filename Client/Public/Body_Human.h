#pragma once

#include "Client_Defines.h"
#include "Body.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Human final : public CBody
{

private:
	CBody_Human(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Human(const CBody_Human& Prototype);
	virtual ~CBody_Human() = default;

public:
	const _float4x4* Get_BoneMatrix_Ptr(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_Human_Body(HUMAN_BODY eBody) { m_pModelCom->SetTexture(0, 1, m_vecTexture_HumanBody[_int(eBody)]); }
	void Set_Human_Face(HUMAN_FACE eBody) { m_pModelCom->SetTexture(1, 1, m_vecTexture_HumanFace[_int(eBody)]); }

private:
	HRESULT Ready_Components();

	vector<CTexture*> m_vecTexture_HumanBody;
	vector<CTexture*> m_vecTexture_HumanFace;

public:
	static CBody_Human* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END