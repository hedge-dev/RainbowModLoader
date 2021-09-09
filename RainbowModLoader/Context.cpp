#include "Context.h"
#include "SigScan.h"

HOOK(void*, __fastcall, InitOsWindows, sigInitOsWindows(), void* A1, void* A2)
{
    initModLoader();
    initWorkLoader();
    initPckLoader();
    initCpkRedirector();
    initCodeLoader();

    return originalInitOsWindows(A1, A2);
}

void init()
{
    INSTALL_HOOK(InitOsWindows);
}
