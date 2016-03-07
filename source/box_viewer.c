#include "box_viewer.h"

#include "key.h"
#include "gfx.h"
#include "font.h"
#include "save.h"

#include <stdio.h>

#define BOX_ROW_PKM_COUNT (8)
#define BOX_COL_PKM_COUNT (4)
#define BOX_PKM_COUNT (BOX_ROW_PKM_COUNT*BOX_PKM_COUNT)
#define BOX_HEADER_SELECTED (0) // TODO: -1

typedef struct 
{
	bool inBank : 1;
	int8_t box : 7;
	int8_t row;
	int8_t col;
} CursorSlot;

typedef struct
{
	bool inBank : 1;
	int8_t box : 7;
	int8_t row;
	int8_t col;
	int8_t slot;
	SAV_PokemonList* vBox;
} CursorInbox;

typedef struct
{
	CursorSlot sSlot;
	CursorInbox pc;
	CursorInbox bk;
	CursorInbox* box;
	SAV_Pokemon* vPkm;
	SAV_Pokemon* sPkm;
} CursorBox;

static CursorBox cursor;

static void computeSlot(CursorInbox* cursorin)
{
	cursorin->slot = cursorin->row * BOX_COL_PKM_COUNT + cursorin->col;
}

static void extractSlot(CursorInbox* cursorin, CursorSlot* cursorot)
{
	cursorot->inBank = cursorin->inBank;
	cursorot->box = cursorin->box;
	cursorot->row = cursorin->row;
	cursorot->col = cursorin->col;
}

// static void injectSlot(CursorInbox* cursorin, CursorSlot* cursorot)
// {
// 	cursorin->inBank = cursorot->inBank;
// 	cursorin->box = cursorot->box;
// 	cursorin->row = cursorot->row;
// 	cursorin->col = cursorot->col;
// }

static void boxViewerDrawTop(void);
static void boxViewerDrawBottom(void);

static void boxDrawBox(CursorInbox* cursor, int16_t x, int16_t y);
static void boxDrawPokemon(SAV_Pokemon* pkm, int16_t x, int16_t y);
static void boxSelectViewBox(void);
static void boxSelectViewPkm(void);
static void boxSelectMovePokemon(void);
static void boxCancelMovePokemon(void);
static void boxSwapBox(void);
static uint8_t boxGetHeight(void);
static uint8_t boxGetWidth(uint8_t row);

void boxViewerInitialize(void)
{
	cursor.pc.inBank = false;
	cursor.pc.box = 0;
	cursor.pc.row = 0;
	cursor.pc.col = 0;
	cursor.pc.slot = 0;
	cursor.bk.inBank = true;
	cursor.bk.box = 0;
	cursor.bk.row = 0;
	cursor.bk.col = 0;
	cursor.bk.slot = 0;

	cursor.sPkm = NULL;

	cursor.box = &cursor.bk; boxSelectViewBox();
	cursor.box = &cursor.pc; boxSelectViewBox();
	boxSelectViewPkm();

	sf2d_set_clear_color(RGBA8(0xF8,0xF8,0xF8,0xFF));
}

void boxViewerUpdate(void)
{
	uint32_t kDown = hidKeysDown();
	// uint32_t kHeld = hidKeysHeld();

	if (kDown)
	{
		bool boolMod = false;
		int8_t boxMod = 0;
		int8_t rowMod = 0;
		int8_t colMod = 0;

		if (kDown & KEY_L) boxMod--;
		else if (kDown & KEY_R) boxMod++;

		if (kDown & KEY_UP) rowMod--;
		else if (kDown & KEY_DOWN) rowMod++;

		if (kDown & KEY_LEFT) colMod--;
		else if (kDown & KEY_RIGHT) colMod++;

		if (boxMod || rowMod || colMod)
		{
			cursor.box->box += boxMod;
			cursor.box->row += rowMod;
			cursor.box->col += colMod;

			if (cursor.box->box < 0) cursor.box->box = sgame.boxCount-1;
			else if (cursor.box->box > sgame.boxCount-1) cursor.box->box = 0;

			if (cursor.box->col < 0)
			{
				uint8_t row = cursor.box->row;
				cursor.box->col = 0;
				boxSwapBox();
				cursor.box->row = row;
				cursor.box->col = boxGetWidth(cursor.box->row)-1;
			}
			else if (cursor.box->col > boxGetWidth(cursor.box->row)-1)
			{
				uint8_t row = cursor.box->row;
				cursor.box->col = boxGetWidth(cursor.box->row)-1;
				boxSwapBox();
				cursor.box->row = row;
				cursor.box->col = 0;
			}

			if (cursor.box->row < BOX_HEADER_SELECTED) cursor.box->row = boxGetHeight()-1;
			else if (cursor.box->row > boxGetHeight()-1) cursor.box->row = BOX_HEADER_SELECTED;

			boolMod = true;
		}

		if (boolMod)
		{
			computeSlot(cursor.box);

			boxSelectViewBox();

			// if (cursor.box->slot > cursor.box->vBox->capacity-1)
			// {
			// 	cursor.box->row = (cursor.box->vBox->capacity-1) / BOX_COL_PKM_COUNT;
			// 	cursor.box->col = (cursor.box->vBox->capacity-1) % BOX_COL_PKM_COUNT;
			// 	computeSlot(cursor.box);
			// }

			boxSelectViewPkm();
		}
	}

	if (kDown)
	{
		{
			if (kDown & KEY_A)
			{
				boxSelectMovePokemon();
			}

			if (kDown & KEY_B)
			{
				boxCancelMovePokemon();
			}
		}

		if (kDown & KEY_Y)
		{
			printf("CursorInbox:\n");
			printf(" inBank: %i\n", cursor.box->inBank);
			printf(" box: %i\n", cursor.box->box);
			printf(" row: %i\n", cursor.box->row);
			printf(" col: %i\n", cursor.box->col);
			printf(" slot: %i\n", cursor.box->slot);
			printf(" vBox: %u\n", cursor.box->vBox->count);
			printf(" boxGetHeight: %u\n", boxGetHeight());
			printf(" boxGetWidth: %u\n", boxGetWidth(cursor.box->row));
		}
	}
}

