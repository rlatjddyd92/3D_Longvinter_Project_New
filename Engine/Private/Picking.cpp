#include "..\Public\Picking.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ pGameInstance }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	m_hWnd = hWnd;

	m_iWinSizeX = iWinSizeX;

	m_iWinSizeY = iWinSizeY;

	return S_OK;
}

void CPicking::Update()
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);

	/* 뷰포트 == 로컬 * 월드행렬 * 뷰행렬 * 투영행렬 /w -> 뷰포트 변환 */
	ScreenToClient(m_hWnd, &ptMouse);

	/* 투영스페이스 == 로컬 * 월드행렬 * 뷰행렬 * 투영행렬 /w */
	_float3		vMousePos{};
	vMousePos.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / (m_iWinSizeY * -0.5f) + 1.f;
	vMousePos.z = 0.f; /* Near평면을 클릭한거야!! */

	_float4x4			ProjMatrixInv{};

	// 투영 행렬의 역행렬 계산

	ProjMatrixInv = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE::D3DTS_PROJ);

	XMMATRIX projectionMatrix = XMLoadFloat4x4(&ProjMatrixInv);
	XMMATRIX invProjectionMatrix = XMMatrixInverse(nullptr, projectionMatrix);

	// 정규 장치 좌표를 뷰 공간으로 변환
	XMVECTOR rayOrigin = XMVectorSet(vMousePos.x, vMousePos.y, 0.0f, 1.0f);
	XMVECTOR rayDirection = XMVectorSet(vMousePos.x, vMousePos.y, 1.0f, 1.0f);

	rayOrigin = XMVector3TransformCoord(rayOrigin, invProjectionMatrix);
	rayDirection = XMVector3TransformCoord(rayDirection, invProjectionMatrix);

	// 뷰 공간에서 레이 방향 정규화
	rayDirection = XMVector3Normalize(rayDirection);

	m_vRayPos_InLocalSpace = { rayOrigin.m128_f32[0],rayOrigin.m128_f32[1], rayOrigin.m128_f32[2] };
	m_vRayDir_InLocalSpace = { rayDirection.m128_f32[0],rayDirection.m128_f32[1], rayDirection.m128_f32[2] };

	// 뷰 행렬의 역행렬 계산

	_float4x4			ViewMatrixInv{};

	ViewMatrixInv = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE::D3DTS_VIEW);

	XMMATRIX viewMatrix = XMLoadFloat4x4(&ViewMatrixInv);
	XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	// 뷰 공간에서 월드 공간으로 변환
	rayOrigin = XMVector3TransformCoord(rayOrigin, invViewMatrix);
	rayDirection = XMVector3TransformNormal(rayDirection, invViewMatrix);

	// 월드 공간에서 레이 방향 정규화
	rayDirection = XMVector3Normalize(rayDirection);

	m_vRayPos_World = { rayOrigin.m128_f32[0],rayOrigin.m128_f32[1], rayOrigin.m128_f32[2] };
	m_vRayDir_World = { rayDirection.m128_f32[0],rayDirection.m128_f32[1], rayDirection.m128_f32[2] };
}

void CPicking::Transform_ToLocalSpace(const _float4x4& WorldMatrix)
{
	XMMATRIX viewMatrix = XMLoadFloat4x4(&WorldMatrix);
	XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	XMVECTOR rayOrigin = XMLoadFloat3(&m_vRayPos_World);
	XMVECTOR rayDirection = XMLoadFloat3(&m_vRayDir_World);

	rayOrigin = XMVector3TransformCoord(rayOrigin, invViewMatrix);
	rayDirection = XMVector3TransformNormal(rayDirection, invViewMatrix);

	rayDirection = XMVector3Normalize(rayDirection);

	m_vRayPos_InLocalSpace = { rayOrigin.m128_f32[0],rayOrigin.m128_f32[1], rayOrigin.m128_f32[2] };
	m_vRayDir_InLocalSpace = { rayDirection.m128_f32[0],rayDirection.m128_f32[1], rayDirection.m128_f32[2] };
}

