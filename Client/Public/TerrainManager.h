#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "SurFace.h"
#include "Monster.h"
#include "Sky.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CCollider;
END

BEGIN(Client)
class CTerrainManager;

class CTerrainManager final : public CGameObject
{
public:
	

	enum class LANDCOMMAND
	{
		LCOMMAND_REMOVE_LAND = 0,
		LCOMMAND_ADD_LAND = 1,
		LCOMMAND_CHANGE_TEXTURE = 2,
		LCOMMAND_END
	};

	typedef struct LANDCUBE
	{
		_bool m_bLand = false;
		_int m_iTextureNum[int(LCUBEDIRECION::LDIREC_END)] = { 0 , };
	}LCUBE;

	typedef struct COMMAND
	{
		LANDCOMMAND m_eType;
		_int m_vIndex[3];
		_int m_iTextureNum[int(LCUBEDIRECION::LDIREC_END)] = { 0 , };
	}LCOMMAND;

	typedef struct SURFACE
	{
		_float4x4 m_vMat; // <- Surface�� ��ġ
		_int m_iTextureNum = int(LCUBEDIRECION::LDIREC_END);
	};

	typedef struct MAKEOBJ
	{
		MAKEOBJ(CONTAINER eType, _float3 Position, _float Rotate = 0.f, _int Index = 0) { eCon_Type = eType; fPosition = Position; fRotate = Rotate; iIndex = Index; }

		MAKEOBJ(INTERACTION eType, _float3 Position, _float Rotate = 0.f, _int Index = 0) { eInter_Type = eType; fPosition = Position; fRotate = Rotate;  iIndex = Index; }

		CONTAINER eCon_Type = CONTAINER::CONTAINER_END;
		INTERACTION eInter_Type = INTERACTION::INTER_END;
		_float3 fPosition = { 0.f,0.f,0.f };
		_float fRotate = 0.f;
		_int iIndex = 0;
	}MOBJ;

private:
	CTerrainManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrainManager(const CTerrainManager& Prototype);
	virtual ~CTerrainManager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT RenderSurface(SURFACE* pSurface);

public:
	void SaveMap(const _char* pPath);
	void LoadMap(const _char* pPath);

	void IndexMarkHeight(_int iHeight) { m_iIndexY = iHeight; }

	_float3 CheckPicking(_int iMode, _int iCX = -1, _int iCY = -1, _int iCZ = -1, _bool bTop = false, CONTAINER eType = CONTAINER::CONTAINER_END, INTERACTION eInter = INTERACTION::INTER_END, _int iRotate = 0, _int iIndex = 0);

	_float3 CheckPicking();

	_bool CheckPicking(_float3 fLook);

	void SetBedRock(_int iX, _int iY, _int iZ);
	void HighLight_Surface(_bool bLinked); // <- bLinked�� true�� ��� �Ѳ����� ĥ�� ��, ������ ����Ǵ� ǥ���� ǥ�� 
	void HighLight_NewCube(_float3 fSize);
	void HighLight_DeleteCube(_float3 fSize);
	void Make_NewCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ, _int TextureIndex);
	void Make_DeleteCube(_int iX, _int iY, _int iZ, _int iCX, _int iCY, _int iCZ);
	void Change_Surface(_bool bLinked); // <- bLinked�� true�� ��� �ؽ��� ��ȣ�� ���� ���� ǥ������ ����� ��� ǥ���� �ؽ��ĸ� ��ü 
	
	void Destroy_Terrain_Explosion(_float3 fPosition, _float fRadius);

	void Set_Object();
	void Delete_Object();

	void MakeTestMap();

	void ChangeIndex(_int iIndex) { m_iTextureIndex = iIndex; }

	// �� ���� 
	void Turn_Model(XMVECTOR fAxis, _float fTimeDelta) 
	{
		
	}
	void Scaling_Model(_float Scale) 
	{ 
		
	}
	void Change_Animation(_int iInput)
	{
		
	}

	void Set_Render_Length(_float fLength) { m_fRender_Length = fLength; }

#pragma region COLLISION
	_float3 Check_Terrain_Collision_Adjust(_float3 fCenter, _float3 fExtents, _float3 vAdjustVector, LCUBEDIRECION* eDirec);
	_bool Check_OnGround(_float3 fCenter, _float3 fExtents);
	_bool Check_Wall(_float3 fCenter, _float3 fLook, _float fRange); 
	_bool Check_Terrain_Collision(_float3 fCenter, _float3 fExtents); // <- �ܼ��� �浹�� ����
	_bool Check_IsTerrain(_float3 fPosition) 
	{
		_int iX = _int(fPosition.x / LCUBESIZE);
		_int iY = _int(fPosition.y / LCUBESIZE);
		_int iZ = _int(fPosition.z / LCUBESIZE);

		Adjust_Index(&iX, &iY, &iZ);

		return m_vecLcubeInfo[iX][iY][iZ].m_bLand;}


#pragma endregion

private:
	HRESULT Ready_Components();
	void ChangeLandInfo_Normal(); // <- commandbuffer�� ���� ���뿡 ���� ���� ���� ���� (Late_Update���� ����)
	void ChangeLandInfo_Backward(); // <- �ڷΰ��� ����

	_wstring MakeKey(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec);
	void InterpretKey(_wstring Key, _int* iX, _int* iY, _int* iZ, LCUBEDIRECION* eDirec);

	void SetPosition_Surface(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec, _float4x4* fSurface);
	_float3 IsPicking_Instancing(SURFACE* pSurface);

	void Adjust_Index(_int* iX, _int* iY, _int* iZ);

	void Delete_LastObject();

	void ShowIndex(_int iX, _int iZ, _matrix mHost);

private:
	
	// ����
	_int m_iBedRock = 2.f; //<- �ΰ��� �÷��̿��� Y �� �������� �ش� �ε��� �̸��� ���� �ı� �Ұ� 

	_bool m_bModifyLand = true;
	_float m_fInterval_Now = 0.0f;
	_float m_fInterval = 0.2f;

	//test
	_float fTest = 0.f;
	_int iTest = 0;

	// ���� ����
	list<LCOMMAND> m_CommandBuffer; // <- ��� ������ ���� �ʿ� �����Ű�� ���� ���� 
	list<list<LCOMMAND>> m_CommandStack; // <- �ڷΰ����
	_bool m_bBackward = false; // <- �ڷΰ��� ���� ���� Ȯ�� 
	
	// �� ���� ���� 
	vector<vector<vector<LCUBE>>> m_vecLcubeInfo; // <- ���� ��� ���� ���� 
	map<_wstring, CSurFace*> m_mapSurFace; // <- ǥ�� ������ ��ü 

	// ǥ�� ���� �ν��Ͻ�
	map<_wstring, SURFACE*> m_mapInstancing_SurFace;

	// ��ü ���� ���� 
	list<MAKEOBJ> m_vecObjInfo;


	// texture
	_int m_iTextureIndex = 0;

	// ���� ���� ���� ���� 
	_float m_fRender_Length = -1.f;

	_float m_fRender_Index = 50.f;
	_int m_iIndexY = 5; // <- �ε��� ǥ���ϴ� ���� (������ ����) 




public:
	class CShader* m_pShaderCom = { nullptr };
	class CShader* m_pShaderCom_Gray = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };
	class CSky* m_pSky = { nullptr };

public:
	static CTerrainManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END