#pragma once

#include "font.h"

#define SAVE_SIZE (0x8000) // 0x7A4C
#define BANK_SIZE (0x8E00) // 0x8D28
#define BOX_SIZE(c,sp,sn) (2+c*(1+sp+sn*2)) // (1+c+1+c*sp+c*sn+c*sn)
#define POKEMON_LIST_MAX_COUNT (32) // Party: 6 | Box: 20 | BoxJP: 30 | BoxBK: 32
#define GAME_BOX_MAX_COUNT (12) // 12 | JP: 8
#define BANK_BOX_MAX_COUNT (20) // 20

#define NOT_POKEMON (0)

/// 
typedef enum
{
	POKEMON_RED = 1,	///< Pokémon Red
	POKEMON_GREEN = 2,	///< Pokémon Green
	POKEMON_BLUE = 3,	///< Pokémon Blue
	POKEMON_YELLOW = 4,	///< Pokémon Yellow
} SAV_GameVersion;

/// 
typedef enum
{
	POKEMON_JP = 1,	///< Japanese
	POKEMON_EN = 2,	///< English
	POKEMON_FR = 3,	///< French
	POKEMON_DE = 4,	///< German
	POKEMON_IT = 5,	///< Italian
	POKEMON_ES = 6,	///< Spanish

	POKEMON_JAP = 0xFE,	///< Japanese Pokémon
	POKEMON_ALL = 0xFF,	///< International Pokémon
} SAV_GameLang;

/// 
typedef enum
{
	OPT_SOUND_MONO = 0x0,
	OPT_SOUND_STEREO = BIT(4),
	OPT_SOUND_EARPHONE1 = BIT(4),
	OPT_SOUND_EARPHONE2 = BIT(5),
	OPT_SOUND_EARPHONE3 = BIT(4) | BIT(5),
	OPT_BATTLE_STYLE_SWITCH = 0x0,
	OPT_BATTLE_STYLE_SET = BIT(6),
	OPT_BATTLE_EFFECT_YES = 0x0,
	OPT_BATTLE_EFFECT_NO = BIT(7),
} SAV_Option;

/// 
typedef enum
{
	STATUS_OK = 0,				///< OK
	STATUS_KO = BIT(0),			///< KO (Custom)
	STATUS_ASLEEP = BIT(2),		///< Asleep status
	STATUS_POISONED = BIT(3),	///< Poisoned status
	STATUS_BURNED = BIT(4),		///< Burned status
	STATUS_FROZEN = BIT(5),		///< Frozen status
	STATUS_PARALYZED = BIT(6),	///< Paralyzed status
} SAV_Status;

/// 
typedef enum
{
	TYPE_NORMAL = 0x0,		///< Normal type
	TYPE_FIGHTING = 0x1,	///< Fighting type
	TYPE_FLYING = 0x2,		///< Flying type
	TYPE_POISON = 0x3,		///< Poison type
	TYPE_GROUND = 0x4,		///< Ground type
	TYPE_ROCK = 0x5,		///< Rock type
	// TYPE_BIRD = 0x6,		///< /UNUSED
	TYPE_BUG = 0x7,			///< Bug type
	TYPE_GHOST = 0x8,		///< Ghost type
	// TYPE_NORMAL = 0x9,	///< /UNUSED
	// TYPE_NORMAL = 0xA,	///< /UNUSED
	// TYPE_NORMAL = 0xB,	///< /UNUSED
	// TYPE_NORMAL = 0xC,	///< /UNUSED
	// TYPE_NORMAL = 0xD,	///< /UNUSED
	// TYPE_NORMAL = 0xE,	///< /UNUSED
	// TYPE_NORMAL = 0xF,	///< /UNUSED
	// TYPE_NORMAL = 0x10,	///< /UNUSED
	// TYPE_NORMAL = 0x11,	///< /UNUSED
	// TYPE_NORMAL = 0x12,	///< /UNUSED
	// TYPE_NORMAL = 0x13,	///< /UNUSED
	TYPE_FIRE = 0x14,		///< Fire type
	TYPE_WATER = 0x15,		///< Water type
	TYPE_GRASS = 0x16,		///< Grass type
	TYPE_ELECTRIC = 0x17,	///< Electric type
	TYPE_PSYCHIC = 0x18,	///< Psychic type
	TYPE_ICE = 0x19,		///< Ice type
	TYPE_DRAGON = 0x1A,		///< Dragon type
} SAV_Type;

