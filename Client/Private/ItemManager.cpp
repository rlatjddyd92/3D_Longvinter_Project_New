#include "stdafx.h"
#include "ItemManager.h"
#include "ClientInstance.h"

CItemManager::CItemManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice ,pContext }
{
}

CItemManager::CItemManager(const CItemManager& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CItemManager::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemManager::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Setting_ItemInfo();

	return S_OK;
}

void CItemManager::Priority_Update(_float fTimeDelta)
{
}

void CItemManager::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIMouseState(DIMK_RBUTTON))
		CancelPick();
}

void CItemManager::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CItemManager::Render()
{
	while (!m_Render_Model_list.empty())
	{
		RENDERINFO tTemp = m_Render_Model_list.front();
		m_Render_Model_list.pop_front();

		if (tTemp.eIndex == ITEMINDEX::ITEM_END)
			continue;

		if (m_vecTool[_int(tTemp.eIndex)] == nullptr)
			continue;

		m_vecTool[_int(tTemp.eIndex)]->SetParentState(tTemp.pParentState);
		m_vecTool[_int(tTemp.eIndex)]->SetSocketMatrix(tTemp.pSocketMatrix);
		m_vecTool[_int(tTemp.eIndex)]->SetRenderMatrix(tTemp.pParentMatrix);

		m_vecTool[_int(tTemp.eIndex)]->Render();
	}

	m_Render_Model_list.clear();

	return S_OK;
}

