#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Tool.h"

// [아이템 매니저] 
// 1. 모든 아이템의 정보, 텍스쳐(인벤)을 저장한다 
// 2. 유저 보유 아이템 정보(인벤토리)를 관리한다 
// 3. 외부의 요청에 따라 아이템의 정보, 모델, 텍스쳐를 제공한다 

BEGIN(Engine)

END

BEGIN(Client)

class CItemManager : public CGameObject
{
public:
	typedef struct ItemInfo // <- 아이템 자체의 정보, 이름 및 모델 등 해당 아이템의 스펙을 저장 
	{
		ITEMINDEX eIndex = ITEMINDEX::ITEM_END;
		wstring strItemName = TEXT("ITEM_DEFALUT_NAME");
		wstring strItemDescription = TEXT("ITEM_DEFAULT_description"); // <- 아이템 툴팁 
		_bool bIsTool = false; // <- true인 경우 장착을 위한 모델이 있어야 한다 

		ITEMTYPE eType = ITEMTYPE::ITEM_TYPE_END;
		_bool bStack = true; // 스택 가능한 아이템 여부


		// 아래는 변동 가능한 수치 
		_float fHp = 100.f; // 아이템 내구도 
		_int iPrice = 100; // 아이템 가격
		_float fAttack = 10.f;
		_float fDefence = 1.f;

		_int iCount = 1;
		_bool bPicked = false;

	}TINFO;

	typedef struct ShopInfo
	{
		vector<TINFO> vecGoods; // <- 상점 혹은 상자가 보유한 아이템 목록 
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
			// 실패값을 반환 시, 초기화 된 값을 가진 TINFO 반환, 해당 구조체의 ITEMINDEX는 END이므로 구분 가능  
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
			// 실패값을 반환 시, 초기화 된 값을 가진 TINFO 반환, 해당 구조체의 ITEMINDEX는 END이므로 구분 가능  
		}

		return m_vecEquipInfo[iEquipNum];
	}
	CTexture* GetItemInvenTexture(ITEMINDEX eIndex)
	{
		if (_int(eIndex) >= _int(ITEMINDEX::ITEM_END))
			return nullptr;

		return m_vecItemInvenTexture[_int(eIndex)];
	}





public: // <- 아이템 매니저 초반 세팅용 함수 

	// 1. 아이템 정보 세팅 
	HRESULT Setting_ItemInfo();




public:  // <- 아이템 집기, 이동을 위한 함수 
	const TINFO PickItem(ITEMARRAY eArray, _int iIndex);
	void CancelPick();
	HRESULT PutInItem(ITEMARRAY eArray, _int iIndex);

	ITEMINDEX GetPickedItemIndex() { return m_tPickedItem.eIndex; }


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };


private: // <- 아이템 관련 변수
	vector<TINFO> m_vecItemInfo; // <- 아이템의 정보 
	vector<TINFO> m_vecInvenInfo; // <- 유저가 보유한 아이템 정보 

	_bool m_bUsingSelectBuffer = false;
	vector<TINFO> m_vecInvenSelectBuffer; // <- 아이템 검색, 혹은 특정 조건의 아이템만 노출 시 사용
	vector<TINFO> m_vecEquipInfo; // <- 유저가 장착한 아이템 정보 


	vector<CTool*> m_vecTool; // <- 유저, 적의 모델에 붙어 있는 파트 오브젝트 목록, 아이템 인덱스로 접근 
	vector<CTexture*> m_vecItemInvenTexture; 

private: // <- 상점, 상자 관련 변수
	_int m_iNowKey = 1; // <- 상점, 상자 생성 시, 키를 배정하기 위한 변수, 사용할 때 마다 증가하며 이미 사용한 키는 재사용하지 않는다 
	map<_int, SINFO> m_mapShopInfo; // <- 상점 NPC, 자판기, 상자의 정보 -> 키로 접근한다  


	TINFO m_tPickedItem{}; // <- 지금 마우스로 집어올린 아이템의 정보
	ITEMARRAY m_eBeforeArray = ITEMARRAY::ARRAY_END; // <- 집은 아이템이 이전에 있었던 배열
	_int m_iBeforeShopKey = -1; // <- 상점에서 집어온 경우 해당 상점의 Key
	_int m_iBeforeIndex = -1; // <- 아이템이 배열에서 어느 지점에 있었는 지 작성 


public:
	static CItemManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END