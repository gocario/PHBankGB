#include "save.h"
#include "bank.h"
#include "pkdir.h"
#include "pokedex.h"
#include "personal.h"
#include "fs.h"

#include <3ds/result.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>

/// 
typedef enum
{
	/* Game */
	OFFSET_PARTY = 0x2F2C,		///< Party Pokémon list
	OFFSET_CURRENT = 0x30C0,	///< Current Box Pokémon list
	OFFSET_BOX_1 = 0x4000,		///< Box 1 Pokémon list
	OFFSET_BOX_2 = 0x4462,		///< Box 2 Pokémon list
	OFFSET_BOX_3 = 0x48C4,		///< Box 3 Pokémon list
	OFFSET_BOX_4 = 0x4D26,		///< Box 4 Pokémon list
	OFFSET_BOX_5 = 0x5188,		///< Box 5 Pokémon list
	OFFSET_BOX_6 = 0x55EA,		///< Box 6 Pokémon list
	OFFSET_BOX_7 = 0x6000,		///< Box 7 Pokémon list
	OFFSET_BOX_8 = 0x6462,		///< Box 8 Pokémon list
	OFFSET_BOX_9 = 0x68C4,		///< Box 9 Pokémon list
	OFFSET_BOX_10 = 0x6D26,		///< Box 10 Pokémon list
	OFFSET_BOX_11 = 0x7188,		///< Box 11 Pokémon list
	OFFSET_BOX_12 = 0x75EA,		///< Box 12 Pokémon list

	/* Bank */
	OFFSET_BBOX_1 = 0x100,		///< Bank box 1 Pokémon list
} SAV_PokemonListOffset;

const uint16_t SaveConst__sizeSave = SAVE_SIZE;
const uint16_t SaveConst__offsetPokedexOwned = 0x25A3;
const uint16_t SaveConst__offsetPokedexSeen = 0x25B6;
const uint16_t SaveConst__offsetChecksum = 0x3523;

uint64_t titleid;
uint8_t save[SAVE_SIZE];
uint8_t bank[BANK_SIZE];
SAV_Game sgame;
SAV_Bank sbank;
SAV_GameVersion vgame;
SAV_GameLang lgame;

Result saveLoad(void)
{
	Result ret;

	ret = saveReadFile(save, ROOT_FOLDER SAVEGB_FILE);
	if (R_FAILED(ret)) return ret;

	saveReadData(save, &sgame);
	bankReadData(bank, &sbank, bankReadFile(bank, BANK_FOLDER BANKGB_FILE));
	
	return (true && true ? 0 : -5);
}

void saveExit(void)
{
	saveWriteData(save, &sgame);
	saveWriteFile(save, ROOT_FOLDER SAVEGB_FILE);

	mkdir(BASE_FOLDER, 0700);
	mkdir(BANK_FOLDER, 0700);

	bankWriteData(bank, &sbank);
	bankWriteFile(bank, BANK_FOLDER BANKGB_FILE);
}

SAV_GameVersion saveGetGameVersion(uint64_t titleid)
{
	switch (titleid)
	{
		case 0x0004000000170C00: ///< Pokémon Red (JPN) (JP)
		case 0x0004000000171000: ///< Pokémon Red (FREE) (EN)
		case 0x0004000000171300: ///< Pokémon Red (EUR) (DE)
		case 0x0004000000171600: ///< Pokémon Red (EUR) (FR) ??
		case 0x0004000000171900: ///< Pokémon Red (EUR) (ES)
		case 0x0004000000171C00: ///< Pokémon Red (EUR) (IT) ??
			return POKEMON_RED;
		case 0x0004000000170D00: ///< Pokémon Green (JPN) (JP)
			return POKEMON_GREEN;
		case 0x0004000000170E00: ///< Pokémon Blue? (JPN) (JP) ??
		case 0x0004000000171100: ///< Pokémon Blue (FREE) (EN)
		case 0x0004000000171400: ///< Pokémon Blue (EUR) (DE)
		case 0x0004000000171700: ///< Pokémon Blue (EUR) (FR)
		case 0x0004000000171A00: ///< Pokémon Blue (EUR) (ES)
		case 0x0004000000171D00: ///< Pokémon Blue (EUR) (IT) ??
			return POKEMON_BLUE;
		case 0x0004000000170F00: ///< Pokémon Yellow (JPN) (JP)
		case 0x0004000000171500: ///< Pokémon Yellow (EUR) (DE)
		case 0x0004000000171200: ///< Pokémon Yellow (FREE) (EN)
		case 0x0004000000171800: ///< Pokémon Yellow (EUR) (FR)
		case 0x0004000000171B00: ///< Pokémon Yellow (EUR) (ES)
		case 0x0004000000171E00: ///< Pokémon Yellow (EUR) (IT)
			return POKEMON_YELLOW;
		default:
			return NOT_POKEMON;
	}
}

