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

	ret = fontLoad();
	if (R_FAILED(ret))
	{
		// Font
		error |= BIT(4);
	}

	ret = gfxLoad();
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
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

	ret = gfxLoadFrame(titleEntry.titleid);
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
	}

	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
#else
	while (aptMainLoop())
	{

	ret = gfxLoadFrame(0x0004000000171800);
	if (R_FAILED(ret))
	{
		// Graphics
		error |= BIT(5);
	}

	ret = FS_Init();
#endif
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= BIT(7);
	}

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
	}

	saveExit();

#ifdef __cia
	gfxFreeFrame();
	} // while (aptMainLoop())
	FSCIA_Exit();
#else
	gfxFreeFrame();
	FS_Exit();
#endif

	if (error)
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^

		printf("\nProblem happened: %lx\n", error);
		printf("PHBankGB version: %08x\n", VERSION);
		printf("Can't start the viewer.\n");
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}

#ifndef __cia
	printf("\n\n");
	}
#endif

	gfxFree();
	fontFree();

	sf2d_fini();
	return 0;
}
