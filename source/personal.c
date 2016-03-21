#include "personal.h"
#include "pkdir.h"
#include "data.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI_SIZE (0x6+1)
#define PMI_SIZE (0x3+1)
#define PERSONAL_SIZE (PI_SIZE*(PKM_COUNT+1))
#define PERSONAL_MOVE_SIZE (PMI_SIZE*(MOVE_COUNT))

static PersonalInfo personalInfos[PKM_COUNT+1];
static PersonalMoveInfo personalMoveInfos[MOVE_COUNT];

static PersonalInfo* PersonalInfoImport(PersonalInfo* pInfo, const uint8_t* buf)
{
	if (!buf) return NULL;

	memcpy(pInfo, buf, sizeof(PersonalInfo));

	return pInfo;
}

static PersonalMoveInfo* PersonalMoveInfoImport(PersonalMoveInfo* pInfo, const uint8_t* buf)
{
	if (!buf) return NULL;

	memcpy(pInfo, buf, sizeof(PersonalMoveInfo));

	return pInfo;
}

static void PersonalImport(const uint8_t* buf)
{
	for (uint8_t i = 0; i < PKM_COUNT+1; i++)
	{
		PersonalInfoImport(&personalInfos[i], buf+PI_SIZE*i);
	}
}

static void PersonalMoveImport(const uint8_t* buf)
{
	for (uint8_t i = 0; i < MOVE_COUNT; i++)
	{
		PersonalMoveInfoImport(&personalMoveInfos[i], buf+PMI_SIZE*i);
	}
}

Result PersonalLoad(void)
{
	FILE* fp;
	Result ret = -1;
	uint16_t bytesRead = 0;

	printf("Opening personal file...\n");
	fp = fopen(DATA_FOLDER "personal_gb", "rb");
	if (fp)
	{
		uint8_t* buf = (uint8_t*) malloc(PERSONAL_SIZE);

		printf("Reading personal file...");
		bytesRead = fread(buf, 1, PERSONAL_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %d/%d bytes\n", bytesRead, PERSONAL_SIZE);

		ret = !ferror(fp);
		fclose(fp);

		PersonalImport(buf);

		free(buf);
	}

	return ret; // TODO: Remove when personal_move_gb file

	printf("Opening personal file...\n");
	fp = fopen(DATA_FOLDER "personal_move_gb", "rb");
	if (fp)
	{
		uint8_t* buf = (uint8_t*) malloc(PERSONAL_MOVE_SIZE);

		printf("Reading personal file...");
		bytesRead = fread(buf, 1, PERSONAL_MOVE_SIZE, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %d/%d bytes\n", bytesRead, PERSONAL_MOVE_SIZE);

		ret = !ferror(fp);
		fclose(fp);

		PersonalMoveImport(buf);

		free(buf);
	}

	return ret;
}

const PersonalInfo* Personal(DEX_Species species)
{
	return &personalInfos[species > SPECIES_MISSINGNO && species <= SPECIES_MEW ? species : SPECIES_MISSINGNO];
}

const PersonalMoveInfo* PersonalMove(uint8_t move)
{
	return &personalMoveInfos[move > 0 && move < 165 ? move : 0];
}