#include "..\Public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"


CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{

}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	// , m_Bones { Prototype.m_Bones }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
	, m_iCurrentAnimIndex{ Prototype.m_iCurrentAnimIndex }
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
	// , m_Animations { Prototype.m_Animations }
{

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
		m_Animations.emplace_back(pPrototypeAnimation->Clone());


	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.emplace_back(pPrototypeBone->Clone());

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMaterialTextures)
			Safe_AddRef(pTexture);
	}


	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);


}

_int CModel::Get_BoneIndex(const _char* pBoneName) const
{
	_uint	iBoneIndex = { 0 };
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (0 == strcmp(pBone->Get_Name(), pBoneName))
				return true;
			++iBoneIndex;
			return false;
		});

	if (iter == m_Bones.end())
		MSG_BOX(TEXT("���µ�?"));

	return iBoneIndex;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, _bool bIsLoad)
{

	

	if (bIsLoad == true)
	{
		if (FAILED(LoadModel(pModelFilePath, eType, PreTransformMatrix)))
			return E_FAIL;

		return S_OK;
	}

	/*  ������ ������ �ε��Ѵ�. ���� ( aiNode, aiBone, aiNodeAnim ) */
	/* aiNode : �θ�, �ڽ��� �, �ڽ��� ��������? , �ʱ��������� �����ͼ� �θ�κ��� �Ļ��� ����� ���Ͽ� �������λ�������� ����� �ִ� ������ �������ش�. */
	/* aiBone : �޽ÿ� ���ԵǾ��ִ� ���� ������ �ǹ�. �� ���� � �޽��� �����鿡�� ������ �ִ���? �󸶳� ���⤷���ִ���? �󸶳� ����(offset)���ʿ�����? */
	/* aiNodeAnim(== Channel) : �ִϸ��̼ǿ��� ����ϴ� ���� ������ �ǹ�. �� �ִϸ��̼��� �����Կ� �־� � �ð��뿡 �� ���� � ���¸� ���ؾ��ϴ����� ���� ��������� �����ϱ����� ��Ḧ �����Ѵ�. */
	_uint		iFlag = { 0 };

	/* ���� : ��� �޽ð� �� ������ �������� �׎´ϴ�. */
	/* ���� : ���� �����ϴ� ��� �޽õ��� ���� ������ ����(�޽ø� ��ġ�ϱ����� ������ ��ġ�� ���缭)��� �̸� ��ȯ���ش�.*/

	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (eType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;


	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
	m_eType = eType;
	
	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::SaveModel(const _char* pModelFilePath)
{
	string fileName = pModelFilePath;
	WCHAR* TempName = {};
	TempName = new WCHAR[fileName.size()];
	memset(TempName, 0, sizeof(TempName));
	for (_int i = 0; i <= fileName.size(); ++i)
		TempName[i] = fileName[i];
	
	HANDLE hFile = CreateFile(TempName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte(0);
	
	WriteFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);
	for (auto& iter : m_Meshes)
		iter->SaveModel(hFile, &dwByte);

	WriteFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	_int iSize = m_Materials_Texture.size();

	WriteFile(hFile, &iSize, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < m_Materials_Texture.size(); ++i)
	{
		for (_int j = 0; j < MAX_PATH; ++j)
		{
			_tchar temp = m_Materials_Texture[i][j];
			WriteFile(hFile, &temp, sizeof(_tchar), &dwByte, nullptr);
		}
			
		WriteFile(hFile, &m_Materials_Exist[i], sizeof(_int), &dwByte, nullptr);
	}

	iSize = m_Bones.size();
	WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
	for (auto& iter : m_Bones)
		iter->SaveModel(hFile, &dwByte);

	iSize = m_Animations.size();
	WriteFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);
	for (auto& iter : m_Animations)
		iter->SaveModel(hFile, &dwByte);

	CloseHandle(hFile);
}

HRESULT CModel::LoadModel(const _char* pModelFilePath, TYPE eType, _fmatrix PreTransformMatrix)
{
	string fileName = pModelFilePath;
	WCHAR* TempName = new WCHAR[fileName.size() + 1];
	memset(TempName, 0, sizeof(TempName));
	for (_int i = 0; i <= fileName.size(); ++i)
		TempName[i] = fileName[i];
	
	HANDLE hFile = CreateFile(TempName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Safe_Delete_Array(TempName);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	m_eType = eType;

	DWORD dwByte(0);

	ReadFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);
	for (_int i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, this, hFile, &dwByte, XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}
	
	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	m_Materials.resize(m_iNumMaterials);

	_int iTextures = 0;

	ReadFile(hFile, &iTextures, sizeof(_int), &dwByte, nullptr);

	for (_int i = 0; i < iTextures; ++i)
	{
		_tchar* Temp = new _tchar[MAX_PATH];
		m_Materials_Texture.push_back(Temp);
		for (_int j = 0; j < MAX_PATH; ++j)
			ReadFile(hFile, &Temp[j], sizeof(_tchar), &dwByte, nullptr);

		_int iTemp = 0;
		ReadFile(hFile, &iTemp, sizeof(_int), &dwByte, nullptr);
		m_Materials_Exist.push_back(iTemp);
		m_Materials[m_Materials_Exist[i] / 100].pMaterialTextures[m_Materials_Exist[i] % 100] = CTexture::Create(m_pDevice, m_pContext, m_Materials_Texture[i], 1);
	}

	_uint iBones = 0;
	ReadFile(hFile, &iBones, sizeof(_uint), &dwByte, nullptr);
	for (_int i = 0; i < iBones; ++i)
	{
		CBone* pBone = CBone::Create(hFile, &dwByte, -1);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.emplace_back(pBone);
	}

	ReadFile(hFile, &m_iNumAnimations, sizeof(_uint), &dwByte, nullptr);
	for (_int i = 0; i < iBones; ++i)
	{
		CAnimation* pAnie = CAnimation::Create(hFile, &dwByte, this);
		if (nullptr == pAnie)
			return E_FAIL;

		m_Animations.emplace_back(pAnie);
	}

	

	CloseHandle(hFile);

	return S_OK;
}

_bool CModel::Play_Animation(_float fTimeDelta)
{
	/* ���� �����δ�.(CBone`s m_TransformationMatrix����� �����Ѵ�.) */
	_bool isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, m_isLoop, fTimeDelta * m_fAnimSpeed);

	/* ��� ���� ������ �ִ� m_CombinedTransformationMatrix�� �����Ѵ�. */
	for (auto& pBone : m_Bones)
	{
		/* �� ���� ��� * �θ��� �Ĺ��ε� */
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}

	return isFinished;
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex)
{
	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	return m_Materials[iMaterialIndex].pMaterialTextures[eMaterialType]->Bind_ShadeResource(pShader, pConstantName, 0);
}

