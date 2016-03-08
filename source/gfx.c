#include "gfx.h"
#include "font.h"
#include "save.h"
#include "pkdir.h"

#include <sfil.h>

#define PKM_PER_ROW (8)

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
		case NOT_POKEMON:
		case POKEMON_RED: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_red.png", SF2D_PLACE_RAM); break;
		case POKEMON_GREEN: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_green.png", SF2D_PLACE_RAM); break;
		case POKEMON_BLUE: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_blue.png", SF2D_PLACE_RAM); break;
		case POKEMON_YELLOW: topFrame = sfil_load_PNG_file(ROMFS "frame_gbc_yellow.png", SF2D_PLACE_RAM); break;
	}

	return (topFrame ? 0 : -5);
}

void gfxFree(void)
{
	sf2d_free_texture(pkmIconsGBC);
	sf2d_free_texture(topFrame);
}

void gfxDrawPokemonIcon(int16_t x, int16_t y, GFX_Species icon, GFX_Frame frame)
{
	if (icon > SPECIES_MEW) icon = SPECIES_MISSINGNO;

	sf2d_draw_texture_part(pkmIconsGBC, x, y, 16 * (frame + (icon % PKM_PER_ROW) * 2), 16 * (icon / PKM_PER_ROW), 16, 16);
}

void gfxDrawFrame(int16_t x, int16_t y)
{
	sf2d_draw_texture(topFrame, x, y);
}

void gfxDrawPanel(int16_t x, int16_t y, uint8_t row, uint8_t col)
{
	// Top left
	fontDrawChar8(x, y, 0x79);

	// Top right
	fontDrawChar8(x + (col + 1) * 8, y, 0x7B);

	// Bottom left
	fontDrawChar8(x, y + (row + 1) * 8, 0x7D);

	// Bottom right
	fontDrawChar8(x + (col + 1) * 8, y + (row + 1) * 8, 0x7E);

	// Horizontal
	for (uint8_t xcol = 0; xcol < col; xcol++)
	{
		fontDrawChar8(x + (xcol + 1) * 8, y, 0x7A);
		fontDrawChar8(x + (xcol + 1) * 8, y + (row + 1) * 8, 0x7A);
	}

	// Vertical
	for (uint8_t yrow = 0; yrow < row; yrow++)
	{
		fontDrawChar8(x, y + (yrow + 1) * 8, 0x7C);
		fontDrawChar8(x + (col + 1) * 8, y + (yrow + 1) * 8, 0x7C);
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
