#pragma once

#include <3ds/types.h>

#include "pokedex.h"

/// 
typedef struct
{
	uint8_t HP, ATK, DEF, SPE, SPC;
	uint16_t BST;
} PersonalInfo;

/**
 * @todo
 */
Result PersonalLoad(void);

/**
 * @todo
 */
const PersonalInfo* Personal(DEX_Species species);
