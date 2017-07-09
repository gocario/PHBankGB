#include "gfx.h"
#include "font.h"
#include "save.h"
#include "pkdir.h"

#include <sfil.h>

#define PKM_PER_ROW (8)
#define ROMFS ROMFS_FOLDER

static sf2d_texture* pkmIconsGBC;
static sf2d_texture* topFrame;

Result gfxLoad(void)
{
	pkmIconsGBC = sfil_load_PNG_file(ROMFS "pokemon_icons_gbc.png", SF2D_PLACE_RAM);

	return (pkmIconsGBC ? 0 : -5);
}

Result gfxLoadFrame(uint64_t titleid)
{
	switch (saveGetGameVersion(titleid))
	{
		case POKEMON_RED: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_red.png", SF2D_PLACE_RAM); break;
		case POKEMON_GREEN: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_green.png", SF2D_PLACE_RAM); break;
		case POKEMON_BLUE: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_blue.png", SF2D_PLACE_RAM); break;
		case POKEMON_YELLOW: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_yellow.png", SF2D_PLACE_RAM); break;
		case POKEMON_CRYSTAL: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_crystal.png", SF2D_PLACE_RAM); break;
		default: topFrame = NULL; break;
	}

	return (topFrame ? 0 : -5);
}

void gfxFree(void)
{
	sf2d_free_texture(pkmIconsGBC);
}

void gfxFreeFrame(void)
{
	sf2d_free_texture(topFrame);
}

void gfxDrawPokemonIcon(int16_t x, int16_t y, DEX_Species icon, GFX_Frame frame)
{
	if (icon > SPECIES_MEW) icon = SPECIES_MISSINGNO;

	sf2d_draw_texture_part(pkmIconsGBC, x, y, 16 * (frame + (icon % PKM_PER_ROW) * 2), 16 * (icon / PKM_PER_ROW), 16, 16);
}

void gfxDrawFrame(int16_t x, int16_t y)
{
	sf2d_draw_texture(topFrame, x, y);
}

void gfxDrawPanel(int16_t x, int16_t y, uint8_t w, uint8_t h)
{
	// Top left
	fontDrawChar8(x, y, 0x79);

	// Top right
	fontDrawChar8(x + (w + 1) * 8, y, 0x7B);

	// Bottom left
	fontDrawChar8(x, y + (h + 1) * 8, 0x7D);

	// Bottom right
	fontDrawChar8(x + (w + 1) * 8, y + (h + 1) * 8, 0x7E);

	// Horizontal
	for (uint8_t xw = 0; xw < w; xw++)
	{
		fontDrawChar8(x + (xw + 1) * 8, y, 0x7A);
		fontDrawChar8(x + (xw + 1) * 8, y + (h + 1) * 8, 0x7A);
	}

	// Vertical
	for (uint8_t yh = 0; yh < h; yh++)
	{
		fontDrawChar8(x, y + (yh + 1) * 8, 0x7C);
		fontDrawChar8(x + (w + 1) * 8, y + (yh + 1) * 8, 0x7C);
	}
}

void gfxDrawFolio(int16_t x, int16_t y, uint8_t w, uint8_t h)
{
	// Bottom left
	fontDrawChar8(x, y + h * 8, 0xDC);

	// Bottom right
	fontDrawChar8(x + (w + 1) * 8, y + h * 8, 0xDE);

	// Horizontal
	for (uint8_t xw = 0; xw < w; xw++)
	{
		fontDrawChar8(x + (xw + 1) * 8, y + h * 8, 0xDD);
	}

	// Vertical
	for (uint8_t yh = 0; yh < h; yh++)
	{
		fontDrawChar8(x + (w + 1) * 8, y + yh * 8, 0xDF);
	}
}

void gfxDrawLevel(int16_t x, int16_t y, uint8_t level)
{
	x += fontDrawChar8(x, y, CHAR8_LVL);
	x += fontDrawUInt8(x, y, level);
}

void gfxDrawLifeBar(int16_t x, int16_t y, uint16_t currentHP, uint16_t maxHP, bool onlyBar)
{
	x += fontDrawSubchar8(x, y, CHAR8_HP, SUBCHAR_TOP_LEFT) + 1;
	x += fontDrawSubchar8(x, y, CHAR8_HP, SUBCHAR_TOP_RIGHT) + 1;
	x += fontDrawSubchar8(x, y, CHAR8_HP, SUBCHAR_BOTTOM_LEFT);

	char8_t c8;
	uint8_t percent = (float) currentHP / (float) maxHP * 100 + 0.5f;
	if (percent == 0) c8 = CHAR8_HPBAR_WHITE;
	else if (percent < 21) c8 = CHAR8_HPBAR_RED;
	else if (percent < 57) c8 = CHAR8_HPBAR_ORANGE;
	else c8 = CHAR8_HPBAR_GREEN;

	x += fontDrawSubchar8(x, y, c8, SUBCHAR_TOP_LEFT);
	for (uint8_t i = 0; i < 11; i++)
	{
		if (i * 100 / 11 > percent)
			c8 = CHAR8_HPBAR_WHITE;
		x += fontDrawSubchar8(x, y, c8, SUBCHAR_BOTTOM_LEFT);
	}
	x += fontDrawSubchar8(x, y, c8, SUBCHAR_TOP_RIGHT);

	if (!onlyBar)
	{
		fontDrawFromRightUInt32(x - 8, y + 6, maxHP);
		fontDrawChar8(x - 32, y + 6, CHAR8_SLASH);
		fontDrawFromRightUInt32(x - 40, y + 6, currentHP);
	}
}

void gfxDrawEmptyArrow(int16_t x, int16_t y)
{
	fontDrawChar8(x, y, 0xEC);
}

void gfxDrawFullArrow(int16_t x, int16_t y)
{
	fontDrawChar8(x, y, 0xED);
}
