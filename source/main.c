#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>

#include "fs.h"
#include "key.h"
#include "gfx.h"
#include "font.h"
#include "save.h"
#include "data.h"
#include "version.h"
#include "personal.h"
#include "box_viewer.h"

#ifdef __cia
#include "ts.h"
#endif

int main(void)
{
	Result ret = 0, error = 0;

	sf2d_init();
	
	// consoleInit(GFX_TOP, NULL);

	ret = gfxLoad();
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
	}

	ret = fontLoad();
	if (R_FAILED(ret))
	{
		// Font
		error |= BIT(4);
	}

	ret = PersonalLoad();
	if (R_FAILED(ret))
	{
		// Data
		error |= BIT(3);
	}

#ifdef __cia
	while (!error && TS_Loop())
	{

	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= BIT(7);
	}

	titleid = titleEntry.titleid;

	ret = gfxLoadFrame(titleEntry.titleid);
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
	}
#else
	ret = FS_Init();
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= BIT(7);
	}

	// TODO: Remove the while loop for 3dsx build
	while (!error && aptMainLoop())
	{

	titleid = 0x0004000000171800; // Pokémon Yellow	(EUR) (FR)
	// titleid = 0x0004000000170D00; // Pokémon Green	(JPN) (JP)

	ret = gfxLoadFrame(titleid);
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
	}
#endif

	ret = saveLoad();
	if (R_FAILED(ret))
	{
		// Save
		error |= BIT(2);
	}

	if (!error)
	{
		boxViewerInitialize();

		while (aptMainLoop())
		{
			gspWaitForVBlank();
			hidScanInput();

			boxViewerUpdate();

			if (hidKeysDown() & KEY_START) break;
			
			boxViewerDraw();
		}

		saveSave();
	}

#ifdef __cia
	gfxFreeFrame();
	FSCIA_Exit();
	} // while (TS_Loop())
#else
	gfxFreeFrame();
	} // while (aptMainLoop())
	FS_Exit();
#endif

	if (error)
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^

		printf("\nProblem happened: 0x%lx\n", error);
		printf("PHBankGB version: %08x\n", VERSION);
		printf("Can't start the viewer.\n");
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}

	fontFree();
	gfxFree();

	sf2d_fini();
	return 0;
}
