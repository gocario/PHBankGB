#include "box_viewer.h"

#include "key.h"
#include "gfx.h"
#include "font.h"
#include "data.h"
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

			if (cursor.box->box < 0) cursor.box->box = (cursor.box->inBank ? sbank.boxCount : sgame.boxCount)-1;
			else if (cursor.box->box > (cursor.box->inBank ? sbank.boxCount : sgame.boxCount)-1) cursor.box->box = 0;

			if (cursor.box->col < 0)
			{
				uint8_t row = cursor.box->row;
				cursor.box->col = 0;
				boxSwapBox();
				cursor.box->row = row;
				cursor.box->col = boxGetWidth(cursor.box->row)-1;

				if (cursor.box->row > boxGetHeight()-1) cursor.box->row = boxGetHeight()-1;

			}
			else if (cursor.box->col > boxGetWidth(cursor.box->row)-1)
			{
				uint8_t row = cursor.box->row;
				cursor.box->col = boxGetWidth(cursor.box->row)-1;
				boxSwapBox();
				cursor.box->row = row;
				cursor.box->col = 0;

				if (cursor.box->row > boxGetHeight()-1) cursor.box->row = boxGetHeight()-1;
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
	gfxDrawFrame(0, 0);

	gfxDrawPanel(48 + 8, 40 + 64, 18, 10);
	gfxDrawFolio(224, 112, 13, 10);
	gfxDrawFolio(224, 40, 13, 8);

	if ((cursor.vPkm && !saveIsPkmEmpty(cursor.vPkm)) || cursor.sPkm)
	{
		SAV_Pokemon* pkm = (cursor.vPkm && !saveIsPkmEmpty(cursor.vPkm) ? cursor.vPkm : cursor.sPkm);

		gfxDrawPanel(56, 40, 6, 6);
		boxDrawPokemon(pkm, 56 + 24, 40 + 24);
		fontDrawChar8(68, 88, CHAR8_NO);
		fontDrawChar8(76, 88, CHAR8_DOT);
		fontDrawPaddingUInt8(84, 88, pkm->nationalDex);

		fontDrawString8(128, 48, pkm->nameNK);

		/** Status **/
		fontDrawString8(128, 60, dataText(TEXT_STATUS));
		if (pkm->currentHP == 0)
		{
			fontDrawString8(184, 60, dataStatus(STATUS_KO));
		}
		else
		{
			uint8_t status;

			switch (pkm->status)
			{
				case STATUS_ASLEEP: status = 2; break;
				case  STATUS_POISONED: status = 3; break;
				case STATUS_BURNED: status = 4; break;
				case STATUS_FROZEN: status = 5; break;
				case STATUS_PARALYZED: status = 6; break;
				default: status = 0; break;
			}

			fontDrawString8(184, 60, dataStatus(status));
		}

		/** OTrainer **/
		fontDrawString8(128, 72, dataText(TEXT_OT));
		fontDrawString8(144, 80, pkm->nameOT);
		fontDrawChar8(128, 88, CHAR8_ID);
		fontDrawChar8(136, 88, CHAR8_NO);
		fontDrawChar8(144, 88, CHAR8_SLASH);
		fontDrawPaddingUInt16(144, 96, pkm->originalTID);
		// TODO: Draw the TID!

		/** Life Bar **/
		gfxDrawLevel(272, 44, pkm->level);
		gfxDrawLifeBar(248, 54, pkm->currentHP, pkm->maxHP, false);

		/** Types **/
		fontDrawString8(232, 72, dataText(TEXT_TYPE1));
		fontDrawString8(248, 80, dataType(pkm->types[0]));
		// TODO: Read types from a personal file?
		if (pkm->types[0] != pkm->types[1])
		{
			fontDrawString8(232, 88, dataText(TEXT_TYPE2));
			fontDrawString8(248, 96, dataType(pkm->types[1]));
		}

		/** Moves **/
		fontDrawString8(224, 112, dataText(TEXT_MOVE));
		for (uint8_t i = 0; i < 4; i++)
		{
			if (pkm->moves[i] != 0)
			{
				fontDrawString8(240, 128+i*16, dataMove(pkm->moves[i]));
				fontDrawFromRightUInt32(304, 136+i*16, pkm->PPs[i]);
				fontDrawChar8(312, 136+i*16, CHAR8_SLASH);
				fontDrawFromRightUInt32(328, 136+i*16, pkm->PPs[i]); // TODO: PersonalMove
			}
		}

		/** Stats **/
		fontDrawString8(64, 112, dataText(TEXT_HP));
		fontDrawString8(64, 128, dataText(TEXT_ATTACK));
		fontDrawString8(64, 144, dataText(TEXT_DEFENSE));
		fontDrawString8(64, 160, dataText(TEXT_SPEED));
		fontDrawString8(64, 176, dataText(TEXT_SPECIAL));
		fontDrawString8(136, 112, dataText(TEXT_IV));
		fontDrawString8(176, 112, dataText(TEXT_EV));
		// Stats
		fontDrawFromRightUInt32(104+16, 120, pkm->maxHP);
		fontDrawFromRightUInt32(104+16, 136, pkm->ATK);
		fontDrawFromRightUInt32(104+16, 152, pkm->DEF);
		fontDrawFromRightUInt32(104+16, 168, pkm->SPE);
		fontDrawFromRightUInt32(104+16, 184, pkm->SPC);
		// IVs
		fontDrawFromRightUInt32(136+16, 120, pkm->IVs[STAT_HP]);
		fontDrawFromRightUInt32(136+16, 136, pkm->IVs[STAT_ATTACK]);
		fontDrawFromRightUInt32(136+16, 152, pkm->IVs[STAT_DEFENSE]);
		fontDrawFromRightUInt32(136+16, 168, pkm->IVs[STAT_SPEED]);
		fontDrawFromRightUInt32(136+16, 184, pkm->IVs[STAT_SPECIAL]);
		// EVs
		fontDrawFromRightUInt32(160+40, 120, pkm->EVs[STAT_HP]);
		fontDrawFromRightUInt32(160+40, 136, pkm->EVs[STAT_ATTACK]);
		fontDrawFromRightUInt32(160+40, 152, pkm->EVs[STAT_DEFENSE]);
		fontDrawFromRightUInt32(160+40, 168, pkm->EVs[STAT_SPEED]);
		fontDrawFromRightUInt32(160+40, 184, pkm->EVs[STAT_SPECIAL]);
	}
}

