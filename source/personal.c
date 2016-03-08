#include "personal.h"
#include "pkdir.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI_SIZE (0x5+1)
#define PERSONAL_SIZE (PI_SIZE*(PKM_COUNT+1))

static PersonalInfo personalInfo; // Empty personal
static PersonalInfo personalInfos[PKM_COUNT+1];

static PersonalInfo* PersonalInfoImport(PersonalInfo* pInfo, const uint8_t* buf)
{
	if (!buf) return NULL;

	pInfo->HP = buf[0x0];
	pInfo->ATK = buf[0x1];
	pInfo->DEF = buf[0x2];
	pInfo->SPE = buf[0x3];
	pInfo->SPC = buf[0x4];
	pInfo->BST = pInfo->HP + pInfo->ATK + pInfo->DEF + pInfo->SPE + pInfo->SPC;

	return pInfo;
}

static void PersonalImport(const uint8_t* buf)
{
	for (uint8_t i = 0; i < PKM_COUNT+1; i++)
	{
		PersonalInfoImport(&personalInfos[i], buf+PI_SIZE*i);
	}
	memset(&personalInfo, 0, sizeof(PersonalInfo));
}

Result PersonalLoad(void)
{
	Result ret = -1;
	uint16_t bytesRead = 0;

	printf("Opening personal file...\n");
	FILE* fp = fopen(DATA_FOLDER "personal_gb", "rb");
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

	return ret;
}

const PersonalInfo* Personal(DEX_Species species)
{
	return species > SPECIES_MISSINGNO && species <= SPECIES_MEW ? &personalInfos[species] : &personalInfo;
}
