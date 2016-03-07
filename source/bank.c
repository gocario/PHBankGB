#include "bank.h"
#include "version.h"

#include <stdio.h>

static inline uint32_t MakeMagic(char a, char b, char c, char d)
{
	return (a | b << 8 | c << 16 | d << 24);
}

bool bankUpdate(uint8_t* bank, uint16_t bytesRead)
{
	// It can't even read the magic/version
	if (bytesRead < 0x10) return false;

	uint32_t version = *(uint32_t*)(bank + 0x4);

	printf("BankGB code version: %8x\n", VERSION);
	printf("BankGB file version: %8lx\n", version);

	// If the version is already the current
	if (version == VERSION) return true;

	// No bank file
	if (version == 0x00000000)
	{
		printf("\a0x00000000->0x010001A0\n");

		*(uint32_t*)(bank + 0x00) = MakeMagic('B', 'K', 'G', 'B');
		*(uint32_t*)(bank + 0x04) = version = VERSION;
		printf("\e"); // It is really needed else it will freeze
	}

	// Current version
	if (version == 0x010001A0)
	{
		printf("\a0x010001A0\n");
		return true;
	}

	// It didn't reach the current version
	return false;
}