HRESULT CItemManager::Setting_ItemInfo()
{
	m_vecItemInfo.resize(_int(ITEMINDEX::ITEM_END));
	m_vecInvenInfo.resize(INVEN_COL * INVEN_ROW);
	m_vecEquipInfo.resize(_int(EQUIPSLOT::SLOT_END));
	m_vecItemInvenTexture.resize(_int(ITEMINDEX::ITEM_END));
	m_vecTool.resize(_int(ITEMINDEX::ITEM_END));
	m_vecItemTag.resize(_int(ITEMINDEX::ITEM_END));

	_uint iTemp = 0;
	CTool::TOOL_DESC		ToolDesc{};
	ToolDesc.pParentState = &iTemp;
	ToolDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	// źȯ
	m_vecItemInfo[_int(ITEMINDEX::ITEM_AMMO)] =
	{ ITEMINDEX::ITEM_AMMO, TEXT("źȯ"), TEXT("���� �� ����� źȯ"), false,  ITEMTYPE::ITEM_TYPE_AMMO, false, 200.f, 100, 100.f, 1.f, 0.f};
	m_vecItemTag[_int(ITEMINDEX::ITEM_AMMO)].resize(_int(ITEMTAG::ITEM_TAG_END));

	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Ammo"),
		TEXT("Com_Texture_0"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_AMMO)]))))
		return E_FAIL;
	
	// ���� ŰƮ
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)] =
	{ ITEMINDEX::ITEM_SHOTGUN_KIT, TEXT("���� ���� ŰƮ"), TEXT("��� ��, ���� 1�� ȹ��"), false, ITEMTYPE::ITEM_TYPE_ETC, false, 100.f, 1000, 0.f, 1.f,0.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun_Kit"),
		TEXT("Com_Texture_1"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_SHOTGUN_KIT)][_int(ITEMTAG::ITEM_TAG_HIGHPRICE)] = true;
	
	// ����
	m_vecItemInfo[_int(ITEMINDEX::ITEM_SHOTGUN)] =
	{ ITEMINDEX::ITEM_SHOTGUN, TEXT("����"), TEXT("����� ������ �������� ������ �˹��ŵ�ϴ�."), true, ITEMTYPE::ITEM_TYPE_WEAPON, false, 100.f, 1500, 500.f, 1.f, 0.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun"),
		TEXT("Com_Texture_2"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_SHOTGUN)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_SHOTGUN)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_SHOTGUN)][_int(ITEMTAG::ITEM_TAG_HEAVYDEMAGE)] = true;
	m_vecItemTag[_int(ITEMINDEX::ITEM_SHOTGUN)][_int(ITEMTAG::ITEM_TAG_PUSH)] = true;
	
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_ShotGun"), TEXT("Prototype_GameObject_Tool_ShotGun"), &ToolDesc);
	m_vecTool[_int(ITEMINDEX::ITEM_SHOTGUN)] = static_cast<CTool*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_ShotGun"), -1));



	m_vecItemInfo[_int(ITEMINDEX::ITEM_ARROW)] =
	{ ITEMINDEX::ITEM_ARROW, TEXT("ȭ��"), TEXT("������ ���� óġ�� �� ����մϴ�."), false,  ITEMTYPE::ITEM_TYPE_AMMO, false, 250.f, 5100, 100.f, 1.f, 15.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconArrows10"),
		TEXT("Com_Texture_3"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_ARROW)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_ARROW)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_ARROW)][_int(ITEMTAG::ITEM_TAG_QUIET)] = true;


	m_vecItemInfo[_int(ITEMINDEX::ITEM_CHAINSAW)] =
	{ ITEMINDEX::ITEM_CHAINSAW, TEXT("������"), TEXT("������ �� �� ����ϸ�, �����Ե� ����� �� �ֽ��ϴ�."), false,  ITEMTYPE::ITEM_TYPE_WEAPON, false, 20.f, 3100, 1400.f, 1.f, 9.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconChainsaw"),
		TEXT("Com_Texture_4"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_CHAINSAW)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_CHAINSAW)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_CHAINSAW)][_int(ITEMTAG::ITEM_TAG_HEAVYDEMAGE)] = true;


	m_vecItemInfo[_int(ITEMINDEX::ITEM_CHRISTMAS_HAT)] =
	{ ITEMINDEX::ITEM_CHRISTMAS_HAT, TEXT("��Ÿ ����"), TEXT("ũ�������� ���� ������, ������ ��ΰ� �� �� �ֽ��ϴ�."), false,  ITEMTYPE::ITEM_TYPE_HAT, false, 700.f, 1200, 120.f, 11.f, 34.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Christmas_Hat"),
		TEXT("Com_Texture_5"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_CHRISTMAS_HAT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_CHRISTMAS_HAT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_CHRISTMAS_HAT)][_int(ITEMTAG::ITEM_TAG_HIGHPRICE)] = true;


	m_vecItemInfo[_int(ITEMINDEX::ITEM_COAL_INGOT)] =
	{ ITEMINDEX::ITEM_COAL_INGOT, TEXT("������ ��ź"), TEXT("���� ȭ���� ���� �� �ֽ��ϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 10.f, 900, 11200.f, 231.f, 44.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Coal_ingot"),
		TEXT("Com_Texture_6"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_COAL_INGOT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_COAL_INGOT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_COAL_INGOT)][_int(ITEMTAG::ITEM_TAG_REFINED)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_COAL_ORE_CHUNK)] =
	{ ITEMINDEX::ITEM_COAL_ORE_CHUNK, TEXT("��ź ���"), TEXT("ȭ���� �����ϴ�. �����ϸ� �� ���� ȭ���� ���� �� �ֽ��ϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 150.f, 78100, 340.f, 23.f , 23.f};
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Coal_ore_chunk"),
		TEXT("Com_Texture_7"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_COAL_ORE_CHUNK)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_COAL_ORE_CHUNK)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_COPPER_INGOT)] =
	{ ITEMINDEX::ITEM_COPPER_INGOT, TEXT("���� �ֱ�"), TEXT("���� ���� ���ۿ� ����մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 800.f, 16500,4400.f, 45.f, 52.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Copper_ingot"),
		TEXT("Com_Texture_8"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_COPPER_INGOT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_COPPER_INGOT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_COPPER_INGOT)][_int(ITEMTAG::ITEM_TAG_REFINED)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_COPPER_ORE_CHUNK)] =
	{ ITEMINDEX::ITEM_COPPER_ORE_CHUNK, TEXT("���� ���"), TEXT("���ۿ� ����Ϸ��� �����ؾ� �մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 10.f, 13400, 50.f, 23.f, 58.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Copper_ore_chunk"),
		TEXT("Com_Texture_9"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_COPPER_ORE_CHUNK)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_COPPER_ORE_CHUNK)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_DOUBLESHOTGUN)] =
	{ ITEMINDEX::ITEM_DOUBLESHOTGUN, TEXT("���� �跲 ����"), TEXT("�� ���� ȭ������ ���� ������������"), false,  ITEMTYPE::ITEM_TYPE_WEAPON, false, 190.f, 600, 3400.f, 12.f, 223.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconDoubleShotgun"),
		TEXT("Com_Texture_10"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_DOUBLESHOTGUN)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_DOUBLESHOTGUN)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_FISH)] =
	{ ITEMINDEX::ITEM_FISH, TEXT("�����"), TEXT("���� �� ������ ������ �����ϰų� �����ϼ���"), false,  ITEMTYPE::ITEM_TYPE_FOOD, false, 200.f, 7800, 4500.f, 33.f, 890.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconAnyFish"),
		TEXT("Com_Texture_11"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_FISH)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_FISH)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_FISH)][_int(ITEMTAG::ITEM_TAG_CANROTTEN)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_IRON_INGOT)] =
	{ ITEMINDEX::ITEM_IRON_INGOT, TEXT("ö �ֱ�"), TEXT("���� ���� ���ۿ� ����մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 220.f, 2300, 340.f, 37.f, 34.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Iron_ingot"),
		TEXT("Com_Texture_12"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_IRON_INGOT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_IRON_INGOT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_IRON_INGOT)][_int(ITEMTAG::ITEM_TAG_REFINED)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_IRON_ORE_CHUNK)] =
	{ ITEMINDEX::ITEM_IRON_ORE_CHUNK, TEXT("ö ���"), TEXT("���ۿ� ����Ϸ��� �����ؾ� �մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 500.f, 1760, 330.f, 34.f , 87.f};
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Iron_ore_chunk"),
		TEXT("Com_Texture_13"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_IRON_ORE_CHUNK)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_IRON_ORE_CHUNK)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_GOLD_INGOT)] =
	{ ITEMINDEX::ITEM_GOLD_INGOT, TEXT("�� �ֱ�"), TEXT("��� ���ݿ� �Ȱų� Ư�� ������ ���ۿ� ����մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 900.f, 4500, 4560.f, 56.f, 4311.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Gold_ingot"),
		TEXT("Com_Texture_14"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_GOLD_INGOT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_GOLD_INGOT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_GOLD_INGOT)][_int(ITEMTAG::ITEM_TAG_REFINED)] = true;
	m_vecItemTag[_int(ITEMINDEX::ITEM_GOLD_INGOT)][_int(ITEMTAG::ITEM_TAG_HIGHPRICE)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_GOLD_ORE_CHUNK)] =
	{ ITEMINDEX::ITEM_GOLD_ORE_CHUNK, TEXT("�� ���"), TEXT("��ΰ� �� �� ������ ���ۿ� ����Ϸ��� �����ؾ� �մϴ�."), false,  ITEMTYPE::ITEM_TYPE_MINERAL, false, 120.f, 3400, 340.f, 33.f, 87.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Gold_ore_chunk"),
		TEXT("Com_Texture_15"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_GOLD_ORE_CHUNK)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_GOLD_ORE_CHUNK)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_GOLD_ORE_CHUNK)][_int(ITEMTAG::ITEM_TAG_HIGHPRICE)] = true;

	m_vecItemInfo[_int(ITEMINDEX::ITEM_JAM)] =
	{ ITEMINDEX::ITEM_JAM, TEXT("��"), TEXT("���� �� ��︮�� ���� ���ݿ� �� ���� �ֽ��ϴ�."), false,  ITEMTYPE::ITEM_TYPE_FOOD, false, 9400.f, 2300, 330.f, 54.f, 38.f };
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconCloudberryJam"),
		TEXT("Com_Texture_16"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_JAM)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_JAM)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_MACHINGUN_CRAFTINGKIT)] =
	{ ITEMINDEX::ITEM_MACHINGUN_CRAFTINGKIT, TEXT("����� ���� ŰƮ"), TEXT("��� ��, ����� 1�� ȹ��"), false,  ITEMTYPE::ITEM_TYPE_ETC, false, 7800.f, 5600, 2340.f,34.f , 92.f};
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MachinGun_CraftingKit"),
		TEXT("Com_Texture_17"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_MACHINGUN_CRAFTINGKIT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_MACHINGUN_CRAFTINGKIT)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_MAFIAHAT)] =
	{ ITEMINDEX::ITEM_MAFIAHAT, TEXT("���Ǿ� ����"), TEXT("����� ������"), false,  ITEMTYPE::ITEM_TYPE_HAT, false, 1900.f, 3400, 3300.f, 45.f , 90.f};
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MafiaHat_steam_small"),
		TEXT("Com_Texture_18"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_MAFIAHAT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_MAFIAHAT)].resize(_int(ITEMTAG::ITEM_TAG_END));


	m_vecItemInfo[_int(ITEMINDEX::ITEM_MEAT)] =
	{ ITEMINDEX::ITEM_MEAT, TEXT("�����"), TEXT("���� �� ������ ������ �����ϰų� �����ϼ���"), false,  ITEMTYPE::ITEM_TYPE_FOOD, false, 1550.f, 4100, 350.f,53.f , 55.f};
	if (FAILED(__super::Add_Component(_int(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconAnyMeat"),
		TEXT("Com_Texture_19"), reinterpret_cast<CComponent**>(&m_vecItemInvenTexture[_int(ITEMINDEX::ITEM_MEAT)]))))
		return E_FAIL;
	m_vecItemTag[_int(ITEMINDEX::ITEM_MEAT)].resize(_int(ITEMTAG::ITEM_TAG_END));
	m_vecItemTag[_int(ITEMINDEX::ITEM_MEAT)][_int(ITEMTAG::ITEM_TAG_CANROTTEN)] = true;


	m_vecTypeName.resize(_int(ITEMTYPE::ITEM_TYPE_END));
	m_vecTagName.resize(_int(ITEMTAG::ITEM_TAG_END));

	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_WEAPON)] = TEXT("�ֹ���/��������");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_AMMO)] = TEXT("ź��/ȭ��");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_ETC)] = TEXT("��Ÿ");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_FOOD)] = TEXT("����");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_HAT)] = TEXT("����");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_MINERAL)] = TEXT("���");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_POTION)] = TEXT("����");
	m_vecTypeName[_int(ITEMTYPE::ITEM_TYPE_TOOL)] = TEXT("����");
	
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_BURN)] = TEXT("ȭ��");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_CANROTTEN)] = TEXT("��������");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_DISH)] = TEXT("�丮");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_HEAVYDEMAGE)] = TEXT("������ ����");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_HIGHPRICE)] = TEXT("����ǰ");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_PUSH)] = TEXT("�˹�");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_QUIET)] = TEXT("������");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_REFINED)] = TEXT("������ ���");
	m_vecTagName[_int(ITEMTAG::ITEM_TAG_ROTTEN)] = TEXT("������");
	
	
	// �κ� �ʱ� ���� 
	for (_int i = 0; i < _int(ITEMINDEX::ITEM_END); ++i)
		m_vecInvenInfo[i] = m_vecItemInfo[i];

	return S_OK;
}

