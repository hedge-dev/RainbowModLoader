#include "Context.h"

HOOK(void*, __fastcall, InitOsWindows, 0x1400FDD00, void* A1, void* A2)
{
    initMods();
    initWorkFolder();
    initMountPck();

    return originalInitOsWindows(A1, A2);
}

void init()
{
    INSTALL_HOOK(InitOsWindows);
}
