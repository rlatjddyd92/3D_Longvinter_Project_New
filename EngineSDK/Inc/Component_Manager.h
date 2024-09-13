#pragma once


#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Rect3D.h"
//#include "VIBuffer_Cube.h"
//#include "Transform.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Collider.h"


/* ������Ʈ�� ������ �������� �����Ѵ�. */
/* �����Ͽ� �����Ѵ�.*/

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);

private:
	map<const _wstring, class CComponent*>*		m_pPrototypes = { nullptr };
	_uint										m_iNumLevels = { 0 };
private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END