const CItemManager::TINFO CItemManager::PickItem(ITEMARRAY eArray, _int iIndex)
{
	TINFO tFail;

	if ((eArray == ITEMARRAY::ARRAY_INFO) && (iIndex >= _int(ITEMINDEX::ITEM_END)))
		return tFail;
	if ((eArray == ITEMARRAY::ARRAY_INVEN) && (iIndex >= _int(INVEN_COL * INVEN_ROW)))
		return tFail;
	if ((eArray == ITEMARRAY::ARRAY_EQUIP) && (iIndex >= _int(EQUIPSLOT::SLOT_END)))
		return tFail;

	m_eBeforeArray = eArray;
	m_iBeforeIndex = iIndex;
	
	if (eArray == ITEMARRAY::ARRAY_INFO)
	{
		m_vecItemInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecItemInfo[iIndex];
		return m_vecItemInfo[iIndex];
	}
		
	if (eArray == ITEMARRAY::ARRAY_INVEN)
	{
		m_vecInvenInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecInvenInfo[iIndex];
		return m_vecInvenInfo[iIndex];
	}
		
	if (eArray == ITEMARRAY::ARRAY_EQUIP)
	{
		m_vecEquipInfo[iIndex].bPicked = true;
		m_tPickedItem = m_vecEquipInfo[iIndex];
		return m_vecEquipInfo[iIndex];
	}
		

	// ���� ���� ���� �߰� �ʿ� 
}