SAV_GameLang saveGetGameLang(uint64_t titleid)
{
	switch (titleid)
	{
		case 0x0004000000170C00: ///< Pokémon Red (JPN) (JP)
		case 0x0004000000170D00: ///< Pokémon Green (JPN) (JP)
		case 0x0004000000170E00: ///< Pokémon Blue? (JPN) (JP) ??
		case 0x0004000000170F00: ///< Pokémon Yellow (JPN) (JP)
			return POKEMON_JP;
		case 0x0004000000171000: ///< Pokémon Red (FREE) (EN)
		case 0x0004000000171100: ///< Pokémon Blue (FREE) (EN)
		case 0x0004000000171200: ///< Pokémon Yellow (FREE) (EN)
			return POKEMON_EN;
		case 0x0004000000171600: ///< Pokémon Red (EUR) (FR) ??
		case 0x0004000000171700: ///< Pokémon Blue (EUR) (FR)
		case 0x0004000000171800: ///< Pokémon Yellow (EUR) (FR)
			return POKEMON_FR;
		case 0x0004000000171300: ///< Pokémon Red (EUR) (DE)
		case 0x0004000000171400: ///< Pokémon Blue (EUR) (DE)
		case 0x0004000000171500: ///< Pokémon Yellow (EUR) (DE)
			return POKEMON_DE;
		case 0x0004000000171C00: ///< Pokémon Red (EUR) (IT) ??
		case 0x0004000000171D00: ///< Pokémon Blue (EUR) (IT) ??
		case 0x0004000000171E00: ///< Pokémon Yellow (EUR) (IT)
			return POKEMON_IT;
		case 0x0004000000171B00: ///< Pokémon Yellow (EUR) (ES)
		case 0x0004000000171900: ///< Pokémon Red (EUR) (ES)
		case 0x0004000000171A00: ///< Pokémon Blue (EUR) (ES)
			return POKEMON_ES;
		default:
			return NOT_POKEMON;
	}
}

/**
 * @brief Reads a Pokémon from the savedata buffer.
 * @param[in] save The savedata buffer to the list offset.
 * @param[out] pkm The Pokémon.
 * @param index The index in the list.
 * @param size The size of a Pokémon struct.
 * @param capacity The capacity of the list.
 */
