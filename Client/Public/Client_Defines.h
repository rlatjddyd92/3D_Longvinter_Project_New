#pragma once

#include <process.h>
#include <iostream>

namespace Client
{
#include "../Default/Enum_Terrain.h"
#include "../Default/Factory_Enum_Part.h"
#include "../Default/Factory_Enum_Container.h"
#include "../Default/Enum_InterAction.h"
#include "../Default/Client_Struct.h"
#include "../Default/Enum_Item.h"
#include "../Default/Enum_Enemy.h"
#include "../Default/Enum_Human.h"

	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

#define GET_INSTANCE	CClientInstance::Get_Instance()
#define	VK_MAX			0xff
#define LCUBESIZE		1.f
#define LMAX_X			200
#define LMAX_Y			20
#define LMAX_Z			200
#define LTEXTUREMAX		8
#define MAKELKEY(X, Y, Z, D) (X * (6*LMAX_X * LMAX_Y * LMAX_Z)) + (Y * (6*LMAX_Y * LMAX_Z)) + (Z * (6*LMAX_Z)) + (D * 6)
#define TERRAINERROR	{-1.f,-1.f,-1.f}
#define GAPFROMTERRAIN	0.01f
#define GRAVITY_ACCELE  9.8f
#define INVEN_ROW		5
#define INVEN_COL		10
#define INVEN_CELLSIZE	50.f
#define INVEN_TEXTURE	45.f

	using namespace std;

	enum class LEVELID 
	{ 
		LEVEL_START,
		LEVEL_STATIC, 
		LEVEL_SINGLE,
		LEVEL_LOADING,
		LEVEL_LOGO, 
		LEVEL_GAMEPLAY, 
		LEVEL_EDITOR,
		LEVEL_END 
	};

	enum class CONTROLMODE
	{
		CMODE_MAIN,
		CMODE_EDITOR,
		CMODE_FIRST,
		CMODE_THIRD,
		CMODE_END
	};

	

}



extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
using namespace std;
