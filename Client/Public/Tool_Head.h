#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Tool.h"

// [Tool_Head]
// Container의 Part 소켓을 사용하지 않을 때 빈 부분을 채우는 용도 

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CTool_Head : public CTool
{
public:
	enum HEAD_TYPE
	{
		TYPE_NORMAL_1,
		TYPE_NORMAL_2,
		TYPE_NORMAL_3,
		TYPE_BROWN,
		TYPE_YELLOW,
		TYPE_PINK,
		TYPE_BLUE,
		TYPE_HEADGEAR,
		TYPE_END
	};



public:
	typedef struct : public CTool::TOOL_DESC
	{
		HEAD_TYPE eType = HEAD_TYPE::TYPE_END;
	}HEAD_DESC;

	const _float4x4* Get_BoneMatrix_Ptr(const _char* pBoneName) const;

private:
	CTool_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTool_Head(const CTool_Head& Prototype);
	virtual ~CTool_Head() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

	_float m_fTest_X = 0.f;
	_float m_fTest_Y = 0.f;
	_float m_fTest_Z = 0.f;

	HEAD_TYPE m_eType = HEAD_TYPE::TYPE_END;

public:
	static CTool_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END