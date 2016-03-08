#pragma once

#include <sf2d.h>

#include "pokedex.h"

/// Unused
typedef enum
{
	PKM_ICON_BIPED = 0,		///< SPRITE_MON       Pokémon icon (2 frames) 
	PKM_ICON_BALL = 1,		///< SPRITE_BALL_M    Ball pkm icon (1 frame) 
	PKM_ICON_FOSSIL = 2,	///< SPRITE_HELIX     Fossil icon (1 frame)
	PKM_ICON_FAIRY = 3,		///< SPRITE_FAIRY     Fairy icon (2 frames)
	PKM_ICON_BIRD = 4,		///< SPRITE_WATER     Bird icon (2 frames)
	PKM_ICON_WATER = 5,		///< SPRITE_WATER     Water icon (2 frames)
	PKM_ICON_BUG = 6,		///< SPRITE_BUG       Insect icon (2 frames)
	PKM_ICON_PLANT = 7,		///< SPRITE_GRASS     Plant icon (2 frames) 
	PKM_ICON_SNAKE = 8,		///< SPRITE_SNAKE     Snake icon (2 frames) 
	PKM_ICON_QUADRUPED = 9,	///< SPRITE_QUADRUPED Quadruped icon (2 frames) 
	PKM_ICON_MISSINGNO = 0,	///<                  Missingno icon (0 frames)
} GFX_PkmIcon;

/// 
typedef enum
{
	GFX_FRAME_0 = 0,	///< First frame.
	GFX_FRAME_1 = 1,	///< Second frame.
} GFX_Frame;

/**
 * @brief Loads and initializes the gfx module.
 */
Result gfxLoad(void);

/**
 * @brief Loads the gb frame of the game version.
 */
Result gfxLoadFrame(uint64_t titleid);

/**
 * @brief Frees and exits the gfx module.
 */
void gfxFree(void);

/**
 * @brief Frees the gb frame of the game version.
 */
void gfxFreeFrame(void);

/**
 * @brief Draws a Pokémon icon.
 * @param x The x position.
 * @param y The y position.
 * @param icon The gfx icon to draw.
 * @param frame The gfx frame to draw.
 */
void gfxDrawPokemonIcon(int16_t x, int16_t y, DEX_Species icon, GFX_Frame frame);

/**
 * @todo
 */
void gfxDrawFrame(int16_t x, int16_t y);

/**
 * @todo
 */
void gfxDrawPanel(int16_t x, int16_t y, uint8_t w, uint8_t h);

/**
 * @todo
 */
void gfxDrawEmptyArrow(int16_t x, int16_t y);

/**
 * @todo
 */
void gfxDrawFullArrow(int16_t x, int16_t y);