void CItemManager::CancelPick()
{
	if (m_tPickedItem.eIndex != ITEMINDEX::ITEM_END)
	{
		if (m_eBeforeArray == ITEMARRAY::ARRAY_INFO)
		{
			m_vecItemInfo[m_iBeforeIndex].bPicked = false;
		}

		if (m_eBeforeArray == ITEMARRAY::ARRAY_INVEN)
		{
			m_vecInvenInfo[m_iBeforeIndex].bPicked = false;
		}

		if (m_eBeforeArray == ITEMARRAY::ARRAY_EQUIP)
		{
			m_vecEquipInfo[m_iBeforeIndex].bPicked = false;
		}

		m_tPickedItem.eIndex = ITEMINDEX::ITEM_END;
		m_eBeforeArray = ITEMARRAY::ARRAY_END;
		m_iBeforeIndex = -1;
	}
}

HRESULT CItemManager::PutInItem(ITEMARRAY eArray, _int iIndex)
{
	if (m_tPickedItem.eIndex == ITEMINDEX::ITEM_END)
		return E_FAIL;

	if ((m_eBeforeArray == eArray) && (m_iBeforeIndex == iIndex))
	{
		CancelPick();
		return E_FAIL;
	}
		

	if (eArray == ITEMARRAY::ARRAY_INVEN)
	{
		_bool bPlus = false;
		if (m_tPickedItem.bStack)
		{
			for (auto& iter : m_vecInvenInfo)
				if (iter.eIndex == m_tPickedItem.eIndex)
				{
					iter.iCount += m_tPickedItem.iCount;
					CancelPick();
					bPlus = true;
					break;
				}
		}
			
		if ((!m_tPickedItem.bStack) || (!bPlus))
		{
			TINFO tTemp = m_tPickedItem;
			_int iBefore = m_iBeforeIndex;
			ITEMARRAY eBefore = m_eBeforeArray;
			PickItem(eArray, iIndex);

			if (eBefore == ITEMARRAY::ARRAY_INVEN)
				if (m_vecInvenInfo[iBefore].bPicked)
					m_vecInvenInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

			if (eBefore == ITEMARRAY::ARRAY_EQUIP)
				if (m_vecEquipInfo[iBefore].bPicked)
					m_vecEquipInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

			m_vecInvenInfo[iIndex] = tTemp;
			m_vecInvenInfo[iIndex].bPicked = false;
		}
	}
	if (eArray == ITEMARRAY::ARRAY_EQUIP)
	{
		if (!m_tPickedItem.bIsTool)
		{
			CancelPick();
			return E_FAIL;
		}
			

		TINFO tTemp = m_tPickedItem;
		_int iBefore = m_iBeforeIndex;
		ITEMARRAY eBefore = m_eBeforeArray;
		PickItem(eArray, iIndex);

		if (eBefore == ITEMARRAY::ARRAY_INVEN)
			if (m_vecInvenInfo[iBefore].bPicked)
				m_vecInvenInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;

		if (eBefore == ITEMARRAY::ARRAY_EQUIP)
			if (m_vecEquipInfo[iBefore].bPicked)
				m_vecEquipInfo[iBefore].eIndex = ITEMINDEX::ITEM_END;
		m_vecEquipInfo[iIndex] = tTemp;
		m_vecEquipInfo[iIndex].bPicked = false;
	}


	return S_OK;
}

CItemManager* CItemManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItemManager* pInstance = new CItemManager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItemManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemManager::Clone(void* pArg)
{
	CItemManager* pInstance = new CItemManager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItemManager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItemManager::Free()
{
	__super::Free();

	m_vecItemInfo.clear();
	m_vecInvenInfo.clear();

	m_vecInvenSelectBuffer.clear();
	m_vecEquipInfo.clear();

	m_vecTool.clear();

	for (auto& iter : m_vecItemInvenTexture)
		Safe_Release(iter);
	m_vecItemInvenTexture.clear();

	for (auto& iter : m_Render_Model_list)
	{
		Safe_Delete(iter.pParentState);
		Safe_Delete(iter.pSocketMatrix);
	}
		
	m_Render_Model_list.clear();
	
	for (auto& iter : m_vecItemTag)
		iter.clear();

	m_vecItemTag.clear();
	m_vecTypeName.clear();
	m_vecTagName.clear();
}
