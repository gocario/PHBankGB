#pragma once

#define TEXT_COUNT (14)
#define TEXT_LENGTH (7+1)

#include "font.h"

typedef enum
{
	TEXT__,
	TEXT_OT,
	TEXT_IV,
	TEXT_EV,
	TEXT_HP,
	TEXT_ATTACK,
	TEXT_DEFENSE,
	TEXT_SPEED,
	TEXT_SPECIAL,
	TEXT_STATUS,
	TEXT_TYPE1,
	TEXT_TYPE2,
	TEXT_MOVE,
	TEXT_PP,
} DATA_Text;

/**
 *
 */
const char8_t* dataText(DATA_Text text);