static void saveExtractPokemon(const uint8_t* save, SAV_Pokemon* pkm, uint8_t index, uint8_t size, uint8_t capacity)
{
	const uint8_t* pkmbuf;

	// Count (1) + Species.index (index)
	pkmbuf = save + 1 + index;

	// Extra attributes
	pkm->species = pkmbuf[0x00];
	pkm->nationalDex = pokedexGetNational(pkm->species);

	// Count (1) + Species (capacity+1) + Pokémon.index (index*size)
	pkmbuf = save + 2 + capacity + index * size;

	// Shared attributes
	pkm->speciesIndex = pkmbuf[0x00];
	pkm->currentHP = pkmbuf[0x01] << 8 | pkmbuf[0x02];
	pkm->currentLevel = pkmbuf[0x03];
	pkm->status = pkmbuf[0x04];
	pkm->types[0] = pkmbuf[0x05];
	pkm->types[1] = pkmbuf[0x06];
	pkm->catchRate = pkmbuf[0x07];
	pkm->moves[0] = pkmbuf[0x08];
	pkm->moves[1] = pkmbuf[0x09];
	pkm->moves[2] = pkmbuf[0x0A];
	pkm->moves[3] = pkmbuf[0x0B];
	pkm->originalTID = pkmbuf[0x0C] << 8 | pkmbuf[0x0D];
	pkm->experience = pkmbuf[0x0E] << 16 | pkmbuf[0x0F] << 8 | pkmbuf[0x10];
	pkm->EVs[0] = pkmbuf[0x11] << 8 | pkmbuf[0x12];
	pkm->EVs[1] = pkmbuf[0x13] << 8 | pkmbuf[0x14];
	pkm->EVs[2] = pkmbuf[0x15] << 8 | pkmbuf[0x16];
	pkm->EVs[3] = pkmbuf[0x17] << 8 | pkmbuf[0x18];
	pkm->EVs[4] = pkmbuf[0x19] << 8 | pkmbuf[0x1A];
	pkm->IVs[1] = pkmbuf[0x1B] >> 4;
	pkm->IVs[2] = pkmbuf[0x1B] & 0x0F;
	pkm->IVs[3] = pkmbuf[0x1C] >> 4;
	pkm->IVs[4] = pkmbuf[0x1C] & 0x0F;
	pkm->PPUps[0] = pkmbuf[0x1D] >> 6;
	pkm->PPUps[1] = pkmbuf[0x1E] >> 6;
	pkm->PPUps[2] = pkmbuf[0x1F] >> 6;
	pkm->PPUps[3] = pkmbuf[0x20] >> 6;
	pkm->PPs[0] = pkmbuf[0x1D] & 0x3F;
	pkm->PPs[1] = pkmbuf[0x1E] & 0x3F;
	pkm->PPs[2] = pkmbuf[0x1F] & 0x3F;
	pkm->PPs[3] = pkmbuf[0x20] & 0x3F;

	pkm->IVs[STAT_HP] = ((pkm->IVs[STAT_ATK] & 0x1) << 3) | ((pkm->IVs[STAT_DEF] & 0x1) << 2) | ((pkm->IVs[STAT_SPE] & 0x1) << 1) |	((pkm->IVs[STAT_SPC] & 0x1) << 0);

	if (size == 0x2C)
	{
		// Party attributes
		pkm->level = pkmbuf[0x21];
		pkm->maxHP = pkmbuf[0x22] << 8 | pkmbuf[0x23];
		pkm->ATK = pkmbuf[0x24] << 8 | pkmbuf[0x25];
		pkm->DEF = pkmbuf[0x26] << 8 | pkmbuf[0x27];
		pkm->SPE = pkmbuf[0x28] << 8 | pkmbuf[0x29];
		pkm->SPC = pkmbuf[0x2A] << 8 | pkmbuf[0x2B];
	}
	else
	{
		const PersonalInfo* pInfo = Personal(pkm->nationalDex);

		// Compute the level? No!
		pkm->level = pkm->currentLevel;
		pkm->maxHP = ((((pInfo->HP + pkm->IVs[STAT_HP]) * 2 + sqrt(pkm->EVs[STAT_HP]) / 4) * pkm->level) / 100) + pkm->level + 10;
		pkm->ATK = ((((pInfo->ATK + pkm->IVs[STAT_ATK]) * 2 + sqrt(pkm->EVs[STAT_ATK]) / 4) * pkm->level) / 100) + 5;
		pkm->DEF = ((((pInfo->DEF + pkm->IVs[STAT_DEF]) * 2 + sqrt(pkm->EVs[STAT_DEF]) / 4) * pkm->level) / 100) + 5;
		pkm->SPE = ((((pInfo->SPE + pkm->IVs[STAT_SPE]) * 2 + sqrt(pkm->EVs[STAT_SPE]) / 4) * pkm->level) / 100) + 5;
		pkm->SPC = ((((pInfo->SPC + pkm->IVs[STAT_SPC]) * 2 + sqrt(pkm->EVs[STAT_SPC]) / 4) * pkm->level) / 100) + 5;
	}

	// Count (1) + Species (capacity+1) + Pokémon (capacity*size) + OT_Name.index (index*11)
	pkmbuf = save + 2 + capacity * (size + 1) + index * 11;

	// Extra attributes
	for (uint8_t i = 0; i < 11; i++)
		pkm->nameOT[i] = pkmbuf[i];

	// Count (1) + Species (capacity+1) + Pokémon (capacity*size) + OT_Name (capacity*11) + NK_Name.index (index*11)
	pkmbuf = save + 2 + capacity * (size + 1) + (capacity + index) * 11;

	// Extra attributes
	for (uint8_t i = 0; i < 11; i++)
		pkm->nameNK[i] = pkmbuf[i];

	pkm->moved = false;
}

