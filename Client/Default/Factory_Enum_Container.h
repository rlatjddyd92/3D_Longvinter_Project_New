#pragma once

#include "Factory_Enum_Container_Animal.h"
#include "Factory_Enum_Container_Effect.h"
#include "Factory_Enum_Container_Enemy.h"
#include "Factory_Enum_Container_InterAction.h"
#include "Factory_Enum_Container_Item.h"
#include "Factory_Enum_Container_NPC.h"

enum class CONTAINER
{
	CONTAINER_PLAYER,
	CONTAINER_NPC,
	CONTAINER_ENEMY,
	CONTAINER_ANIMAL,
	CONTAINER_ITEM,
	CONTAINER_INTERACTION,
	CONTAINER_EFFECT,
	CONTAINER_END
};
