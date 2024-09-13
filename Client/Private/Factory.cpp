#include "stdafx.h"
#include "Factory.h"



CFactory::CFactory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pGameInstance = pGameInstance;
}

HRESULT CFactory::Setting_Program_Start()
{
	Ready_Prototype_Shader();
	Ready_Prototype_Buffer();
	Ready_Prototype_Texture();

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_SINGLE), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/* For. Prototype_Component_Model_Player*/

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model(CModel::TYPE_ANIM, TEXT("Prototype_Component_Model_Player"), "../Bin/Resources/Models/Player/Player", PreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Part()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_Container()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SurFace"),
		CSurFace::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(Ready_Prototype_UIPart()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_UIPage()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Setting_Ingame_Start()
{
	return S_OK;
}

HRESULT CFactory::Setting_Editor_Start()
{

	return S_OK;
}

void CFactory::Save_Prototype_Model_Data()
{
	list<CModel*>::iterator iterModel = m_ModelPrototypelist.begin();
	list<_char*>::iterator iterFile = m_ModelFilePath.begin();

	while ((iterModel != m_ModelPrototypelist.end()) && (iterFile != m_ModelFilePath.end()))
	{
		(*iterModel)->SaveModel((*iterFile));
		++iterModel;
		++iterFile;
	}

}

void CFactory::Make_Container_Player(_float3 Position)
{
	CGameObject::GAMEOBJECT_DESC		pTemp{};
	pTemp.fPosition = Position;
	pTemp.fSpeedPerSec = 10.0f;
	pTemp.fRotationPerSec = XMConvertToRadians(180.0f);
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), TEXT("Prototype_GameObject_Container_Player"), &pTemp);
}

HRESULT CFactory::Ready_Prototype_Texture()
{
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LandTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/LandTexture/texture%d.dds"), LTEXTUREMAX))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BGA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Picture/BGA%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MainLogo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Picture/MainLogo.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Base"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Button/Button_Base.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_UI_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Back/UI_Back.dds"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Part()
{
	/* For. Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Body_Human"),
		CBody_Human::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Empty"),
		CTool_Empty::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Container()
{
	/* For. Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Container_Player"),
		CContainer_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Shader()
{
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	D3DCompileFromFile(L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl", nullptr, nullptr, "PS_Main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, nullptr, nullptr);

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxSurFace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSurFace.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Buffer()
{
	/* For. Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Rect3D */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect3D"),
		CVIBuffer_Rect3D::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_UIPart()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Back"),CUIPart_Back::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Button"), CUIPart_Button::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Picture"), CUIPart_Picture::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CFactory::Ready_Prototype_UIPage()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Main"), CUIPage_Main::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Model(CModel::TYPE eType, const _wstring strTag, const _char* pPath, _fmatrix PreTransformMatrix)
{
	string strType = ".dat";
	string strTemp = {};

	_int i = 0;
	while (*(pPath + i) != '\0')
	{
		strTemp += *(pPath + i);
		++i;
	}

	string strPath = strTemp + strType;

	CModel* pNew = CModel::Create(m_pDevice, m_pContext, eType, strPath.c_str(), PreTransformMatrix, true);
	if (pNew == nullptr)
	{
		strType = ".fbx";
		strPath = strTemp + strType;
		pNew = CModel::Create(m_pDevice, m_pContext, eType, strPath.c_str(), PreTransformMatrix);

		m_ModelPrototypelist.push_back(pNew);
		if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), strTag, pNew)))
			return E_FAIL;

		strType = ".dat";
		strPath = strTemp + strType;
		m_ModelFilePath.push_back("");
		m_ModelFilePath.back() = new _char[strPath.size()];
		memset(m_ModelFilePath.back(), 0, sizeof(_char) * strPath.size());
		for (_uint i = 0; i <= strPath.size(); ++i)
			*(m_ModelFilePath.back() + i) = strPath[i];

	}
	else
		if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), strTag, pNew)))
			return E_FAIL;

	return S_OK;
}

CUIPart_Back* CFactory::MakeUIPart_Back(CUIPart_Back::UIBACK_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CUIPart_Back::UIBack_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Back"), TEXT("Prototype_UIPart_Back"), &pTemp);

	return static_cast<CUIPart_Back*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Back"), -1));
}

CUIPart_Button* CFactory::MakeUIPart_Button(CUIPart_Button::UIBUTTON_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CUIPart_Button::UIButton_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Button"), TEXT("Prototype_UIPart_Button"), &pTemp);

	return static_cast<CUIPart_Button*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Button"), -1));
}

CUIPart_Picture* CFactory::MakeUIPart_Picture(CUIPart_Picture::UIPICTURE_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CUIPart_Picture::UIPicture_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Picture"), TEXT("Prototype_UIPart_Picture"), &pTemp);

	return static_cast<CUIPart_Picture*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Picture"), -1));
}

CUIPage_Main* CFactory::MakeUIPage_Main()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Main"), TEXT("Prototype_UIPage_Main"));

	return static_cast<CUIPage_Main*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Main"), -1));
}

CFactory* CFactory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance)
{
	CFactory* pInstance = new CFactory(pDevice, pContext, pGameInstance);

	if (FAILED(pInstance->Setting_Program_Start()))
	{
		MSG_BOX(TEXT("Failed to Created : CFactory"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFactory::Free()
{
	__super::Free();

	for (auto& iter : m_ModelPrototypelist)
		Safe_Release(iter);
	m_ModelPrototypelist.clear();


	m_ModelFilePath.clear();
}