/**
 * @brief Writes a Pokémon to the savedata buffer.
 * @param[in/out] save The savedata buffer to the list offset.
 * @param[in] pkm The Pokémon.
 * @param index The index in the list.
 * @param size The size of a Pokémon struct.
 * @param capacity The capacity of the list.
 */
static void saveInjectPokemon(uint8_t* save, const SAV_Pokemon* pkm, uint8_t index, uint8_t size, uint8_t capacity)
{
	if (!pkm->moved) return;

	uint8_t* pkmbuf;

	// Count (1) + Species.index (index)
	pkmbuf = save + 1 + index;

	// Extra attributes
	pkmbuf[0x00] = pkm->species;

	// Count (1) + Species (capacity+1) + Pokémon.index (index*size)
	pkmbuf = save + 2 + capacity + index * size;

	// Shared attributes
	pkmbuf[0x00] = pkm->speciesIndex;
	pkmbuf[0x01] = pkm->currentHP >> 8;
	pkmbuf[0x02] = pkm->currentHP & 0xFF;
	pkmbuf[0x03] = pkm->currentLevel;
	pkmbuf[0x04] = pkm->status;
	pkmbuf[0x05] = pkm->types[0];
	pkmbuf[0x06] = pkm->types[1];
	pkmbuf[0x07] = pkm->catchRate;
	pkmbuf[0x08] = pkm->moves[0];
	pkmbuf[0x09] = pkm->moves[1];
	pkmbuf[0x0A] = pkm->moves[2];
	pkmbuf[0x0B] = pkm->moves[3];
	pkmbuf[0x0C] = pkm->originalTID >> 8;
	pkmbuf[0x0D] = pkm->originalTID & 0xFF;
	pkmbuf[0x0E] = pkm->experience >> 16 & 0xFF;
	pkmbuf[0x0F] = pkm->experience >> 8 & 0xFF;
	pkmbuf[0x10] = pkm->experience & 0xFF;
	pkmbuf[0x11] = pkm->EVs[0] >> 8;
	pkmbuf[0x12] = pkm->EVs[0] & 0xFF;
	pkmbuf[0x13] = pkm->EVs[1] >> 8;
	pkmbuf[0x14] = pkm->EVs[1] & 0xFF;
	pkmbuf[0x15] = pkm->EVs[2] >> 8;
	pkmbuf[0x16] = pkm->EVs[2] & 0xFF;
	pkmbuf[0x17] = pkm->EVs[3] >> 8;
	pkmbuf[0x18] = pkm->EVs[3] & 0xFF;
	pkmbuf[0x19] = pkm->EVs[4] >> 8;
	pkmbuf[0x1A] = pkm->EVs[4] & 0xFF;
	pkmbuf[0x1B] = (pkm->IVs[1] & 0x0F) << 4 | (pkm->IVs[2] & 0x0F);
	pkmbuf[0x1C] = (pkm->IVs[3] & 0x0F) << 4 | (pkm->IVs[4] & 0x0F);
	pkmbuf[0x1D] = (pkm->PPUps[0] & 0x3) << 6 | (pkm->PPs[0] & 0x3F);
	pkmbuf[0x1E] = (pkm->PPUps[1] & 0x3) << 6 | (pkm->PPs[1] & 0x3F);
	pkmbuf[0x1F] = (pkm->PPUps[2] & 0x3) << 6 | (pkm->PPs[2] & 0x3F);
	pkmbuf[0x20] = (pkm->PPUps[3] & 0x3) << 6 | (pkm->PPs[3] & 0x3F);

	if (size == 0x2C)
	{
		// Party attributes
		pkmbuf[0x21] = pkm->level;
		pkmbuf[0x22] = pkm->maxHP >> 8;
		pkmbuf[0x23] = pkm->maxHP & 0xFF;
		pkmbuf[0x24] = pkm->ATK >> 8;
		pkmbuf[0x25] = pkm->ATK & 0xFF;
		pkmbuf[0x26] = pkm->DEF >> 8;
		pkmbuf[0x27] = pkm->DEF & 0xFF;
		pkmbuf[0x28] = pkm->SPE >> 8;
		pkmbuf[0x29] = pkm->SPE & 0xFF;
		pkmbuf[0x2A] = pkm->SPC >> 8;
		pkmbuf[0x2B] = pkm->SPC & 0xFF;
	}

	// Count (1) + Species (capacity+1) + Pokémon (capacity*size) + OT_Name.index (index*11)
	pkmbuf = save + 2 + capacity * (size + 1) + index * 11;

	// Extra attributes
	for (uint8_t i = 0; i < 11; i++)
		pkmbuf[i] = pkm->nameOT[i];

	// Count (1) + Species (capacity+1) + Pokémon (capacity*size) + OT_Name (capacity*11) + NK_Name.index (index*11)
	pkmbuf = save + 2 + capacity * (size + 1) + (capacity + index) * 11;

	// Extra attributes
	for (uint8_t i = 0; i < 11; i++)
		pkmbuf[i] = pkm->nameNK[i];
}

