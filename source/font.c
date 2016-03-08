#include "font.h"
#include "save.h"
#include "pkdir.h"

#include <sf2d.h>
#include <sfil.h>

#define CHAR_SIZE (8)

///
enum
{
	C_NULL = 0x00,			///< \0
	C_PKMN = 0x4A,			///< PKMN
	C_LINE_BREAK = 0x4F,	///< \n
	C_TERMINATOR = 0x50,	///< \0
	C_PLAYER_NAME = 0x52,	///< NINTEN
	C_RIVAL_NAME = 0x53,	///< SONY
	C_POKE = 0x54,			///< POKé
	C_ELLIPSIS = 0x56,		///< …
	C_INACTIVE_PKM = 0x59,	///< Enemy
	C_ACTIVE_PKM = 0x54,	///< Friend
	C_PC = 0x5B,			///< PC
	C_TM = 0x5C,			///< TM
	C_TRAINER = 0x5D,		///< TRAINER
	C_ROCKET = 0x5E,		///< ROCKET
} FONT_ControlChar;

static sf2d_texture* fontGB;

Result fontLoad(void)
{
	fontGB = sfil_load_PNG_file(ROMFS "font_gb.png", SF2D_PLACE_RAM);

	return (fontGB ? 0 : -5);
}

void fontFree(void)
{
	sf2d_free_texture(fontGB);
}

/**
 * @brief Converts a char character to a char8_t character.
 * @param c The char character.
 * @return The char8_t character.
 */
static char8_t fontConvertChar(char c)
{
	switch (c)
	{
		case 'A' ... 'Z': return 0x80 + (c - 'A');
		case 'a' ... 'z': return 0xA0 + (c - 'a');
		case '0' ... '9': return 0xF6 + (c - '0');
		case ' ': return 0x7F;
		default: return 0x50;
	}
}

void fontConvertString(char8_t* str8, const char* str)
{
	while (*str != 0x00 && *str != 0x50)
	{
		*str8 = fontConvertChar(*str);
		str8++;
		str++;
	}
}

int8_t fontDrawChar8(int16_t x, int16_t y, char8_t c)
{
	// Do not draw the control characters.
	// if (c >= 0x48 && c <= 0x5F) return 0;

	sf2d_draw_texture_part(fontGB, x, y, (c % 0x10) * CHAR_SIZE, (c / 0x10) * CHAR_SIZE, CHAR_SIZE, CHAR_SIZE);

	return CHAR_SIZE;
}

/**
 * @brief Handles a char8_t control character.
 * @param o_x The x original position.
 * @param o_y The y original position.
 * @param[in] x The x position.
 * @param[in] y The y position.
 * @param c The char8_t character.
 */
static void fontDrawControlChar8(int16_t o_x, int16_t o_y, int16_t* x, int16_t* y, char8_t c)
{
	switch (c)
	{
		case C_PKMN:
			*x += fontDrawChar8(*x, *y, 0xE1); // PK
			*x += fontDrawChar8(*x, *y, 0xE2); // MN
			break;
		case C_LINE_BREAK:
			*x = o_x;
			*y += CHAR_SIZE;
			break;
		case C_PLAYER_NAME:
			*x += fontDrawString8(*x, *y, save + 0x2598);
			break;
		case C_RIVAL_NAME:
			*x += fontDrawString8(*x, *y, save + 0x25F6);
			break;
		case C_POKE:
			*x += fontDrawChar8(*x, *y, 0x8F); // P
			*x += fontDrawChar8(*x, *y, 0x8E); // O
			*x += fontDrawChar8(*x, *y, 0x8A); // K
			*x += fontDrawChar8(*x, *y, 0xBA); // é
			break;
		case C_ELLIPSIS:
			*x += fontDrawChar8(*x, *y, 0x75); // …
			break;
		case C_PC:
			*x += fontDrawChar8(*x, *y, 0x8F); // P
			*x += fontDrawChar8(*x, *y, 0x82); // C
			break;
		case C_TM:
			*x += fontDrawChar8(*x, *y, 0x82); // T
			*x += fontDrawChar8(*x, *y, 0x8C); // M
			break;
		case C_TRAINER:
			*x += fontDrawChar8(*x, *y, 0x93); // T
			*x += fontDrawChar8(*x, *y, 0x91); // R
			*x += fontDrawChar8(*x, *y, 0x80); // A
			*x += fontDrawChar8(*x, *y, 0x88); // I
			*x += fontDrawChar8(*x, *y, 0x8D); // N
			*x += fontDrawChar8(*x, *y, 0x84); // E
			*x += fontDrawChar8(*x, *y, 0x91); // R
			break;
		case C_ROCKET:
			*x += fontDrawChar8(*x, *y, 0x91); // R
			*x += fontDrawChar8(*x, *y, 0x8E); // O
			*x += fontDrawChar8(*x, *y, 0x82); // C
			*x += fontDrawChar8(*x, *y, 0x8A); // K
			*x += fontDrawChar8(*x, *y, 0x84); // E
			*x += fontDrawChar8(*x, *y, 0x93); // T
			break;
		default: return;
	}
}

int16_t fontDrawString8(int16_t x, int16_t y, const char8_t* str)
{
	int16_t o_x = x, o_y = y;
	while (*str != 0x00 && *str != 0x50)
	{
		// If it is a control char
		if (*str >= 0x48 && *str <= 0x5F)
		{
			fontDrawControlChar8(o_x, o_y, &x, &y, *str);
		}
		// If it is a print/junk char
		else
		{
			x += fontDrawChar8(x, y, *str);
		}
		str++;
	}
	return x - o_x;
}
