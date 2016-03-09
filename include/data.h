#pragma once

#define TEXT_COUNT (14)
#define TEXT_LENGTH (7+1)

#define MOVE_COUNT (165)
#define MOVE_LENGTH (12+1)

#define TYPE_COUNT (15+12)
#define TYPE_LENGTH (8+1)

#define STATUS_COUNT (8)
#define STATUS_LENGTH (3+1)

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

/**
 *
 */
const char8_t* dataMove(uint8_t move);

/**
 *
 */
const char8_t* dataType(uint8_t type);

/**
 *
 */
const char8_t* dataStatus(uint8_t status);
