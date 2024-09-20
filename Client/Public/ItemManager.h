#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Tool.h"

// [������ �Ŵ���] 
// 1. ��� �������� ����, �ؽ���(�κ�)�� �����Ѵ� 
// 2. ���� ���� ������ ����(�κ��丮)�� �����Ѵ� 
// 3. �ܺ��� ��û�� ���� �������� ����, ��, �ؽ��ĸ� �����Ѵ� 

BEGIN(Engine)

END

BEGIN(Client)

class CItemManager : public CGameObject
{
public:
	typedef struct ItemInfo // <- ������ ��ü�� ����, �̸� �� �� �� �ش� �������� ������ ���� 
	{
		ITEMINDEX eIndex = ITEMINDEX::ITEM_END;
		wstring strItemName = TEXT("ITEM_DEFALUT_NAME");
		wstring strItemDescription = TEXT("ITEM_DEFAULT_description"); // <- ������ ���� 
		_bool bIsTool = false; // <- true�� ��� ������ ���� ���� �־�� �Ѵ� 

		ITEMTYPE eType = ITEMTYPE::ITEM_TYPE_END;
		_bool bStack = true; // ���� ������ ������ ����


		// �Ʒ��� ���� ������ ��ġ 
		_float fHp = 100.f; // ������ ������ 
		_int iPrice = 100; // ������ ����
		_float fAttack = 10.f;
		_float fDefence = 1.f;

		_int iCount = 1;
		_bool bPicked = false;

	}TINFO;

	typedef struct ShopInfo
	{
		vector<TINFO> vecGoods; // <- ���� Ȥ�� ���ڰ� ������ ������ ��� 
	}SINFO;

	enum ITEMARRAY
	{
		ARRAY_INFO, 
		ARRAY_INVEN,
		ARRAY_EQUIP,
		ARRAY_SHOP,
		ARRAY_END
	};

protected:
	CItemManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemManager(const CItemManager& Prototype);
	virtual ~CItemManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const TINFO& GetItemInfo(ITEMINDEX eIndex) { return m_vecItemInfo[_int(eIndex)]; }
	const TINFO& GetInvenInfo(_uint iInvenNum)
	{
		if (iInvenNum >= INVEN_ROW * INVEN_COL)
		{
			TINFO tFali;
			return tFali;
			// ���а��� ��ȯ ��, �ʱ�ȭ �� ���� ���� TINFO ��ȯ, �ش� ����ü�� ITEMINDEX�� END�̹Ƿ� ���� ����  
		}
			
		if (m_bUsingSelectBuffer)
			return m_vecInvenSelectBuffer[iInvenNum];
		else
			return m_vecInvenInfo[iInvenNum];
	}
	const TINFO& GetEquipInfo(_uint iEquipNum)
	{
		if (iEquipNum >= 5)
		{
			TINFO tFali;
			return tFali;
			// ���а��� ��ȯ ��, �ʱ�ȭ �� ���� ���� TINFO ��ȯ, �ش� ����ü�� ITEMINDEX�� END�̹Ƿ� ���� ����  
		}

		return m_vecEquipInfo[iEquipNum];
	}
	CTexture* GetItemInvenTexture(ITEMINDEX eIndex)
	{
		if (_int(eIndex) >= _int(ITEMINDEX::ITEM_END))
			return nullptr;

		return m_vecItemInvenTexture[_int(eIndex)];
	}





public: // <- ������ �Ŵ��� �ʹ� ���ÿ� �Լ� 

	// 1. ������ ���� ���� 
	HRESULT Setting_ItemInfo();




public:  // <- ������ ����, �̵��� ���� �Լ� 
	const TINFO PickItem(ITEMARRAY eArray, _int iIndex);
	void CancelPick();
	HRESULT PutInItem(ITEMARRAY eArray, _int iIndex);

	ITEMINDEX GetPickedItemIndex() { return m_tPickedItem.eIndex; }


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };


private: // <- ������ ���� ����
	vector<TINFO> m_vecItemInfo; // <- �������� ���� 
	vector<TINFO> m_vecInvenInfo; // <- ������ ������ ������ ���� 

	_bool m_bUsingSelectBuffer = false;
	vector<TINFO> m_vecInvenSelectBuffer; // <- ������ �˻�, Ȥ�� Ư�� ������ �����۸� ���� �� ���
	vector<TINFO> m_vecEquipInfo; // <- ������ ������ ������ ���� 


	vector<CTool*> m_vecTool; // <- ����, ���� �𵨿� �پ� �ִ� ��Ʈ ������Ʈ ���, ������ �ε����� ���� 
	vector<CTexture*> m_vecItemInvenTexture; 

private: // <- ����, ���� ���� ����
	_int m_iNowKey = 1; // <- ����, ���� ���� ��, Ű�� �����ϱ� ���� ����, ����� �� ���� �����ϸ� �̹� ����� Ű�� �������� �ʴ´� 
	map<_int, SINFO> m_mapShopInfo; // <- ���� NPC, ���Ǳ�, ������ ���� -> Ű�� �����Ѵ�  


	TINFO m_tPickedItem{}; // <- ���� ���콺�� ����ø� �������� ����
	ITEMARRAY m_eBeforeArray = ITEMARRAY::ARRAY_END; // <- ���� �������� ������ �־��� �迭
	_int m_iBeforeShopKey = -1; // <- �������� ����� ��� �ش� ������ Key
	_int m_iBeforeIndex = -1; // <- �������� �迭���� ��� ������ �־��� �� �ۼ� 


public:
	static CItemManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END