/**
 * @brief Reads the data from a save to a structure.
 * @param[in] save The savedata buffer.
 * @param[out] pkmList The Pokémon list to populate.
 * @param listOffset The offset of the list.
 */
static void saveExtractPokemonList(const uint8_t* save, SAV_PokemonList* pkmList, SAV_PokemonListOffset listOffset, uint8_t size, uint8_t capacity)
{
	pkmList->count = save[listOffset];
	pkmList->size = size;
	pkmList->capacity = capacity;

	// printf("List %u/%u (x%u)\n", pkmList->count, pkmList->capacity, pkmList->size);

	for (uint8_t i = 0; i < pkmList->capacity; i++)
		saveExtractPokemon(save + listOffset, &pkmList->slots[i], i, pkmList->size, pkmList->capacity);
}

/**
 * @brief Writes the data from a structure to a save.
 * @param[in/out] save The savedata buffer.
 * @param[in] pkmList The Pokémon list to inject.
 * @param listOffset The offset of the list.
 */
static void saveInjectPokemonList(uint8_t* save, const SAV_PokemonList* pkmList, SAV_PokemonListOffset listOffset)
{
	// printf("List %u/%u (%u)\n", pkmList->size, pkmList->count, pkmList->capacity);

	save[listOffset] = pkmList->count;

	for (uint8_t i = 0; i < pkmList->capacity; i++)
		saveInjectPokemon(save + listOffset, &pkmList->slots[i], i, pkmList->size, pkmList->capacity);
}

/**
 * @brief Gets the current box id.
 * @param[in] save The savedata buffer.
 * @return The id of the current box.
 */
static uint8_t saveGetCurrentBox(const uint8_t* save)
{
	return save[0x284C] & 0x7F;
}

SAV_PokemonList* gameGetBox(uint8_t box)
{
	return &sgame.boxes[box];
}

SAV_PokemonList* bankGetBox(uint8_t box)
{
	return &sbank.boxes[box];
}

SAV_PokemonList* saveGetBox(uint8_t box, bool inBank)
{
	return (inBank ? bankGetBox(box) : gameGetBox(box));
}

SAV_Pokemon* saveGetPkm(uint8_t box, uint8_t slot, bool inBank)
{
	return &saveGetBox(box, inBank)->slots[slot];
}

/**
 * @brief Swaps two Pokémon.
 * @param src The Pokémon source.
 * @param dst The Pokémon destination.
 * @return Whether the Pokémon have been swapped.
 */
static bool _saveMovePkm(SAV_Pokemon* src, SAV_Pokemon* dst)
{
	// We copy the pokemon data
	SAV_Pokemon tmp;
	memcpy(&tmp, dst, sizeof(SAV_Pokemon));
	memcpy(dst, src, sizeof(SAV_Pokemon));
	memcpy(src, &tmp, sizeof(SAV_Pokemon));

	// We valid the Pokémon move'd
	src->moved = true;
	dst->moved = true;

	return true;
}

bool saveMovePkm(SAV_Pokemon* src, SAV_Pokemon* dst, bool srcBanked, bool dstBanked)
{
	return _saveMovePkm(src, dst);
}

/**
 * @brief Pastes a Pokémon over another.
 * @param src The Pokémon source.
 * @param dst The Pokémon destination.
 * @return Whether the Pokémon has been pasted.
 */
