#include "Context.h"
#include "Utilities.h"
#include "SigScan.h"

FUNCTION_PTR(int, __fastcall, criFsBindDirectory, sigCriFsBindDirectory(), void* bndrhn, void* srcbndrhn, const char* path, void* work, int worksize, int* bndrid);
FUNCTION_PTR(int, __fastcall, criFsSetPriority, sigCriFsSetPriority(), int bndrid, int priority);

std::vector<std::string> cpkDirectoryPaths;

HOOK(int, __fastcall, CriFsBindCpkInternal, sigCriFsBindCpkInternal(), void* bndrhn, void* srcbndrhn, const char* path, void* work, int worksize, int* bndrid, int flags)
{
    LOG("CPK:")

    for (size_t i = 0; i < cpkDirectoryPaths.size(); i++)
    {
        LOG(" - %s", cpkDirectoryPaths[i].c_str())

        criFsBindDirectory(bndrhn, srcbndrhn, cpkDirectoryPaths[i].c_str(), operator new(88), 88, bndrid);
        criFsSetPriority(*bndrid, (int)(1 + i));
    }

    return originalCriFsBindCpkInternal(bndrhn, srcbndrhn, path, work, worksize, bndrid, flags);
}

HOOK(int, __fastcall, CriFsInitializeLibrary, sigCriFsInitializeLibrary(), void* config, void* buffer, int size)
{
    int maxBinds = (int)(1 + cpkDirectoryPaths.size());

    // Increase max bind count for every AWB file
    for (auto& cpkDirectoryPath : cpkDirectoryPaths)
    {
        for (auto& entry : std::filesystem::recursive_directory_iterator(cpkDirectoryPath))
        {
            std::string extension = entry.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

            if (extension == ".awb")
                ++maxBinds;
        }
    }

    *(int*)((char*)config + 24) = maxBinds;

    return originalCriFsInitializeLibrary(config, buffer, size);
}

void initCpkRedirector()
{
    processDirectoryPaths(cpkDirectoryPaths, reverseLoadOrder);

    if (cpkDirectoryPaths.empty())
        return;

    INSTALL_HOOK(CriFsBindCpkInternal);
    INSTALL_HOOK(CriFsInitializeLibrary);
}