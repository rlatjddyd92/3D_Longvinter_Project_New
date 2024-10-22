#include "..\Public\Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	auto	iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize()
{

	return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc, _int iFrame)
{
	CLight* pLight = CLight::Create(LightDesc);
	pLight->SetFrame(iFrame);
	if (nullptr == pLight)
		return E_FAIL;

	if (m_Lights.size() > 5)
		for (list<CLight*>::iterator pLight = m_Lights.begin(); pLight != m_Lights.end(); ++pLight)
			if ((*pLight)->GetFrame() > 0)
			{
				Safe_Release((*pLight));
				m_Lights.erase(pLight);
				break;
			}

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (list<CLight*>::iterator pLight = m_Lights.begin(); pLight != m_Lights.end();)
	{
		(*pLight)->Render(pShader, pVIBuffer);

		if ((*pLight)->GetDead())
		{
			Safe_Release((*pLight));
			pLight = m_Lights.erase(pLight);
		}
		else
			++pLight;
	}

	return S_OK;
}

CLight_Manager* CLight_Manager::Create()
{
	CLight_Manager* pInstance = new CLight_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Cloned : CLight_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLight_Manager::Free()
{
	__super::Free();

	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}