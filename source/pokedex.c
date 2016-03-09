#include "pokedex.h"
#include "save.h"

extern const uint32_t SaveConst__offsetPokedexOwned;
extern const uint32_t SaveConst__offsetPokedexSeen;

/**
 * @brief Gets a bit of a buffer.
 * @param buf The buffer to extract the bit from.
 * @param off The offset (in byte).
 * @param bit The offset (in bit).
 */
static inline bool getOffsetBit(const uint8_t* buf, uint32_t off, uint32_t bit)
{
	return ((buf[off+(bit/8)] >> (bit%8)) & 0x1) == 1;
}

/**
 * @brief Sets a bit of a buffer.
 * @param buf The buffer to inject the bit to.
 * @param off The offset (in byte).
 * @param bit The offset (in bit).
 * @param v The new value of the bit.
 */
static inline void setOffsetBit(uint8_t* buf, uint32_t off, uint32_t bit, bool v)
{
	buf[off+(bit/8)] = (buf[off+(bit/8)] & ~(0x1 << (bit%8))) | ((v?1:0) << (bit%8));
}

uint8_t pokedexGetNational(uint8_t index)
{
	switch (index)
	{
		case 0x01: return 112; // Rhydon
		case 0x02: return 115; // Kangaskhan
		case 0x03: return  32; // Nidoran♂
		case 0x04: return  35; // Clefairy
		case 0x05: return  21; // Spearow
		case 0x06: return 100; // Voltorb
		case 0x07: return  34; // Nidoking
		case 0x08: return  80; // Slowbro
		case 0x09: return   2; // Ivysaur
		case 0x0A: return 103; // Exeggutor
		case 0x0B: return 108; // Lickitng
		case 0x0C: return 102; // Exeggcute
		case 0x0D: return  88; // Grimer
		case 0x0E: return  94; // Gengar
		case 0x0F: return  29; // Nidoran♀
		case 0x10: return  31; // Nidoqueen
		case 0x11: return 104; // Cubone
		case 0x12: return 111; // Rhyhorn
		case 0x13: return 131; // Lapras
		case 0x14: return  59; // Arcanine
		case 0x15: return 151; // Mew
		case 0x16: return 130; // Gyarados
		case 0x17: return  90; // Shellder
		case 0x18: return  72; // Tentacool
		case 0x19: return  92; // Gastly
		case 0x1A: return 123; // Scyther
		case 0x1B: return 120; // Staryu
		case 0x1C: return   9; // Blastoise
		case 0x1D: return 127; // Pinsir
		case 0x1E: return 114; // Tangela
		case 0x21: return  58; // Growlith
		case 0x22: return  95; // Onix
		case 0x23: return  22; // Fearow
		case 0x24: return  16; // Pidgey
		case 0x25: return  79; // Slowpoke
		case 0x26: return  64; // Kadabra
		case 0x27: return  75; // Graveler
		case 0x28: return 113; // Chansey
		case 0x29: return  67; // Machoke
		case 0x2A: return 122; // Mr. Mime
		case 0x2B: return 106; // Hitmonlee
		case 0x2C: return 107; // Hitmonchan
		case 0x2D: return  24; // Arbok
		case 0x2E: return  47; // Parasek
		case 0x2F: return  54; // Psyduck
		case 0x30: return  96; // Drowzee
		case 0x31: return  76; // Golem
		case 0x33: return 126; // Magmar
		case 0x35: return 125; // Electabuzz
		case 0x36: return  82; // Magneton
		case 0x37: return 109; // Koffing
		case 0x39: return  56; // Mankey
		case 0x3A: return  86; // Seel
		case 0x3B: return  50; // Diglett
		case 0x3C: return 128; // Tauros
		case 0x40: return  83; // Farfetch'd
		case 0x41: return  48; // Venonat
		case 0x42: return 149; // Dragonite
		case 0x46: return  84; // Doduo
		case 0x47: return  60; // Poliwag
		case 0x48: return 124; // Jynx
		case 0x49: return 146; // Moltres
		case 0x4A: return 144; // Articuno
		case 0x4B: return 145; // Zapdos
		case 0x4C: return 132; // Ditto
		case 0x4D: return  52; // Meowth
		case 0x4E: return  98; // Krabby
		case 0x52: return  37; // Vulpix
		case 0x53: return  38; // Ninetales
		case 0x54: return  25; // Pikachu
		case 0x55: return  26; // Raichu
		case 0x58: return 147; // Dratini
		case 0x59: return 148; // Dragonair
		case 0x5A: return 140; // Kabuto
		case 0x5B: return 141; // Kabutops
		case 0x5C: return 116; // Horsea
		case 0x5D: return 117; // Seadra
		case 0x60: return  27; // Sandshrew
		case 0x61: return  28; // Sandslash
		case 0x62: return 138; // Omanyte
		case 0x63: return 139; // Omastar
		case 0x64: return  39; // Jigglypuff
		case 0x65: return  40; // Wigglytuff
		case 0x66: return 133; // Eevee
		case 0x67: return 136; // Flareon
		case 0x68: return 135; // Jolteon
		case 0x69: return 134; // Vaporeon
		case 0x6A: return  66; // Machop
		case 0x6B: return  41; // Zubat
		case 0x6C: return  23; // Ekans
		case 0x6D: return  46; // Para
		case 0x6E: return  61; // Poliwhirl
		case 0x6F: return  62; // Poliwrath
		case 0x70: return  13; // Weedle
		case 0x71: return  14; // Kakuna
		case 0x72: return  15; // Beedrill
		case 0x74: return  85; // Dodrio
		case 0x75: return  57; // Primeape
		case 0x76: return  51; // Dugtrio
		case 0x77: return  49; // Venomoth
		case 0x78: return  87; // Dewgong
		case 0x7B: return  10; // Caterpie
		case 0x7C: return  11; // Metapod
		case 0x7D: return  12; // Butterfree
		case 0x7E: return  68; // Machamp
		case 0x80: return  54; // Golduck
		case 0x81: return  97; // Hypno
		case 0x82: return  42; // Golbat
		case 0x83: return 150; // Mewtwo
		case 0x84: return 143; // Snorlax
		case 0x85: return 129; // Magikarp
		case 0x88: return  89; // Muk
		case 0x8A: return  99; // Kingler
		case 0x8B: return  91; // Cloyster
		case 0x8D: return 101; // Electrode
		case 0x8E: return  36; // Clefable
		case 0x8F: return 110; // Weezing
		case 0x90: return  53; // Persian
		case 0x91: return 105; // Marowak
		case 0x93: return  93; // Haunter
		case 0x94: return  63; // Abra
		case 0x95: return  65; // Alakazam
		case 0x96: return  17; // Pidgeotto
		case 0x97: return  18; // Pidgeot
		case 0x98: return 121; // Starmie
		case 0x99: return   1; // Bulbasaur
		case 0x9A: return   3; // Venusaur
		case 0x9B: return  73; // Tentacruel
		case 0x9D: return 118; // Goldeen
		case 0x9E: return 119; // Seaking
		case 0xA3: return  77; // Ponyta
		case 0xA4: return  78; // Rapidash
		case 0xA5: return  19; // Rattata
		case 0xA6: return  20; // Raticate
		case 0xA7: return  33; // Nidorino
		case 0xA8: return  30; // Nidorina
		case 0xA9: return  74; // Geodude
		case 0xAA: return 137; // Porygon
		case 0xAB: return 142; // Aerodactly
		case 0xAD: return  81; // Magnemite
		case 0xB0: return   4; // Charmander
		case 0xB1: return   7; // Squirtle
		case 0xB2: return   5; // Charmeleon
		case 0xB3: return   8; // Wartortle
		case 0xB4: return   6; // Charizard
		case 0xB9: return  43; // Oddish
		case 0xBA: return  44; // Gloom
		case 0xBB: return  45; // Vileplume
		case 0xBC: return  69; // Bellsprout
		case 0xBD: return  70; // Weepinbell
		case 0xBE: return  71; // Victreebel
		case 0x1F: // Scizor
		case 0x20: // Shuckle
		case 0x32: // Heracross
		case 0x34: // Ho-Oh
		case 0x38: // Sneasel
		case 0x3D: // Teddiursa
		case 0x3E: // Ursaring
		case 0x3F: // Slugma
		case 0x43: // Magcargo
		case 0x44: // Swinub
		case 0x45: // Piloswine
		case 0x4f: // Corsola
		case 0x50: // Remoraid
		case 0x56: // Delibird
		case 0x57: // Mantine
		case 0x5E: // Skarmory
		case 0x5F: // Houndour
		case 0x73: // Houndoom
		case 0x79: // Kingdra
		case 0x7A: // Phanpy
		case 0x7F: // Donphan
		case 0x86: // Porygon2
		case 0x87: // Stantler
		case 0x8C: // Tyrogue
		case 0x92: // Hitmontop
		case 0x9C: // Smoochum
		case 0x9F: // Elekid
		case 0xA0: // Magby
		case 0xA1: // Miltank
		case 0xA2: // Blissey
		case 0xAC: // Raikou
		case 0xAE: // Entei
		case 0xAF: // Suicune
		case 0xB5: // Larvitar
		case 0xB6: // Pupitar (Kabutops Fossil)
		case 0xB7: // Tyranitar (Aerodactly Fossil)
		case 0xB8: // Lugia (Ghost)
		           return 152; // Missingno.
		default:   return 153; // Garbage data
	}
}

