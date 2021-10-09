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
    processFilePaths(dllFilePaths, !reverseLoadOrder);

    if (!dllFilePaths.empty())
    {
        std::vector<DllEvent*> postInitEvents;

        WCHAR currentDirectory[0x400];
        WCHAR dllDirectory[0x400];

        GetCurrentDirectoryW(_countof(currentDirectory), currentDirectory);
        GetDllDirectoryW(_countof(dllDirectory), dllDirectory);

        LOG("DLL:")

        for (auto& dllFilePath : dllFilePaths)
        {
            const std::wstring directoryPath = std::filesystem::path(dllFilePath).parent_path().wstring();

            SetCurrentDirectoryW(directoryPath.c_str());
            SetDllDirectoryW(directoryPath.c_str());

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

            if (enableDebugConsole)
            {
                std::wstring relativePath = std::filesystem::relative(dllFilePath, currentDirectory).wstring();

                if (relativePath.empty() || relativePath.size() > dllFilePath.size())
                    relativePath = dllFilePath;

                std::replace(relativePath.begin(), relativePath.end(), '\\', '/');

                LOG(" - %ls", relativePath.c_str())
            }

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

        SetCurrentDirectoryW(currentDirectory);
        SetDllDirectoryW(dllDirectory);
    }

    INSTALL_HOOK(Present);
}