_bool CPicking::isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	const float EPSILON = 0.0000001f;

	XMVECTOR xmPointA = XMLoadFloat3(&vPointA);
	XMVECTOR xmPointB = XMLoadFloat3(&vPointB);
	XMVECTOR xmPointC = XMLoadFloat3(&vPointC);

	XMVECTOR xmRayPos = XMLoadFloat3(&m_vRayPos_World);
	XMVECTOR xmRayDir = XMLoadFloat3(&m_vRayDir_World);

	XMVECTOR edge1 = xmPointB - xmPointA;
	XMVECTOR edge2 = xmPointC - xmPointA;
	XMVECTOR h = XMVector3Cross(xmRayDir, edge2);
	float a = XMVectorGetX(XMVector3Dot(edge1, h));

	if (a > -EPSILON && a < EPSILON)
	{
		return false;    // 레이가 삼각형에 평행함
	}

	float f = 1.0f / a;
	XMVECTOR s = xmRayPos - xmPointA;
	float u = f * XMVectorGetX(XMVector3Dot(s, h));

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	XMVECTOR q = XMVector3Cross(s, edge1);
	float v = f * XMVectorGetX(XMVector3Dot(xmRayDir, q));

	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	// Ray가 삼각형과 교차함
	float t = f * XMVectorGetX(XMVector3Dot(edge2, q));
	if (t > EPSILON) // t가 0보다 크면 Ray가 삼각형과 교차함
	{
		*pOut = { (xmRayPos + xmRayDir * t).m128_f32[0],(xmRayPos + xmRayDir * t).m128_f32[1], (xmRayPos + xmRayDir * t).m128_f32[2] };
		return true;
	}
	else // t가 0 이하이면 Ray는 삼각형에 닿지 않음
	{
		return false;
	}
}

_bool CPicking::isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	const float EPSILON = 0.0000001f;

	XMVECTOR xmPointA = XMLoadFloat3(&vPointA);
	XMVECTOR xmPointB = XMLoadFloat3(&vPointB);
	XMVECTOR xmPointC = XMLoadFloat3(&vPointC);

	XMVECTOR xmRayPos = XMLoadFloat3(&m_vRayPos_InLocalSpace);
	XMVECTOR xmRayDir = XMLoadFloat3(&m_vRayDir_InLocalSpace);

	XMVECTOR edge1 = xmPointB - xmPointA;
	XMVECTOR edge2 = xmPointC - xmPointA;
	XMVECTOR h = XMVector3Cross(xmRayDir, edge2);
	float a = XMVectorGetX(XMVector3Dot(edge1, h));

	if (a > -EPSILON && a < EPSILON)
	{
		return false;    // 레이가 삼각형에 평행함
	}

	float f = 1.0f / a;
	XMVECTOR s = xmRayPos - xmPointA;
	float u = f * XMVectorGetX(XMVector3Dot(s, h));

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	XMVECTOR q = XMVector3Cross(s, edge1);
	float v = f * XMVectorGetX(XMVector3Dot(xmRayDir, q));

	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	// Ray가 삼각형과 교차함
	float t = f * XMVectorGetX(XMVector3Dot(edge2, q));
	if (t > EPSILON) // t가 0보다 크면 Ray가 삼각형과 교차함
	{
		*pOut = { (xmRayPos + xmRayDir * t).m128_f32[0],(xmRayPos + xmRayDir * t).m128_f32[1], (xmRayPos + xmRayDir * t).m128_f32[2] };
		return true;
	}
	else // t가 0 이하이면 Ray는 삼각형에 닿지 않음
	{
		return false;
	}
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY, CGameInstance* pGameInstance)
{
	CPicking* pInstance = new CPicking(pDevice, pContext, pGameInstance);

	if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY)))
	{
		MSG_BOX(TEXT("Failed to Created : CPicking"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