void pokedexAddOwned(uint8_t species)
{
	if (species > 0 && species < 152)
	{
		setOffsetBit(save, SaveConst__offsetPokedexOwned, species-1, true);
	}
}

void pokedexAddSeen(uint8_t species)
{
	if (species > 0 && species < 152)
	{
		setOffsetBit(save, SaveConst__offsetPokedexSeen, species-1, true);
	}
}

bool pokedexIsFromTPP(uint8_t species, const char8_t* nickname, const char8_t* otname)
{
	// Lord Helix - A God
	if (species == SPECIES_OMANYTE)
	{
		// OMANYTE
		return true;
	}
	// Lord Helix - A God
	else if (species == SPECIES_OMASTAR)
	{
		// OMASTAR
		return true;
	}
	// Bird Jesus - A Messiah
	else if (species == SPECIES_PIDGEOT)
	{
		// aaabaaajss
		return true;
	}
	// Battery Jesus - An Angel
	else if (species == SPECIES_ZAPDOS)
	{
		// AA-J
		return true;
	}
	// The Fonz - A King
	else if (species == SPECIES_NIDOKING)
	{
		// AAAAAAAAA
		return true;
	}
	// Air Jordan - A Prince
	else if (species == SPECIES_LAPRAS)
	{
		// AIIIIIIRRR
		return true;
	}
	// AII-Terrain Venomoth - And an All Terrain Vehicle
	else if (species == SPECIES_VENOMOTH)
	{
		// AATTVVV
		return true;
	}

	return false;
}
