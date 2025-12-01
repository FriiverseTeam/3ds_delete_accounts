#include "main.h"

int main(int argc, char *argv[]) {
    gfxInitDefault();
    hidInit();
    frdaInit();

    consoleInit(GFX_BOTTOM, NULL);

	ndmuInit();
	NDMU_SuspendDaemons(NDM_DAEMON_MASK_FRIENDS);
	ndmuExit();

	FRDA_SetClientSdkVersion(0x70000c8);
	FRDA_Save();
	FRDA_UnloadLocalAccount();

    signed int selection = 0;
    const char* options[] = { "Delete Nintendo Account", "Delete Friiverse Account" };
    const signed int optionCount = 2;

    uint32_t accountId = 1;

    while (aptMainLoop()) {
        consoleClear();
        for (signed int i = 0; i < optionCount; i++) {
            if (i == selection) {
                printf("> %s\n", options[i]);
            } else {
                printf("  %s\n", options[i]);
            }
        }

        gspWaitForVBlank();
        gfxSwapBuffers();

        hidScanInput();
        uint32_t kDown = hidKeysDown();

        if ((kDown & KEY_DOWN) && selection < optionCount - 1) selection++; 
        if ((kDown & KEY_UP) && selection > 0) selection--;
        if ((kDown & KEY_A)) {
            if (selection == 0) {
                consoleClear();
                accountId = 1;
                FRDA_DeleteLocalAccount(accountId);
                printf("\n\nNintendo account deleted!");
                svcSleepThread(5 * 1000 * 1000 * 1000LL);
                break;
            } else if (selection == 1) {
                consoleClear();
                accountId = 3;
                FRDA_DeleteLocalAccount(accountId);
                printf("\n\nFriiverse account deleted!");
                svcSleepThread(5 * 1000 * 1000 * 1000LL);
                break;
            }
        } else if (kDown & KEY_START) {
            break;
        }
    }

    frdaExit();
    hidExit();
    gfxExit();
    return 0;
}