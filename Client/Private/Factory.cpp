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
	if (FAILED(Ready_Prototype_Shader()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Buffer()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Font()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;



	if (FAILED(Ready_Prototype_Model()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Part()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_Container()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SurFace"),
		CSurFace::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(Ready_Prototype_UIPart()))
		return E_FAIL;
	if (FAILED(Ready_Prototype_UIPage()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_InterAction()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Effect()))
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

void CFactory::Make_Container_Player(_float3 Position, _float fAngle)
{
	CGameObject::GAMEOBJECT_DESC		pTemp{};
	pTemp.fPosition = Position;
	pTemp.fSpeedPerSec = 5.0f;
	pTemp.fRotationPerSec = XMConvertToRadians(180.0f);
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), TEXT("Prototype_GameObject_Container_Player"), &pTemp);
	static_cast<CLongvinter_Container*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Player"), -1))->Rotation({ 0.f,1.f,0.f }, fAngle);

}

void CFactory::Make_Container_Enemy(_float3 Position, ENEMY_TYPE eType, _float fAngle)
{
	CAI_Enemy::AI_Enemy_Desc		pTemp{};
	pTemp.fPosition = Position;
	pTemp.fSpeedPerSec = 3.0f;
	pTemp.fRotationPerSec = XMConvertToRadians(180.0f);
	pTemp.eType = eType;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), TEXT("Prototype_GameObject_Container_AI_Enemy"), &pTemp);
	static_cast<CLongvinter_Container*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Enemy"), -1))->Rotation({ 0.f,1.f,0.f }, fAngle);
}

void CFactory::Make_Container_NPC(_float3 Position, NPC_TYPE eType, _float fAngle)
{
	CAI_NPC::AI_NPC_Desc		pTemp{};
	pTemp.fPosition = Position;
	pTemp.fSpeedPerSec = 3.0f;
	pTemp.fRotationPerSec = XMConvertToRadians(180.0f);
	pTemp.eType = eType;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_NPC"), TEXT("Prototype_GameObject_Container_AI_NPC"), &pTemp);
	static_cast<CLongvinter_Container*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_NPC"), -1))->Rotation({ 0.f,1.f,0.f }, fAngle);
}

void CFactory::Make_Container_Turret(_float3 Position, _float fAngle)
{
	CGameObject::GAMEOBJECT_DESC		pTemp{};
	pTemp.fPosition = Position;
	pTemp.fSpeedPerSec = 3.0f;
	pTemp.fRotationPerSec = XMConvertToRadians(180.0f);
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Turret"), TEXT("Prototype_GameObject_Container_Turret"), &pTemp);
	static_cast<CLongvinter_Container*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Container_Turret"), -1))->Rotation({ 0.f,1.f,0.f }, fAngle);
}

CPartObject* CFactory::Make_Tool_Head(_int iHeadIndex, _uint* pParentState, const _float4x4* pMatrix, const _float4x4* pParent)
{
	CTool_Head::HEAD_DESC ToolDesc{};
	ToolDesc.eType = CTool_Head::HEAD_TYPE(iHeadIndex);
	ToolDesc.pParentState = pParentState;
	ToolDesc.pParentWorldMatrix = pMatrix;
	ToolDesc.pSocketBoneMatrix = pParent;

	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_Head"), TEXT("Prototype_GameObject_Tool_Head"), &ToolDesc);
	return static_cast<CPartObject*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_Tool_Head"), -1));
}