/// 
typedef enum
{
	STAT_HP = 0,		///< The hit points stat
	STAT_ATTACK = 1,	///< The attack stat
	STAT_DEFENSE = 2,	///< The defense stat
	STAT_SPEED = 3,		///< The speed stat
	STAT_SPECIAL = 4,	///< The special stat
	STAT_ATK = STAT_ATTACK,
	STAT_DEF = STAT_DEFENSE,
	STAT_SPE = STAT_SPEED,
	STAT_SPC = STAT_SPECIAL,
} SAV_Stat;

/// 
typedef struct
{
	// Memory pointer
	uint8_t* pk1;	///< UNUSED / Pk1 structure pointer
	uint8_t* ot1;	///< UNUSED / OT name pointer
	uint8_t* nk1;	///< UNUSED / NK name pointer
	bool moved;

	// Shared attributes
	uint8_t speciesIndex;
	uint16_t currentHP;
	uint8_t currentLevel;
	uint8_t status;
	uint8_t types[2];
	uint8_t catchRate;
	uint8_t moves[4];
	uint16_t originalTID;
	uint32_t experience;
	uint16_t EVs[5];
	uint8_t IVs[5];
	uint8_t PPUps[4];
	uint8_t PPs[4];

	// Party attributes
	uint8_t level;
	uint16_t maxHP;	///< HitPoint max
	uint16_t ATK;	///< Attack
	uint16_t DEF;	///< Defense
	uint16_t SPE;	///< Speed
	uint16_t SPC;	///< Special

	// Extra attributes
	uint8_t species;		///< Species from list
	char8_t nameOT[11];		///< OT name from list
	char8_t nameNK[11];		///< Nickname from list
	uint8_t nationalDex;	///< National Pokédex
} SAV_Pokemon;

/// 
typedef struct
{
	uint8_t count;		///< The count of Pokémon (<= capacity)
	uint8_t species[POKEMON_LIST_MAX_COUNT];
	SAV_Pokemon slots[POKEMON_LIST_MAX_COUNT];
	// OT/NK names are stored in SAV_Pokemon

	// Extra attributes
	uint8_t index;		///< The index of the box
	uint8_t sizePkm;	///< The size of a Pokémon struct (pk1)
	uint8_t sizeName;	///< The size of a name.
	uint8_t capacity;	///< The max size of the list (<= POKEMON_LIST_MAX_COUNT)
	char8_t title[11];	///< An emulated title box
} SAV_PokemonList;

/// 
typedef struct
{
	uint8_t nameSize;
	uint8_t boxCount;
	uint8_t boxCapacity;
	SAV_PokemonList boxes[GAME_BOX_MAX_COUNT];
} SAV_Game;

/// 
typedef struct
{
	uint32_t magic;
	uint32_t version;
	uint8_t boxCount;
	uint8_t boxCapacity;
	SAV_PokemonList boxes[BANK_BOX_MAX_COUNT];
} SAV_Bank;

extern uint64_t titleid;
extern uint8_t save[];
extern uint8_t bank[];
extern SAV_Game sgame;
extern SAV_Bank sbank;
extern SAV_GameVersion vgame;
extern SAV_GameLang lgame;

/**
 * @brief Loads and initializes the save module.
 */
Result saveLoad(void);

/**
 * @brief Exits the save module.
 */
void saveExit(void);

/**
 * @brief Gets the game version of a game.
 * @param titleid The title id of the game..
 * @return The game version.
 */
SAV_GameVersion saveGetGameVersion(uint64_t titleid);

/**
 * @brief Gets the game lang of a game.
 * @param titleid The title id of the game..
 * @return The game lang.
 */
