#include "..\Public\Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.emplace_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Priority_Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_GameObjects)
        if (!pGameObject->GetOff())
            pGameObject->Priority_Update(fTimeDelta);

	return S_OK;
}

HRESULT CLayer::Update(_float fTimeDelta)
{
    for (auto& pGameObject : m_GameObjects)
        if (!pGameObject->GetOff())
            pGameObject->Update(fTimeDelta);

	return S_OK;
}

HRESULT CLayer::Late_Update(_float fTimeDelta)
{
    for (list<CGameObject*>::iterator iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
    {
        if ((*iter)->GetDead() == true)
        {
            Safe_Release(*iter);
            iter = m_GameObjects.erase(iter);
        }
        else if (!(*iter)->GetOff())
        {
            (*iter)->Late_Update(fTimeDelta);
            ++iter;
        }
        else 
            ++iter;
    }

	return S_OK;
}

CGameObject* CLayer::Get_GameObject(_int iIndex)
{
    if (iIndex == -1)
        iIndex = m_GameObjects.size() - 1;

    auto iter = begin(m_GameObjects);
    int i = 0;
    for (; i < iIndex && iter != end(m_GameObjects); i++)
        ++iter;
    if (iter == end(m_GameObjects)) {
        return nullptr;
    }
    return *iter;
}

HRESULT CLayer::Delete_GameObject(CGameObject* pGameObject)
{
    auto iter = begin(m_GameObjects);
    int i = 0;
    for (; i < m_GameObjects.size(); i++) {
        if ((*iter) == pGameObject) {
            Safe_Release(*iter);
            iter = m_GameObjects.erase(iter);
            return S_OK;
        }
        else {
            ++iter;
        }
    }

    return E_FAIL;
}

HRESULT CLayer::Delete_GameObject(_int iIndex)
{
    if (iIndex == -1)
        iIndex = m_GameObjects.size() - 1;

    auto iter = begin(m_GameObjects);
    int i = 0;
    for (; i < iIndex && iter != end(m_GameObjects); i++)
        ++iter;
    if (iter != end(m_GameObjects))
    {
        Safe_Release(*iter);
        iter = m_GameObjects.erase(iter);
        return S_OK;
    }

    return E_FAIL;
}

CComponent* CLayer::Find_Component(const _wstring& strComponentTag, _int iIndex)
{
    if (iIndex == -1)
        iIndex = m_GameObjects.size() - 1;

    if (iIndex >= m_GameObjects.size())
        return nullptr;

    auto	iter = m_GameObjects.begin();

    for (size_t i = 0; i < iIndex; i++)
        ++iter;

    return (*iter)->Find_Component(strComponentTag);
}

CGameObject* CLayer::Find_Object(_int iIndex)
{
    if (iIndex == -1)
        iIndex = m_GameObjects.size() - 1;

    if (iIndex >= m_GameObjects.size())
        return nullptr;

    auto	iter = m_GameObjects.begin();

    for (size_t i = 0; i < iIndex; i++)
        ++iter;

    return *(iter);
}

CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}

