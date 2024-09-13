#pragma once

#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)
class CPipeLine;

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void Update(); /* ��������ǥ��(����Ʈ��)�� ���콺 ��ǥ�� ���ϰ� �� ���콺�� ��ǥ�� ���� �����̽�(���� �����̽�)���� ���� �� ���� ��ŭ ������. */
	void Transform_ToLocalSpace(const _float4x4& WorldMatrix);
	_bool isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	HWND						m_hWnd = {};
	_uint						m_iWinSizeX = {};
	_uint						m_iWinSizeY = {};

private:
	_float3						m_vRayPos_World = {};
	_float3						m_vRayDir_World = {};

private:
	_float3						m_vRayPos_InLocalSpace = {};
	_float3						m_vRayDir_InLocalSpace = {};

	CGameInstance*				m_pGameInstance = { nullptr };

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, CGameInstance* pGameInstance);
	virtual void Free();
};

END