static bool _savePastePkm(SAV_Pokemon* src, SAV_Pokemon* dst)
{
	// We copy the pokemon data
	memcpy(dst, src, sizeof(SAV_Pokemon));

	// We valid the Pokémon move'd
	dst->moved = true;

	return true;
}

bool savePastePkm(SAV_Pokemon* src, SAV_Pokemon* dst, bool srcBanked, bool dstBanked)
{
	return _savePastePkm(src, dst);
}

bool saveIsPkmEmpty(const SAV_Pokemon* pkm)
{
	return pkm->species == 0x00 || pkm->species > 0xBE;
}

const char8_t* saveGetTrainer(void)
{
	return save + 0x2598;
}

Result saveReadFile(uint8_t* save, const char* path)
{
	Result ret;
	uint32_t bytesRead = 0;

	printf("Reading save file...");
	ret = FS_ReadFile(path, save, &saveArchive, SAVE_SIZE, &bytesRead);

	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Read %ld/%d bytes\n", bytesRead, SAVE_SIZE);

	return ret;
}

Result saveWriteFile(const uint8_t* save, const char* path)
{
	Result ret;
	uint32_t bytesWritten = 0;

	printf("Deleting old save file...");
	ret = FS_DeleteFile(path, &saveArchive);
	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n");

	printf("Writing save file...");
	ret = FS_WriteFile(path, save, SAVE_SIZE, &saveArchive, &bytesWritten);
	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n  Written %ld/%d bytes\n", bytesWritten, SAVE_SIZE);

	return bytesWritten;
}

void saveReadData(const uint8_t* save, SAV_Game* sgame)
{
	char title[11];
	sgame->boxCount = 12;

	for (uint8_t i = 0; i < sgame->boxCount; i++)
	{
		sgame->boxes[i].index = i;
		sprintf(title, "Box %u", i+1);
		fontConvertString(sgame->boxes[i].title, title);
		saveExtractPokemonList(save, &sgame->boxes[i], (i == saveGetCurrentBox(save) ? OFFSET_CURRENT : OFFSET_BOX_1 + (i < 6 ?  0 : 0x5B4) + i * BOX_SIZE(20,0x21)), 0x21, 20); // TODO: Adapt for foreign games
	}
}

void saveWriteData(uint8_t* save, SAV_Game* sgame)
{
	for (uint8_t iB = 0; iB < sgame->boxCount; iB++)
	{
		// Iterate to find the empty slots
		for (uint8_t iP = 0, iPmin = 0; iP < sgame->boxes[iB].capacity; iP++)
		{
			// If looking for the next empty slot
			if (iPmin == 0)
			{
				if (saveIsPkmEmpty(&sgame->boxes[iB].slots[iP]))
				{
					iPmin = iP+1;
				}
			}
			// If looking for the next occupied slot
			else
			{
				if (!saveIsPkmEmpty(&sgame->boxes[iB].slots[iP]))
				{
					printf("Moving %03u -> %03u\n",
						sgame->boxes[iB].slots[iP].nationalDex,
						sgame->boxes[iB].slots[iPmin-1].nationalDex);
					_saveMovePkm(&sgame->boxes[iB].slots[iP], &sgame->boxes[iB].slots[iPmin-1]);
					iP = iPmin-1;
					iPmin = 0;
				}
			}
		}

		// Count the Pokémon
		sgame->boxes[iB].count = 0;
		for (uint8_t iP = 0; iP < sgame->boxes[iB].capacity; iP++)
		{
			if (!saveIsPkmEmpty(&sgame->boxes[iB].slots[iP]))
				sgame->boxes[iB].count++;

			// Might be useless
			if (sgame->boxes[iB].slots[iP].species == 0x00)
				sgame->boxes[iB].slots[iP].species = 0xFF;

			pokedexAddOwned(sgame->boxes[iB].slots[iP].nationalDex);
			pokedexAddSeen(sgame->boxes[iB].slots[iP].nationalDex);
		}

		saveInjectPokemonList(save, &sgame->boxes[iB], (iB == saveGetCurrentBox(save) ? OFFSET_CURRENT : OFFSET_BOX_1 + (iB < 6 ?  0 : 0x5B4) + iB * BOX_SIZE(20,0x21))); // TODO: Adapt for foreign games
	}

	saveFixChecksum(save);
}