HRESULT CFactory::Ready_Prototype_Texture()
{

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_LandTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/LandTexture/texture%d.dds"), LTEXTUREMAX))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_BGA"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Picture/BGA%d.dds"), 6))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Picture/T_IconCash.dds"), 1))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Button_Close"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Button/CloseIcon.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cell_InvenEmpty"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Cell/InvenCell_Empty.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Symbol_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Symbol/T_Rounded32.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconAutoShotgun.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Ammo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconAmmo.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_ShotGun_Kit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/AutoShotgun_CraftingKit.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Cursor/Cursor%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_SkyBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/SkyBox/SkyBox0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Crosshair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Cursor/Crosshair.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Check"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Button/T_SettingsCheckTrue.dds"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Christmas_Hat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Christmas_hat.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Coal_ingot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Coal_ingot.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Coal_ore_chunk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Coal_ore_chunk.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Copper_ingot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Copper_ingot.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Copper_ore_chunk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Copper_ore_chunk.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Gold_ingot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Gold_ingot.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Gold_ore_chunk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Gold_ore_chunk.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Iron_ingot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Iron_ingot.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Iron_ore_chunk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/Iron_ore_chunk.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MachinGun_CraftingKit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/MachinGun_CraftingKit.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_MafiaHat_steam_small"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/MafiaHat_steam_small.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconAnyFish"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconAnyFish.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconAnyMeat"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconAnyMeat.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconChainsaw"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconChainsaw.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconCloudberryJam"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconCloudberryJam.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconArrows10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconArrows10.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconShotgun"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconShotgun.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconGrenade"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconGrenade.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconFireThrower"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconSemiRifle.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconMachineGun"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconMachineGun.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconModernRifle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconModernRifle.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconCrossbow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconCrossbow.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconMine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconMine.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_T_IconMachete"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Item/Item_2DTexture/T_IconMachete.dds"), 1))))
		return E_FAIL;

	
	
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Body_Red.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Body_Green.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Body_Blue.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Yellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Body_Yellow.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Body_Brown"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Body_Brown.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Mad"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Mad.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Boring"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Boring.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Normal.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Sleep"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Sleep.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Psycho"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Psycho.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Texture_Human_Face_Sad"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Human/Human_Face_Sad.dds"), 1))))
		return E_FAIL;



	

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Font()
{
	m_pGameInstance->Add_Font(TEXT("Font_Test1"), TEXT("../Bin/Resources/Fonts/140.spritefont"));
	m_pGameInstance->Add_Font(TEXT("Font_Test2"), TEXT("../Bin/Resources/Fonts/142.spritefont"));
	m_pGameInstance->Add_Font(TEXT("Font_Test3"), TEXT("../Bin/Resources/Fonts/142ex.spritefont"));
	m_pGameInstance->Add_Font(TEXT("Font_Test4"), TEXT("../Bin/Resources/Fonts/143.spritefont"));
	m_pGameInstance->Add_Font(TEXT("Font_Test5"), TEXT("../Bin/Resources/Fonts/143ex.spritefont"));

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Part()
{
	/* For. Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Body_Human"),
		CBody_Human::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Body_Turret"),
		CBody_Turret::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Empty"),
		CTool_Empty::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_ShotGun"),
		CTool_ShotGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Granade"),
		CTool_Granade::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_ChainSaw"),
		CTool_ChainSaw::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_MachineGun"),
		CTool_MachineGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_LandMine"),
		CTool_LandMine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_FireThrower"),
		CTool_FireThrower::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Bow"),
		CTool_Bow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Machete"),
		CTool_Machete::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tool_Head"),
		CTool_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Container()
{
	/* For. Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Container_Player"),
		CContainer_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Container_AI_Enemy"),
		CAI_Enemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Container_AI_NPC"),
		CAI_NPC::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Container_Turret"),
		CContainer_Turret::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Model()
{
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/* For. Prototype_Component_Model_Player*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_ANIM, true, TEXT("Prototype_Component_Model_Player"), "../Bin/Resources/Models/Player/Player", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Human*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_ANIM, true, TEXT("Prototype_Component_Model_Human"), "../Bin/Resources/Models/Human/Human1", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Shotgun*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Shotgun"), "../Bin/Resources/Models/Shotgun/Shotgun_New", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bullet*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Bullet_Normal"), "../Bin/Resources/Particle/Bullet_Normal", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bullet*/
	PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Explosion"), "../Bin/Resources/Particle/Explosion", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bullet*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Throw_LandMine"), "../Bin/Resources/Throw/LandMine", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bullet*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Throw_Granade_Pinned"), "../Bin/Resources/Throw/Granade_Pinned", PreTransformMatrix)))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bullet*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Throw_Granade_Unpinned"), "../Bin/Resources/Throw/Granade_Unpinned", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_ChainSaw"), "../Bin/Resources/Models/ChainSaw/ChainSaw", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_CrossBow"), "../Bin/Resources/Models/CrossBow/CrossBow", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_FireThrower"), "../Bin/Resources/Models/FireThrower/FireThrower", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_MachineGun"), "../Bin/Resources/Models/MachineGun/HeavyMachineGun", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Machete"), "../Bin/Resources/Models/Machete/Machete", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Turret_Base"), "../Bin/Resources/Models/Turret/Turret_Base", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Turret_Gun"), "../Bin/Resources/Models/Turret/Turret_Gun", PreTransformMatrix)))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Turret_Control"), "../Bin/Resources/Models/Turret/Turret_Control", PreTransformMatrix)))
		return E_FAIL;

	// LANDOBJECT
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_AppleTree"), "../Bin/Resources/Models/LandObject/AppleTree", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Bush_1"), "../Bin/Resources/Models/LandObject/Bush_1", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Bush_2"), "../Bin/Resources/Models/LandObject/Bush_2", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Bush_3"), "../Bin/Resources/Models/LandObject/Bush_3", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_ItemPocket"), "../Bin/Resources/Models/LandObject/ItemPocket", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_1"), "../Bin/Resources/Models/LandObject/Rock_1", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_2"), "../Bin/Resources/Models/LandObject/Rock_2", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_3"), "../Bin/Resources/Models/LandObject/Rock_3", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_4"), "../Bin/Resources/Models/LandObject/Rock_4", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_5"), "../Bin/Resources/Models/LandObject/Rock_5", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_6"), "../Bin/Resources/Models/LandObject/Rock_6", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_7"), "../Bin/Resources/Models/LandObject/Rock_7", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, true, TEXT("Prototype_Component_Model_Rock_8"), "../Bin/Resources/Models/LandObject/Rock_8", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Tree_Fine"), "../Bin/Resources/Models/LandObject/Tree_Fine", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Tree_Normal"), "../Bin/Resources/Models/LandObject/Tree_Normal", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Door"), "../Bin/Resources/Models/LandObject/Door", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_MonsterMaker"), "../Bin/Resources/Models/LandObject/MonsterMaker", PreTransformMatrix)))
		return E_FAIL;

	// Head
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Normal_1"), "../Bin/Resources/Models/Hair/Hair_Normal", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Normal_2"), "../Bin/Resources/Models/Hair/Hair_Normal1", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Normal_3"), "../Bin/Resources/Models/Hair/Hair_Normal2", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Brown"), "../Bin/Resources/Models/Hair/Hair_Brown", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Yellow"), "../Bin/Resources/Models/Hair/Hair_Yellow", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Pink"), "../Bin/Resources/Models/Hair/Hair_Pink", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_HeadGear"), "../Bin/Resources/Models/Hair/Hair_HeadGear", PreTransformMatrix)))
		return E_FAIL;
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(Ready_Prototype_Model_Single(CModel::TYPE_NONANIM, false, TEXT("Prototype_Component_Model_Head_Blue"), "../Bin/Resources/Models/Hair/Hair_Blue", PreTransformMatrix)))
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

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_NonTexture.hlsl"), VTXMESH_NONTEX::Elements, VTXMESH_NONTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel_NonTexture_Alpah"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_NonTexture_Alpah.hlsl"), VTXMESH_NONTEX::Elements, VTXMESH_NONTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxSurFace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSurFace.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxSurFace_Gray"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSurFace_Gray.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECTINSTANCE::Elements, VTXRECTINSTANCE::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTXPOINTINSTANCE::Elements, VTXPOINTINSTANCE::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_Shader_VtxFireSmoke"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxFireSmoke.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
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

	/* For. Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
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

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Cell"), CUIPart_Cell::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_TextBox"), CUIPart_TextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Bar"), CUIPart_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPart_Symbol"), CUIPart_Symbol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_UIPage()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Main"), CUIPage_Main::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Test"), CUIPage_Test::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Inven"), CUIPage_Inven::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Equip"), CUIPage_Equip::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Crafting"), CUIPage_Crafting::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Option"), CUIPage_Option::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_ToolTip"), CUIPage_ToolTip::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_User"), CUIPage_User::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Shop"), CUIPage_Shop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Hack"), CUIPage_Hack::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_UIPage_Talk"), CUIPage_Talk::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFactory::Ready_Prototype_InterAction()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Bullet_Straight"), CBullet_Straight::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Bullet_MachineGun"), CBullet_MachineGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Throw_Mine"), CThrow_Mine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Throw_Granade"), CThrow_Granade::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Explosion_Normal"), CExplosion_Normal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Fire"), CFire::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Melee_ShotGun"), CMelee_ShotGun::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Melee_ChainSaw"), CMelee_ChainSaw::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_Explosion_Normal"), CExplosion_Normal::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	// LandObject

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_LandObject_NonAnim"), CLandObject_NonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Inter_LandObject_Anim"), CLandObject_Anim::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	


	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Effect()
{
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Particle_Fire"), CParticle_Fire::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Particle_Debris"), CParticle_Debris::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Particle_Trail"), CParticle_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Particle_Hit"), CParticle_Hit::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_Effect_Particle_Flame"), CParticle_Flame::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT CFactory::Ready_Prototype_Model_Single(CModel::TYPE eType, _bool bTexture, const _wstring strTag, const _char* pPath, _fmatrix PreTransformMatrix)
{
	if (eType == CModel::TYPE::TYPE_NONANIM)
	{
		string strType = ".fbx";
		string strTemp = {};

		_int i = 0;
		while (*(pPath + i) != '\0')
		{
			strTemp += *(pPath + i);
			++i;
		}

		string strPath = strTemp + strType;

		CModel* pNew = CModel::Create(m_pDevice, m_pContext, eType, bTexture, strPath.c_str(), PreTransformMatrix);
		m_ModelPrototypelist.push_back(pNew);
		if (FAILED(m_pGameInstance->Add_Prototype(_uint(LEVELID::LEVEL_STATIC), strTag, pNew)))
			return E_FAIL;

		return S_OK;
	}

	string strType = ".dat";
	string strTemp = {};

	_int i = 0;
	while (*(pPath + i) != '\0')
	{
		strTemp += *(pPath + i);
		++i;
	}

	string strPath = strTemp + strType;

	CModel* pNew = CModel::Create(m_pDevice, m_pContext, eType, bTexture, strPath.c_str(), PreTransformMatrix, true);
	if (pNew == nullptr)
	{
		strType = ".fbx";
		strPath = strTemp + strType;
		pNew = CModel::Create(m_pDevice, m_pContext, eType, bTexture, strPath.c_str(), PreTransformMatrix);

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

CUIPart_Cell* CFactory::MakeUIPart_Cell(CUIPart_Cell::UICELL_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CUIPart_Cell::UICell_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Cell"), TEXT("Prototype_UIPart_Cell"), &pTemp);

	return static_cast<CUIPart_Cell*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Cell"), -1));
}

CUIPart_Bar* CFactory::MakeUIPart_Bar(CUIPart_Bar::UIBAR_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, CLongvinter_Container* pHost)
{
	CUIPart_Bar::UIBar_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	pTemp.pHost = pHost;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Bar"), TEXT("Prototype_UIPart_Bar"), &pTemp);

	return static_cast<CUIPart_Bar*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Bar"), -1));
}

CUIPart_TextBox* CFactory::MakeUIPart_TextBox(CUIPart_TextBox::UITEXTBOX_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, _bool bCenter, _bool bAutoRemove, _float fShowTime, CLongvinter_Container* pHost)
{
	CUIPart_TextBox::UITextBox_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	pTemp.fTime = fShowTime;
	pTemp.AutoRemove = bAutoRemove;
	pTemp.bCenter = bCenter;
	pTemp.pHost = pHost;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_TextBox"), TEXT("Prototype_UIPart_TextBox"), &pTemp);

	return static_cast<CUIPart_TextBox*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_TextBox"), -1));
}

CUIPart_Symbol* CFactory::MakeUIPart_Symbol(CUIPart_Symbol::UISYMBOL_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY, CLongvinter_Container* pHost)
{
	CUIPart_Symbol::UISymbol_DESC		pTemp{};
	pTemp.eType = eType;
	pTemp.fX = fX;
	pTemp.fY = fY;
	pTemp.fSizeX = fSizeX;
	pTemp.fSizeY = fSizeY;
	pTemp.pHost = pHost;
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Symbol"), TEXT("Prototype_UIPart_Symbol"), &pTemp);

	return static_cast<CUIPart_Symbol*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPart_Symbol"), -1));
}

CUIPage_Main* CFactory::MakeUIPage_Main()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Main"), TEXT("Prototype_UIPage_Main"));

	return static_cast<CUIPage_Main*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Main"), -1));
}

CUIPage_Test* CFactory::MakeUIPage_Test()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Test"), TEXT("Prototype_UIPage_Test"));

	return static_cast<CUIPage_Test*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Test"), -1));
}

CUIPage_Inven* CFactory::MakeUIPage_Inven()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Inven"), TEXT("Prototype_UIPage_Inven"));

	return static_cast<CUIPage_Inven*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Inven"), -1));
}

CUIPage_Equip* CFactory::MakeUIPage_Equip()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Equip"), TEXT("Prototype_UIPage_Equip"));

	return static_cast<CUIPage_Equip*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Equip"), -1));
}

CUIPage_Crafting* CFactory::MakeUIPage_Crafting()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Crafting"), TEXT("Prototype_UIPage_Crafting"));

	return static_cast<CUIPage_Crafting*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Crafting"), -1));
}

CUIPage_Option* CFactory::MakeUIPage_Option()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Option"), TEXT("Prototype_UIPage_Option"));

	return static_cast<CUIPage_Option*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Option"), -1));
}

CUIPage_ToolTip* CFactory::MakeUIPage_ToolTip()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_ToolTip"), TEXT("Prototype_UIPage_ToolTip"));

	return static_cast<CUIPage_ToolTip*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_ToolTip"), -1));
}

CUIPage_User* CFactory::MakeUIPage_User()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_User"), TEXT("Prototype_UIPage_User"));

	return static_cast<CUIPage_User*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_User"), -1));
}

CUIPage_Shop* CFactory::MakeUIPage_Shop()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Shop"), TEXT("Prototype_UIPage_Shop"));

	return static_cast<CUIPage_Shop*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Shop"), -1));
}

CUIPage_Hack* CFactory::MakeUIPage_Hack()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Hack"), TEXT("Prototype_UIPage_Hack"));

	return static_cast<CUIPage_Hack*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Hack"), -1));
}

CUIPage_Talk* CFactory::MakeUIPage_Talk()
{
	m_pGameInstance->Add_CloneObject_ToLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Talk"), TEXT("Prototype_UIPage_Talk"));

	return static_cast<CUIPage_Talk*>(m_pGameInstance->Get_CloneObject_ByLayer(_uint(LEVELID::LEVEL_STATIC), TEXT("Layer_UIPage_Talk"), -1));
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
