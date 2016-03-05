#include <3ds.h>
#include <sf2d.h>
int main(void)
{
	sf2d_init();
	consoleInit(GFX_TOP, NULL);
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		if (hidKeysDown() & KEY_B) break;
	}
	sf2d_fini();
	return 0;
}
