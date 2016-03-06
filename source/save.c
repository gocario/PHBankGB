#include "save.h"
#include "pokedex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// 
typedef enum
{
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
} SAV_PokemonListOffset;

const uint16_t SaveConst__sizeSave = SAVE_SIZE;
const uint16_t SaveConst__offsetPokedexOwned = 0x25A3;
const uint16_t SaveConst__offsetPokedexSeen = 0x25B6;
const uint16_t SaveConst__offsetChecksum = 0x3523;

uint8_t save[SAVE_SIZE];
SAV_Game sgame;
SAV_Bank sbank;

void saveInitialize(void)
{
	saveReadFile(save, "/rey_pokered.sav");
	saveReadData(save, &sgame);
}

void saveExit(void)
{
	saveWriteData(save, &sgame);
	saveWriteFile(save, "/rey_pokered_out.sav");
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
	pkm->species = pkmbuf[0];
	pkm->nationalDex = pokedexGetNational(pkm->species);

	// Count (1) + Species (capacity+1) + Pokémon.index (index*size)
	pkmbuf = save + 2 + capacity + index * size;

	// Shared attributes
	pkm->speciesIndex = pkmbuf[0];
	pkm->currentHP = *(u16*)(pkmbuf + 0x01);
	pkm->currentLevel = pkmbuf[0x03];
	pkm->status = pkmbuf[0x04];
	pkm->types[0] = pkmbuf[0x05];
	pkm->types[1] = pkmbuf[0x06];
	pkm->catchRate = pkmbuf[0x07];
	pkm->moves[0] = pkmbuf[0x08];
	pkm->moves[1] = pkmbuf[0x09];
	pkm->moves[2] = pkmbuf[0x0A];
	pkm->moves[3] = pkmbuf[0x0B];
	pkm->originalTID = *(u16*)(pkmbuf + 0x0C);
	pkm->experience = *(u32*)(pkmbuf + 0x0E) & 0x00FFFFFF;
	pkm->EVs[0] = *(u16*)(pkmbuf + 0x11);
	pkm->EVs[1] = *(u16*)(pkmbuf + 0x13);
	pkm->EVs[2] = *(u16*)(pkmbuf + 0x15);
	pkm->EVs[3] = *(u16*)(pkmbuf + 0x17);
	pkm->EVs[4] = *(u16*)(pkmbuf + 0x19);
	pkm->IVs[0] = 0;
	pkm->IVs[1] = pkmbuf[0x1B] & 0x0F;
	pkm->IVs[2] = pkmbuf[0x1B] >> 4;
	pkm->IVs[3] = pkmbuf[0x1C] & 0x0F;
	pkm->IVs[4] = pkmbuf[0x1C] >> 4;
	pkm->PPUps[0] = pkmbuf[0x1D] >> 6;
	pkm->PPUps[1] = pkmbuf[0x1E] >> 6;
	pkm->PPUps[2] = pkmbuf[0x1F] >> 6;
	pkm->PPUps[3] = pkmbuf[0x20] >> 6;
	pkm->PPs[0] = pkmbuf[0x1D] & 0x3F;
	pkm->PPs[1] = pkmbuf[0x1E] & 0x3F;
	pkm->PPs[2] = pkmbuf[0x1F] & 0x3F;
	pkm->PPs[3] = pkmbuf[0x20] & 0x3F;

	if (size == 0x2C)
	{
		// Party attributes
		pkm->level = pkmbuf[0x21];
		pkm->maximumHP = *(u16*)(pkmbuf + 0x22);
		pkm->attack = *(u16*)(pkmbuf + 0x24);
		pkm->defense = *(u16*)(pkmbuf + 0x26);
		pkm->speed = *(u16*)(pkmbuf + 0x28);
		pkm->special = *(u16*)(pkmbuf + 0x2A);
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
	pkmbuf[0] = pkm->species;

	// Count (1) + Species (capacity+1) + Pokémon.index (index*size)
	pkmbuf = save + 2 + capacity + index * size;

	// Shared attributes
	pkmbuf[0] = pkm->speciesIndex;
	*(u16*)(pkmbuf + 0x01) = pkm->currentHP;
	pkmbuf[0x03] = pkm->currentLevel;
	pkmbuf[0x04] = pkm->status;
	pkmbuf[0x05] = pkm->types[0];
	pkmbuf[0x06] = pkm->types[1];
	pkmbuf[0x07] = pkm->catchRate;
	pkmbuf[0x08] = pkm->moves[0];
	pkmbuf[0x09] = pkm->moves[1];
	pkmbuf[0x0A] = pkm->moves[2];
	pkmbuf[0x0B] = pkm->moves[3];
	*(u16*)(pkmbuf + 0x0C) = pkm->originalTID;
	*(u32*)(pkmbuf + 0x0E) = (*(u32*)(pkmbuf + 0x0E) & ~0x00FFFFFF) | (pkm->experience & 0x00FFFFFF);
	*(u16*)(pkmbuf + 0x11) = pkm->EVs[0];
	*(u16*)(pkmbuf + 0x13) = pkm->EVs[1];
	*(u16*)(pkmbuf + 0x15) = pkm->EVs[2];
	*(u16*)(pkmbuf + 0x17) = pkm->EVs[3];
	*(u16*)(pkmbuf + 0x19) = pkm->EVs[4];
	pkmbuf[0x1B] = (pkm->IVs[1] & 0x0F) | ((pkm->IVs[2] & 0x0F) << 4);
	pkmbuf[0x1C] = (pkm->IVs[3] & 0x0F) | ((pkm->IVs[4] & 0x0F) << 4);
	pkmbuf[0x1D] = ((pkm->PPUps[0] & 0x3) << 6) | (pkm->PPs[0] & 0x3F);
	pkmbuf[0x1E] = ((pkm->PPUps[1] & 0x3) << 6) | (pkm->PPs[1] & 0x3F);
	pkmbuf[0x1F] = ((pkm->PPUps[2] & 0x3) << 6) | (pkm->PPs[2] & 0x3F);
	pkmbuf[0x20] = ((pkm->PPUps[3] & 0x3) << 6) | (pkm->PPs[3] & 0x3F);

	if (size == 0x2C)
	{
		// Party attributes
		pkmbuf[0x21] = pkm->level;
		*(u16*)(pkmbuf + 0x22) = pkm->maximumHP;
		*(u16*)(pkmbuf + 0x24) = pkm->attack;
		*(u16*)(pkmbuf + 0x26) = pkm->defense;
		*(u16*)(pkmbuf + 0x28) = pkm->speed;
		*(u16*)(pkmbuf + 0x2A) = pkm->special;
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
	uint8_t i = 0;

	pkmList->count = save[listOffset];
	pkmList->size = size;
	pkmList->capacity = capacity;

	printf("List %u/%u (x%u)\n", pkmList->count, pkmList->capacity, pkmList->size);

	for (; i < pkmList->count; i++)
		saveExtractPokemon(save + listOffset, &pkmList->slots[i], i, pkmList->size, pkmList->capacity);
	for (; i < pkmList->capacity; i++)
		memset(&pkmList->slots[i], 0, sizeof(SAV_Pokemon));


}

/**
 * @brief Writes the data from a structure to a save.
 * @param[in/out] save The savedata buffer.
 * @param[in] pkmList The Pokémon list to inject.
 * @param listOffset The offset of the list.
 */
static void saveInjectPokemonList(uint8_t* save, const SAV_PokemonList* pkmList, SAV_PokemonListOffset listOffset)
{
	uint8_t i = 0;

	printf("List %u/%u (%u)\n", pkmList->size, pkmList->count, pkmList->capacity);

	save[0] = pkmList->count;

	for (; i < pkmList->capacity; i++)
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
	return &sgame.boxes[box];
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

void saveReadFile(uint8_t* save, const char* path)
{
	uint16_t bytesRead = 0;

	printf("Opening save file...\n");
	FILE* fp = fopen(path, "rb");
	if (fp)
	{
		printf("Reading save file...");
		bytesRead = fread(save, 1, SAVE_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %d/%d bytes\n", bytesRead, SAVE_SIZE);

		fclose(fp);
	}
}

void saveWriteFile(const uint8_t* save, const char* path)
{
	uint16_t bytesWritten = 0;

	printf("Opening save file...\n");
	FILE* fp = fopen(path, "wb");
	if (fp)
	{
		printf("Writing save file...");
		bytesWritten = fwrite(save, 1, SAVE_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Written %d/%d bytes\n", bytesWritten, SAVE_SIZE);

		fclose(fp);
	}
}

void saveReadData(const uint8_t* save, SAV_Game* sgame)
{
	sgame->boxCount = 12;

	for (uint8_t i = 0; i < sgame->boxCount; i++)
	{
		saveExtractPokemonList(save, &sgame->boxes[i], (i == saveGetCurrentBox(save) ? OFFSET_CURRENT : OFFSET_BOX_1 + (i < 6 ?  0 : 0x5B4) + i * BOX_SIZE), 0x21, 20);
	}

}

void saveWriteData(uint8_t* save, SAV_Game* sgame)
{
	for (uint8_t iB = 0; iB < sgame->boxCount; iB++)
	{
		// TODO: memalign the Pokémon in the list to avoid empty slot amongst occupied slots.

		// Iterate to find the empty slots
		for (uint8_t iP = 0, iPmin = 0; iP < sgame->boxes[iB].capacity; iP++)
		{
			// If looking for the next empty slot
			if (iPmin == 0)
			{
				printf("Looking for empty slot (%u) -> %03u (%03u)\n", iP, sgame->boxes[iB].slots[iP].species, save[0x4001+iP]);
				if (saveIsPkmEmpty(&sgame->boxes[iB].slots[iP]))
				{
					printf("New empty slot (%u)\n", iP);
					iPmin = iP+1;
				}
			}
			// If looking for the next occupied slot
			else
			{
				printf("Looking for occupied slot (%u) -> %03u (%03u)\n", iP, sgame->boxes[iB].slots[iP].species, save[0x4001+iP]);
				if (!saveIsPkmEmpty(&sgame->boxes[iB].slots[iP]))
				{
					printf("New occupied slot (%u)\n", iP);
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
		}

		saveInjectPokemonList(save, &sgame->boxes[iB], (iB == saveGetCurrentBox(save) ? OFFSET_CURRENT : OFFSET_BOX_1 + (iB < 6 ?  0 : 0x5B4) + iB * BOX_SIZE));
	}
}

void saveFixChecksum(uint8_t* save)
{
	uint8_t chk = 0;

	for (uint16_t off = 0x2598; off < 0x3522; off++)
	{
		chk += save[off];
	}

	save[0x3523] = ~chk;
}