SAV_GameLang saveGetGameLang(uint64_t titleid);

/**
 * @brief Gets a box from a list.
 * @param[in] save The savedata buffer.
 * @param box The id of the box.
 * @param inBank Whether the box is in bank.
 * @return The Pokémon list.
 */
SAV_PokemonList* saveGetBox(uint8_t box, bool inBank);

/**
 * @brief Gets a Pokémon from a list.
 * @param[in] save The savedata buffer.
 * @param box The id of the box.
 * @param slot The id of the slot.
 * @param inBank Whether the box is in bank.
 * @return The Pokémon.
 */
SAV_Pokemon* saveGetPkm(uint8_t box, uint8_t slot, bool inBank);

/**
 * @brief Swaps two Pokémon.
 * @param src The Pokémon source.
 * @param dst The Pokémon destination.
 * @param srcBanked Whether the source is in bank.
 * @param dstBanked Whether the destination is in bank.
 * @return Whether the Pokémon have been swapped.
 */
bool saveMovePkm(SAV_Pokemon* src, SAV_Pokemon* dst, bool srcBanked, bool dstBanked);

/**
 * @brief Pastes a Pokémon over another.
 * @param src The Pokémon source.
 * @param dst The Pokémon destination.
 * @param srcBanked Whether the source is in bank.
 * @param dstBanked Whether the destination is in bank.
 * @return Whether the Pokémon has been pasted.
 */
bool savePastePkm(SAV_Pokemon* src, SAV_Pokemon* dst, bool srcBanked, bool dstBanked);

/**
 * @briefs Checks if a Pokémon is empty.
 * @param pkm The Pokémon.
 * @return Whether the Pokémon is empty.
 */
bool saveIsPkmEmpty(const SAV_Pokemon* pkm);

/**
 * @todo jp
 */
uint8_t getBoxCount(bool inBank);

/**
 * @brief Gets the name of the game's trainer.
 * @return The trainer's name as string8_t.
 */
const char8_t* saveGetTrainer(void);

/**
 * @brief Reads a save file to a save buffer.
 * @param[out] save The savedata buffer.
 * @param[in] path The path of the save file.
 */
Result saveReadFile(uint8_t* save, const char* path);

/**
 * @brief Writes a save buffer to a save file.
 * @param[in] save The savedata buffer.
 * @param[in] path The path of the save file.
 */
Result saveWriteFile(const uint8_t* save, const char* path);

/**
 * @brief Reads a save buffer to a save struct.
 * @param[in] save The savedata buffer.
 * @param[out] sgame The path of the save file.
 */
void saveReadData(const uint8_t* save, SAV_Game* sgame);

/**
 * @brief Writes a save struct to a save buffer.
 * @param[in/out] save The savedata buffer.
 * @param[in] path The path of the save file.
 */
void saveWriteData(uint8_t* save, SAV_Game* sgame);

/**
 * @brief Fixs the checksum of a save.
 * @param[in/out] save The savedata buffer.
 */
void saveFixChecksum(uint8_t* save);

/**
 * @brief Reads a bank file to a bank buffer.
 * @param[out] bank The bankdata buffer.
 * @param[in] path The path of the bank file.
 * @return The total of read bytes.
 */
uint16_t bankReadFile(uint8_t* bank, const char* path);

/**
 * @brief Writes a bank buffer to a bank file.
 * @param[in] bank The bankdata buffer.
 * @param[in] path The path of the bank file.
 * @return The total of written bytes.
 */
uint16_t bankWriteFile(const uint8_t* bank, const char* path);

/**
 * @brief Reads a bank buffer to a bank struct.
 * @param[in] bank The bankdata buffer.
 * @param[in] path The path of the bank file.
 * @param bytesRead The total of read bytes.
 */
void bankReadData(uint8_t* bank, SAV_Bank* sbank, uint16_t bytesRead);

/**
 * @brief Writes a bank struct to a bank buffer.
 * @param[in/out] bank The bankdata buffer.
 * @param[in] path The path of the bank file.
 */
void bankWriteData(uint8_t* bank, SAV_Bank* sbank);
