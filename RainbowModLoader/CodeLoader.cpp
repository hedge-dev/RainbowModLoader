#include "Context.h"
#include "SigScan.h"
#include "Utilities.h"

typedef void DllEvent();

std::vector<std::wstring> dllFilePaths;
std::vector<DllEvent*> onFrameEvents;

HOOK(bool, __fastcall, Update, sigUpdate(), void* This)
{
    for (auto& dllEvent : onFrameEvents)
        dllEvent();

    CommonLoader::CommonLoader::RaiseUpdates();

    return originalUpdate(This);
}

void initCodeLoader()
{
    processFilePaths(dllFilePaths, reverseLoadOrder);

    std::vector<DllEvent*> postInitEvents;

    for (auto& dllFilePath : dllFilePaths)
    {
        const HMODULE module = LoadLibraryW(dllFilePath.c_str());

        if (!module)
            continue;

        const FARPROC initEvent = GetProcAddress(module, "Init");

        if (initEvent)
            ((DllEvent*)initEvent)();

        const FARPROC postInitEvent = GetProcAddress(module, "PostInit");

        if (postInitEvent)
            postInitEvents.push_back((DllEvent*)postInitEvent);

        const FARPROC onFrameEvent = GetProcAddress(module, "OnFrame");

        if (onFrameEvent)
            onFrameEvents.push_back((DllEvent*)onFrameEvent);
    }

    for (auto& dllEvent : postInitEvents)
        dllEvent();

    INSTALL_HOOK(Update);
}