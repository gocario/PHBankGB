#include "data.h"

const char8_t text_text[TEXT_COUNT][TEXT_LENGTH] =
{
	{},
	{ 0x8E, 0x93, 0x50 }, // OT
	{ 0x88, 0x95, 0x92, 0x50 }, // IVS
	{ 0x84, 0x95, 0x92, 0x50 }, // EVS
	{ 0x87, 0x8F, 0x50 }, // HP
	{ 0x80, 0x93, 0x93, 0x80, 0x82, 0x8A, 0x50 }, // ATTACK
	{ 0x83, 0x84, 0x85, 0x84, 0x8D, 0x92, 0x84, 0x50 }, // DEFENSE
	{ 0x92, 0x8F, 0x84, 0x84, 0x83, 0x50 }, // SPEED
	{ 0x92, 0x8F, 0x84, 0x82, 0x88, 0x80, 0x8B, 0x50 }, // SPECIAL
	{ 0x92, 0x93, 0x80, 0x93, 0x94, 0x92, 0x50 }, // STATUS
	{ 0x93, 0x98, 0x8F, 0x84, 0xF7, 0x50 }, // TYPE1
	{ 0x93, 0x98, 0x8F, 0x84, 0xF8, 0x50 }, // TYPE2
	{ 0x8C, 0x8E, 0x95, 0x84, 0x50 }, // MOVE
	{ 0x8F, 0x8F, 0x50 }, // PP
};