static void boxViewerDrawBottom(void)
{
	// fontDrawString8(200, 30, saveGetTrainer());

	boxDrawBox(&cursor.pc, 0, 0);
	boxDrawBox(&cursor.bk, 176, 0);

	if (cursor.sPkm)
	{
		boxDrawPokemon(cursor.sPkm, 152, 8);
	}
}

void boxViewerDraw(void)
{
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		boxViewerDrawTop();
	sf2d_end_frame();
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		boxViewerDrawBottom();
	sf2d_end_frame();
	sf2d_swapbuffers();
}

static void boxDrawBox(CursorInbox* cursorin, int16_t x, int16_t y)
{
	gfxDrawPanel(x, y, 16, 2);
	gfxDrawPanel(x, y + 32, 16, 24);

	fontDrawString8(x + 32, y + 12, cursorin->vBox->title);

	for (uint8_t i = 0; i < cursorin->vBox->capacity; i++)
	{
		if (&cursorin->vBox->slots[i] != cursor.sPkm)
			boxDrawPokemon(&cursorin->vBox->slots[i], x + 20 + 32 * (i % 4), y + 44 + 24 * (i / 4));
	}

	if (cursorin == cursor.box) gfxDrawFullArrow(x + 8 + 32 * cursorin->col, y + 48 + 24 * cursorin->row);
}

static void boxDrawPokemon(SAV_Pokemon* pkm, int16_t x, int16_t y)
{
	if (saveIsPkmEmpty(pkm)) return;

	gfxDrawPokemonIcon(x, y, pkm->nationalDex, (pkm == cursor.vPkm && (osGetTime() / 250) % 2 ? GFX_FRAME_1 : GFX_FRAME_0));
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