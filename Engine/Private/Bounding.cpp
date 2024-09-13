#include "..\Public\Bounding.h"

CBounding::CBounding(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
}

HRESULT CBounding::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	return S_OK;
}

HRESULT CBounding::Initialize(CBounding::BOUNDING_DESC* pBoundingDesc)
{
	return S_OK;
}

void CBounding::Update(_fmatrix WorldMatrix)
{

}

void CBounding::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Delete(m_pBoundingDesc_AABB);
	Safe_Delete(m_pOriginalBoundingDesc_AABB);
	Safe_Delete(m_pBoundingDesc_OBB);
	Safe_Delete(m_pOriginalBoundingDesc_OBB);
	Safe_Delete(m_pBoundingDesc_Sphere);

}
