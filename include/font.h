#pragma once

#include <3ds/types.h>

#define CHAR8_LINE_BREAK (0x4F)

typedef uint8_t char8_t;

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
 * @brief Draws a char8_t string using the glyph sheet.
 * @param x The x position.
 * @param y The y position.
 * @param[in] str8 The char8_t string.
 * @return The width of the written string.
 */
uint16_t fontDrawString8(int16_t x, int16_t y, const char8_t* str8);
