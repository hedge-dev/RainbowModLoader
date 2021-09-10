#include "Context.h"
#include "SigScan.h"
#include "Utilities.h"

typedef void DllEvent();

std::vector<std::wstring> dllFilePaths;
std::vector<DllEvent*> onFrameEvents;

HOOK(HRESULT, __fastcall, Present, sigPresent(), void* This)
{
    for (auto& dllEvent : onFrameEvents)
        dllEvent();

    CommonLoader::CommonLoader::RaiseUpdates();

    return originalPresent(This);
}

void initCodeLoader()
{
    processFilePaths(dllFilePaths, reverseLoadOrder);

    if (!dllFilePaths.empty())
    {
        LOG("DLL:")

        std::vector<DllEvent*> postInitEvents;

        for (auto& dllFilePath : dllFilePaths)
        {
            const HMODULE module = LoadLibraryW(dllFilePath.c_str());

            if (!module)
            {
                LPWSTR buffer = nullptr;

                const DWORD msgSize = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buffer, 0, NULL);

                const std::wstring msg = L"Failed to load \"" + dllFilePath + L"\"\n" + std::wstring(buffer, msgSize);
                MessageBoxW(nullptr, msg.c_str(), L"Rainbow Mod Loader", MB_OK);

                LocalFree(buffer);

                LOG(" - Failed to load \"%ls\"", dllFilePath.c_str())

                continue;
            }

            LOG(" - %ls", dllFilePath.c_str())

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
    }

    INSTALL_HOOK(Present);
}