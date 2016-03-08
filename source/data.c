#include "data.h"

extern const char8_t text_text[TEXT_COUNT][TEXT_LENGTH];

/**
 *
 */
const char8_t* dataText(DATA_Text text)
{
	return text_text[text];
}