HRESULT CModel::Bind_MeshBoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_BoneMatrices(this, pShader, pConstantName);

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, this, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MESH_MATERIAL		MeshMaterial{};

		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			// pAIMaterial->GetTextureCount(j);
			aiString			strTexturePath;

			_char				szDrive[MAX_PATH] = "";
			_char				szDirectory[MAX_PATH] = "";
			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_char				szTexturePath[MAX_PATH] = "";

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTexturePath, szDrive);
			strcat_s(szTexturePath, szDirectory);
			strcat_s(szTexturePath, szFileName);
			strcat_s(szTexturePath, szExt);

			_tchar				szFinalPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTexturePath, strlen(szTexturePath), szFinalPath, MAX_PATH);

			MeshMaterial.pMaterialTextures[j] = CTexture::Create(m_pDevice, m_pContext, szFinalPath, 1);

			_tchar* szTemp = new _tchar[MAX_PATH];
			for (_int z = 0; z < MAX_PATH; ++z)
				szTemp[z] = szFinalPath[z];

			m_Materials_Texture.push_back(szTemp);
			m_Materials_Exist.push_back((i * 100) + j);

			if (nullptr == MeshMaterial.pMaterialTextures[j])
				return E_FAIL;
		}

		m_Materials.emplace_back(MeshMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAIBone, _int iParentBoneIndex)
{
	/* ���� ������ �����Ѵ�. ���� �ؾ��� ��(�ڽĻ��� ���¸� ��ȯ�Ѵ�. * �θ���� ����� ���Ͽ� ������������� �����.)*/
	CBone* pBone = CBone::Create(pAIBone, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.emplace_back(pBone);

	_int		iParentIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAIBone->mNumChildren; i++)
	{
		Ready_Bones(pAIBone->mChildren[i], iParentIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}
	return S_OK;
}


CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, _bool IsLoad)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix, IsLoad)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMaterialTextures)
			Safe_Release(pTexture);
	}
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
	{
		Safe_Release(pMesh);
	}
	m_Meshes.clear();

	for (auto& pMesh : m_Materials_Texture)
	{
		Safe_Delete(pMesh);
	}
	m_Materials_Texture.clear();
	
	m_Materials_Exist.clear();
}
