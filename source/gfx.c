#include "gfx.h"
#include "font.h"

#define PKM_PER_ROW (8)

typedef const struct {
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel;
	unsigned char pixel_data[];
} RAW_img;

extern RAW_img pkmIconsGBC_img;

sf2d_texture* pkmIconsGBC;

void gfxCreate(void)
{
	pkmIconsGBC = sf2d_create_texture_mem_RGBA8(pkmIconsGBC_img.pixel_data, pkmIconsGBC_img.width, pkmIconsGBC_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void gfxFree(void)
{
	sf2d_free_texture(pkmIconsGBC);
}

GFX_PkmIcon gfxGetPokemonIcon(uint8_t species)
{
	switch (species)
	{
		case   4: // Charmender
		case   5: // Charmeleon
		case   6: // Charizard
		case  27: // Sandshrew
		case  28: // Sandslash
		case  29: // Nidoran♀
		case  30: // Nidorina
		case  31: // Nidoqueen
		case  32: // Nidoran♂
		case  33: // Nidorino
		case  34: // Nidoking
		case  41: // Zubat
		case  42: // Golbat
		case  50: // Diglett
		case  51: // Dugtrio
		case  52: // Meowth
		case  53: // Persian
		case  54: // Psyduck
		case  55: // Golduck
		case  56: // Mankey
		case  57: // Primeape
		case  60: // Poliwag
		case  61: // Poliwhirl
		case  62: // Poliwrath
		case  63: // Abra
		case  64: // Kadabra
		case  65: // Alakazam
		case  66: // Machop
		case  67: // Machoke
		case  68: // Machamp
		case  74: // Geodude
		case  75: // Graveler
		case  76: // Golem
		case  80: // Slowbro
		case  88: // Grimer
		case  89: // Muk
		case  92: // Gastly
		case  93: // Haunter
		case  94: // Gengar
		case  96: // Drowzee
		case  97: // Hypno
		case 104: // Cubone
		case 105: // Marowak
		case 106: // Hitmonlee
		case 107: // Hitmonchan
		case 108: // Lickitung
		case 109: // Koffing
		case 110: // Weezing
		case 112: // Rhydon
		case 115: // Kangaskhan
		case 122: // Mr. Mime
		case 124: // Jynx
		case 125: // Electabuzz
		case 126: // Magmar
		case 132: // Ditto
		case 137: // Porygon
		case 143: // Snorlax
		case 150: // Mewtwo
		case 151: // Mew
		return PKM_ICON_BIPED;

		case  81: // Magnemite
		case  82: // Magneton
		case 100: // Voltorb
		case 101: // Electrode
		return PKM_ICON_BALL;
		
		case  90: // Shellder
		case  91: // Cloyster
		case 120: // Staryu
		case 121: // Starmie
		case 138: // Omanyte
		case 139: // Omastar
		case 140: // Kabuto
		case 141: // Kabutops
		return PKM_ICON_FOSSIL;

		case  25: // Pikachu
		case  26: // Raichu
		case  35: // Clefairy
		case  36: // Clefable
		case  39: // Jigglypuff
		case  40: // Wigglypuff
		case 113: // Chansey
		return PKM_ICON_FAIRY;

		case  16: // Pidgey
		case  17: // Pidgeotto
		case  18: // Pidgeot
		case  21: // Spearow
		case  22: // Fearow
		case  83: // Farfetch'd
		case  84: // Doduo
		case  85: // Dodrio
		case 142: // Aerodactyl
		case 144: // Articuno
		case 145: // Zapdos
		case 146: // Moltres
		return PKM_ICON_BIRD;

		case   7: // Squirtle
		case   8: // Wartortle
		case   9: // Blastoise
		case  72: // Tentacool
		case  73: // Tentacruel
		case  86: // Seel
		case  87: // Dewgong
		case  98: // Krabby
		case  99: // Kingler
		case 116: // Horsea
		case 117: // Seadra
		case 118: // Goldeen
		case 119: // Seaking
		case 129: // Magikarp
		case 131: // Lapras
		return PKM_ICON_WATER;

		case  10: // Caterpie
		case  11: // Metapod
		case  12: // Butterfree
		case  13: // Weedle
		case  14: // Kakuna
		case  15: // Beedrill
		case  46: // Paras
		case  47: // Parasect
		case  48: // Venonat
		case  49: // Venomoth
		case 123: // Scynther
		case 127: // Pinsisr
		return PKM_ICON_BUG;

		case   1: // Bulbasaur
		case   2: // Ivysaur
		case   3: // Venusaur
		case  43: // Oddish
		case  44: // Gloom
		case  45: // Vileplume
		case  69: // Bellsprout
		case  70: // Weepinbell
		case  71: // Victreebel
		case 102: // Exeggcute
		case 103: // Exeggutor
		case 114: // Tangela
		return PKM_ICON_PLANT;

		case  23: // Ekans
		case  24: // Arbok
		case  95: // Onix
		case 130: // Gyarados
		case 147: // Dratini
		case 148: // Dragonair
		case 149: // Dragonite
		return PKM_ICON_SNAKE;

		case  19: // Rattata
		case  20: // Raticate
		case  37: // Vulpix
		case  38: // Ninetales
		case  58: // Growlithe
		case  59: // Arcanine
		case  77: // Ponyta
		case  78: // Rapidash
		case  79: // Slowpoke
		case 111: // Rhyhorn
		case 128: // Tauros
		case 133: // Eevee
		case 134: // Vaporeon
		case 135: // Jolteon
		case 136: // Flareon
		return PKM_ICON_QUADRUPED;

		default: // Missingno.
		return PKM_ICON_MISSINGNO;
	}
}

void gfxDrawPokemonIcon(int16_t x, int16_t y, GFX_Species icon, GFX_Frame frame)
{
	if (icon > SPECIES_MEW) icon = SPECIES_MISSINGNO;

	sf2d_draw_texture_part(pkmIconsGBC, x, y, 16 * (frame + (icon % PKM_PER_ROW) * 2), 16 * (icon / PKM_PER_ROW), 16, 16);
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