void saveFixChecksum(uint8_t* save)
{
	uint8_t chk = 0;

	// [0x2598 -> 0x3522]
	for (uint16_t off = 0x2598; off < 0x3523; off++)
	{
		chk += save[off];
	}

	save[0x3523] = ~chk;
}

uint16_t bankReadFile(uint8_t* bank, const char* path)
{
	uint16_t bytesRead = 0;

	printf("Opening bank file...\n");
	FILE* fp = fopen(path, "rb");
	if (fp)
	{
		printf("Reading bank file...");
		bytesRead = fread(bank, 1, BANK_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %d/%d bytes\n", bytesRead, BANK_SIZE);

		if (!ferror(fp))
		{
			memset(bank + bytesRead, 0, BANK_SIZE - bytesRead);
			bankUpdate(bank, bytesRead);
		}

		fclose(fp);
	}
	else
	{
		printf(" Creating...");
		memset(bank, 0, BANK_SIZE);
		bankUpdate(bank, BANK_SIZE);

		printf(" OK\n  Created %d bytes\n", BANK_SIZE);
	}

	return bytesRead;
}

uint16_t bankWriteFile(const uint8_t* bank, const char* path)
{
	uint16_t bytesWritten = 0;

	printf("Opening bank file...\n");
	FILE* fp = fopen(path, "wb");
	if (fp)
	{
		printf("Writing bank file...");
		bytesWritten = fwrite(bank, 1, BANK_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Written %d/%d bytes\n", bytesWritten, BANK_SIZE);

		fclose(fp);

	}

	return bytesWritten;
}

void bankReadData(uint8_t* bank, SAV_Bank* sbank, uint16_t bytesRead)
{
	char title[11];
	sbank->magic = *(uint32_t*)(bank + 0x00);
	sbank->version = *(uint32_t*)(bank + 0x04);
	sbank->boxCount = BANK_BOX_MAX_COUNT;

	for (uint8_t i = 0; i < sbank->boxCount; i++)
	{
		sbank->boxes[i].index = i;
		sprintf(title, "Box %u", i+1);
		fontConvertString(sbank->boxes[i].title, title);
		saveExtractPokemonList(bank, &sbank->boxes[i], OFFSET_BBOX_1 + i * BOX_SIZE(32,0x21), 0x21, POKEMON_LIST_MAX_COUNT);
	}
}

void bankWriteData(uint8_t* bank, SAV_Bank* sbank)
{
	*(uint32_t*)(bank + 0x00) = sbank->magic;
	*(uint32_t*)(bank + 0x04) = sbank->version;

	for (uint8_t iB = 0; iB < sbank->boxCount; iB++)
	{
		// Iterate to find the empty slots
		for (uint8_t iP = 0, iPmin = 0; iP < sbank->boxes[iB].capacity; iP++)
		{
			// If looking for the next empty slot
			if (iPmin == 0)
			{
				if (saveIsPkmEmpty(&sbank->boxes[iB].slots[iP]))
				{
					iPmin = iP+1;
				}
			}
			// If looking for the next occupied slot
			else
			{
				if (!saveIsPkmEmpty(&sbank->boxes[iB].slots[iP]))
				{
					printf("Moving %03u -> %03u\n",
						sbank->boxes[iB].slots[iP].nationalDex,
						sbank->boxes[iB].slots[iPmin-1].nationalDex);
					_saveMovePkm(&sbank->boxes[iB].slots[iP], &sbank->boxes[iB].slots[iPmin-1]);
					iP = iPmin-1;
					iPmin = 0;
				}
			}
		}

		// Count the Pokémon
		sbank->boxes[iB].count = 0;
		for (uint8_t iP = 0; iP < sbank->boxes[iB].capacity; iP++)
		{
			if (!saveIsPkmEmpty(&sbank->boxes[iB].slots[iP]))
				sbank->boxes[iB].count++;

			// Might be useless
			if (sbank->boxes[iB].slots[iP].species == 0x00)
				sbank->boxes[iB].slots[iP].species = 0xFF;
		}

		saveInjectPokemonList(bank, &sbank->boxes[iB], OFFSET_BBOX_1 + iB * BOX_SIZE(32,0x21));
	}
}
