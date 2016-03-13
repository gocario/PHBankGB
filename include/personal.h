#pragma once

#include <3ds/types.h>

#include "pokedex.h"

/// 
typedef struct
{
	uint8_t dex;
	uint8_t HP, ATK, DEF, SPE, SPC;
} PersonalInfo;

/// 
typedef struct
{
	uint8_t id;
	uint8_t PP;
	uint8_t type;
} PersonalMoveInfo;

/**
 * @todo
 */
Result PersonalLoad(void);

/**
 * @todo
 */
const PersonalInfo* Personal(DEX_Species species);

/**
 * @todo
 */
const PersonalMoveInfo* PersonalMove(uint8_t move);
