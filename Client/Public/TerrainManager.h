#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "SurFace.h"
#include "Monster.h"

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
		MAKEOBJ(CONTAINER eType, _float3 Position) { eCon_Type = eType; fPosition = Position; }

		CONTAINER eCon_Type = CONTAINER::CONTAINER_END;
		_float3 fPosition = { 0.f,0.f,0.f };
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

public:
	void SaveMap(const _char* pPath);
	void LoadMap(const _char* pPath);

	_float3 CheckPicking(_int iMode, _int iCX = -1, _int iCY = -1, _int iCZ = -1, _bool bTop = false, CONTAINER eType = CONTAINER::CONTAINER_END);

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



#pragma endregion

private:
	HRESULT Ready_Components();
	void ChangeLandInfo_Normal(); // <- commandbuffer�� ���� ���뿡 ���� ���� ���� ���� (Late_Update���� ����)
	void ChangeLandInfo_Backward(); // <- �ڷΰ��� ����

	_wstring MakeKey(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec);
	void InterpretKey(_wstring Key, _int* iX, _int* iY, _int* iZ, LCUBEDIRECION* eDirec);

	void SetPosition_Surface(_int iX, _int iY, _int iZ, LCUBEDIRECION eDirec, _float4x4* fSurface);
	_float3 IsPicking_Instancing(SURFACE* pSurface);

private:
	


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

public:
	class CShader* m_pShaderCom = { nullptr };
	class CShader* m_pShaderCom_Gray = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	class CCollider* m_pColliderCom = { nullptr };

public:
	static CTerrainManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END