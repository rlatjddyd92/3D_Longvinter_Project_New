#include "..\Public\UIObject.h"

CUIObject::CUIObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CUIObject::CUIObject(const CUIObject & Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void * pArg)
{
	UI_DESC*	pDesc = static_cast<UI_DESC*>(pArg);

	/* �������� �ʱ� ��ġ�� ����� �����Ѵ�. */
	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;	
	m_fShowTime = pDesc->fTime;

	_uint		iNumView = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumView, &ViewportDesc);

	/* m_ViewMatrix : XMFLOAT4X4 */
	/* XMMatrixIdentity�Լ��� ���ϰ� XMMATRIX */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());			
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));
	
	m_fViewWidth = ViewportDesc.Width;
	m_fViewHeight = ViewportDesc.Height;

	// XMVector3Equal(XMLoadFloat3(&m_vTmp), XMLoadFloat3(&m_vTmp));	

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
	
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ���� ����� ����� �ش�. */

	m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, 
		XMVectorSet(m_fX - m_fViewWidth * 0.5f, -m_fY + m_fViewHeight * 0.5f, 0.f, 1.f));	
}

HRESULT CUIObject::Render()
{
	return S_OK;
}


void CUIObject::Free()
{
	__super::Free();

}