static void boxViewerDrawTop(void)
{

}

static void boxViewerDrawBottom(void)
{
	// fontDrawString8(200, 30, saveGetTrainer());

	boxDrawBox(&cursor.pc, 0, 0);
	boxDrawBox(&cursor.bk, 176, 0);

	if (cursor.vPkm && !saveIsPkmEmpty(cursor.vPkm))
	{
		// fontDrawString8(200, 50, cursor.vPkm->nameOT);
		// fontDrawString8(200, 70, cursor.vPkm->nameNK);
	}
}

void boxViewerDraw(void)
{
	// sf2d_start_frame(GFX_TOP, GFX_LEFT);
		boxViewerDrawTop();
	// sf2d_end_frame();
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		boxViewerDrawBottom();
	sf2d_end_frame();
	sf2d_swapbuffers();
}

static void boxDrawBox(CursorInbox* cursorin, int16_t x, int16_t y)
{
	gfxDrawPanel(x, y, 2, 16);
	gfxDrawPanel(x, y + 32, 24, 16);

	for (uint8_t i = 0; i < POKEMON_LIST_MAX_COUNT; i++)
	{
		boxDrawPokemon(&cursorin->vBox->slots[i], x + 20 + 32 * (i % 4), y + 44 + 24 * (i / 4));
	}

	if (cursorin == cursor.box) gfxDrawFullArrow(x + 8 + 32 * cursorin->col, y + 48 + 24 * cursorin->row);
	else gfxDrawEmptyArrow(x + 8 + 32 * cursorin->col, y + 48 + 24 * cursorin->row);
}

static void boxDrawPokemon(SAV_Pokemon* pkm, int16_t x, int16_t y)
{
	if (saveIsPkmEmpty(pkm)) return;

	gfxDrawPokemonIcon(x, y, pkm->nationalDex, (pkm == cursor.vPkm && (osGetTime() / 500) % 2 ? GFX_FRAME_1 : GFX_FRAME_0));
}

static void boxSelectViewBox(void)
{
	computeSlot(cursor.box);

	cursor.box->vBox = saveGetBox(cursor.box->box, cursor.box->inBank);
}

static void boxSelectViewPkm(void)
{
	computeSlot(cursor.box);

	cursor.vPkm = saveGetPkm(cursor.box->box, cursor.box->slot, cursor.box->inBank);
}

static void boxSelectMovePokemon(void)
{
	computeSlot(cursor.box);

	// If no Pokémon currently selected
	if (!cursor.sPkm)
	{
		// If the current Pokémon slot isn't empty (to avoid empty slot move)
		if (!saveIsPkmEmpty(cursor.vPkm))
		{
			// Select the current Pokémon
			cursor.sPkm = cursor.vPkm;
			extractSlot(cursor.box, &cursor.sSlot);

			// If the buttons are used
			// if (cursor.isPkmDragged)
			// {
			// 	cursor.isPkmHeld = true;
			// }
		}
	}
	// Else if there is a current Pokémon
	else if (cursor.vPkm)
	{
		// If the selected Pokémon isn't the current Pokémon
		if (cursor.sPkm != cursor.vPkm)
		{
			// Swap the selected Pokémon with the current Pokémon
			bool moved = saveMovePkm(cursor.sPkm, cursor.vPkm, cursor.sSlot.inBank, cursor.box->inBank);

			// If the Pokémon had moved
			if (moved)
			{
				boxCancelMovePokemon();
			}

			// Populate the Pokémon data
			// boxPopulatePokemon(cursor.vPkm);
		}
		else
		{
			// Cancel the selection, since it's moved to the same slot
			boxCancelMovePokemon();
		}
	}
}

static void boxCancelMovePokemon(void)
{
	cursor.sPkm = NULL;
}

static void boxSwapBox(void)
{
	if (cursor.box == &cursor.pc)
		cursor.box = &cursor.bk;
	else if (cursor.box == &cursor.bk)
		cursor.box = &cursor.pc;
	computeSlot(cursor.box);
}

static uint8_t boxGetHeight(void)
{
	return (cursor.box->vBox->capacity-1) / BOX_COL_PKM_COUNT + 1;
}

static uint8_t boxGetWidth(uint8_t row)
{
	return (row == boxGetHeight()-1 ? (cursor.box->vBox->capacity-1) % BOX_COL_PKM_COUNT + 1 : BOX_COL_PKM_COUNT);
}