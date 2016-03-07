#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>

#include "key.h"
#include "gfx.h"
#include "font.h"
#include "save.h"
#include "data.h"

#include "box_viewer.h"

extern sf2d_texture* pkmIcons;

int main(void)
{
	sf2d_init();
	gfxCreate();
	fontInit();

	consoleInit(GFX_TOP, NULL);

	saveInitialize();
	boxViewerInitialize();

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		boxViewerUpdate();

		if (hidKeysDown() & KEY_START) break;
		
		boxViewerDraw();
	}

	saveExit();

	waitKey(KEY_TOUCH);

	fontExit();
	gfxFree();
	sf2d_fini();
	return 0;
}
