#include "data.h"

extern const char8_t text_text[TEXT_COUNT][TEXT_LENGTH];
extern const char8_t moves_text[MOVE_COUNT][MOVE_LENGTH];
extern const char8_t types_text[TYPE_COUNT][TYPE_LENGTH];
extern const char8_t status_text[STATUS_COUNT][STATUS_LENGTH];

/**
 *
 */
const char8_t* dataText(DATA_Text text)
{
	return text_text[text];
}

/**
 *
 */
const char8_t* dataMove(uint8_t move)
{
	return moves_text[move];
}

/**
 *
 */
const char8_t* dataType(uint8_t type)
{
	return types_text[type];
}

/**
 *
 */
const char8_t* dataStatus(uint8_t status)
{
	return status_text[status];
}
