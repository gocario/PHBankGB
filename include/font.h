#pragma once

#include <3ds/types.h>

///
typedef uint8_t char8_t;

///
typedef enum
{
	CHAR8_LINE_BREAK = 0x4F,
	CHAR8_DOT = 0xE8,
	CHAR8_SLASH = 0xF3,

	// Extra characters
	CHAR8_ID = 0xD0,
	CHAR8_NO = 0xD1,
	CHAR8_LVL = 0xD2,
	CHAR8_HP = 0xD3,
	CHAR8_HPBAR_WHITE = 0xD4,
	CHAR8_HPBAR_GREEN = 0xD5,
	CHAR8_HPBAR_ORANGE = 0xD6,
	CHAR8_HPBAR_RED = 0xD7,
	CHAR8_POKEBALL_SEEN = 0xD8,
} FONT_Char8;

/// 
typedef enum
{
	SUBCHAR_TOP_LEFT = 0,
	SUBCHAR_TOP_RIGHT = 1,
	SUBCHAR_BOTTOM_LEFT = 2,
	SUBCHAR_BOTTOM_RIGHT = 3,
} FONT_Subchar8;

/**
 * @brief Loads and initializes the font module.
 */
Result fontLoad(void);

/**
 * @brief Frees and exits the font module.
 */
void fontFree(void);

/**
 * @briefs Converts basically a char string into a char8_t string.
 * @param[out] str8 The char8_t string.
 * @param[in] str The char string.
 */
void fontConvertString(char8_t* str8, const char* str);

/**
 * @brief Draws a char8_t plain character.
 * @param x The x position.
 * @param y The y position.
 * @param c The char8_t character.
 * @return The size of the char.
 */
int8_t fontDrawChar8(int16_t x, int16_t y, char8_t c);

/**
 * @brief Draws a japan char8_t plain character.
 * @param x The x position.
 * @param y The y position.
 * @param c The japan char8_t character.
 * @return The size of the char.
 */
int8_t fontDrawJPChar8(int16_t x, int16_t y, char8_t c);

/**
 * @brief Draws a char8_t plain sub-character.
 * @param x The x position.
 * @param y The y position.
 * @param c The char8_t character.
 * @param i The index of the sub-character.
 * @return The size of the sub-character.
 */
int8_t fontDrawSubchar8(int16_t x, int16_t y, char8_t c, FONT_Subchar8 i);

/**
 * @brief Draws a char8_t string using the glyph sheet.
 * @param x The x position.
 * @param y The y position.
 * @param[in] str8 The char8_t string.
 * @return The width of the written string.
 */
uint16_t fontDrawString8(int16_t x, int16_t y, const char8_t* str8);
/**
 * @brief Draws a japan char8_t string using the glyph sheet.
 * @param x The x position.
 * @param y The y position.
 * @param[in] str8 The japan char8_t string.
 * @return The width of the written string.
 */
uint16_t fontDrawJPString8(int16_t x, int16_t y, const char8_t* str8);

/**
 * @brief Draws an uint8_t number using the glyph sheet.
 * @param x The x position.
 * @param y The y position.
 * @param v The uint8_t number.
 */
uint16_t fontDrawUInt8(int16_t x, int16_t y, int8_t v);

/**
 * @brief Draws an uint32_t number using the glyph sheet from the right.
 * @param x The x position.
 * @param y The y position.
 * @param v The uint32_t number.
 */
uint16_t fontDrawFromRightUInt32(int16_t x, int16_t y, uint32_t v);

/**
 * @brief Draws an uint8_t number using the glyph sheet with left padding zeros.
 * @param x The x position.
 * @param y The y position.
 * @param v The uint8_t number.
 */
uint16_t fontDrawPaddingUInt8(int16_t x, int16_t y, uint8_t v);

/**
 * @brief Draws an uint16_t number using the glyph sheet with left padding zeros.
 * @param x The x position.
 * @param y The y position.
 * @param v The uint16_t number.
 */
uint16_t fontDrawPaddingUInt16(int16_t x, int16_t y, uint16_t v);
