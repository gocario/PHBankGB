#include "font.h"
#include "save.h"

#include <sf2d.h>

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
};

extern const struct {
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel;
	unsigned char pixel_data[];
} fontGB_img;

static sf2d_texture* fontGB;

void fontInit(void)
{
	fontGB = sf2d_create_texture_mem_RGBA8(fontGB_img.pixel_data, fontGB_img.width, fontGB_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void fontExit(void)
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

int16_t fontDrawString8(int16_t x, int16_t y, const char8_t* str)
{
	int16_t o_x = x, o_y = y;
	while (*str != 0x00 && *str != 0x50)
	{
		// If it is a print/junk char
		{
			x += fontDrawChar8(x, y, *str);
		}
		str++;
	}
	return x - o_x;
}
