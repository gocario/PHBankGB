#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>

#include "fs.h"
#include "key.h"
#include "gfx.h"
#include "font.h"
#include "save.h"
#include "data.h"
#include "error.h"
#include "version.h"
#include "personal.h"
#include "box_viewer.h"

#ifdef __cia
#include "ts.h"
#endif

int main(int argc, char* argv[])
{
	Result ret = 0, error = 0;

	sf2d_init();
	
	// consoleInit(GFX_TOP, NULL);

	ret = gfxLoad();
	if (R_FAILED(ret))
	{
		// Graphics
		error |= ERR_GRAPHICS;
	}

	ret = fontLoad();
	if (R_FAILED(ret))
	{
		// Font
		error |= ERR_FONT;
	}

	ret = PersonalLoad();
	if (R_FAILED(ret))
	{
		// Data
		error |= ERR_DATA;
	}

#ifdef __cia
	while (!error && TS_Loop())
	{

	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= ERR_FILESYSTEM;
	}

	titleid = titleEntry.titleid;
#else // __3dsx
	ret = FS_Init();
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= ERR_FILESYSTEM;
	}

#ifdef DEBUG
	titleid = 0x0004000000171800; // Pokémon Jaune
#else
	aptOpenSession();
	APT_GetProgramID(&titleid);
	aptCloseSession();
#endif
#endif

	ret = gfxLoadFrame(titleid);
	if (R_FAILED(ret))
	{
		// Graphics
		error |= ERR_GRAPHICS;
	}

	ret = saveLoad(argc > 1 ? argv[1] : NULL);
	if (R_FAILED(ret))
	{
		// Save
		error |= ERR_SAVE;
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

		saveSave(argc > 1 ? argv[1] : NULL);
	}

	gfxFreeFrame();

#ifdef __cia
	FSCIA_Exit();
	} // while (TS_Loop())
#else // __3dsx
	FS_Exit();
#endif

	if (error)
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^

		printf("\nProblem happened: 0x%lx\n", error);
		if (error & ERR_SAVE) printf(" \a Save\n");
		if (error & ERR_DATA) printf(" \a Data\n");
		if (error & ERR_FONT) printf(" \a Font\n");
		if (error & ERR_GRAPHICS) printf(" \a Graphics\n");
		if (error & ERR_FILESYSTEM) printf(" \a Filesystem\n");
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
