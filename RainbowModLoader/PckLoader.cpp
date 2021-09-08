#include "Context.h"
#include "Types.h"
#include "Utilities.h"
#include "SigScan.h"

std::vector<std::string> modDirectoryPaths;

HOOK(bool, __fastcall, LoadPckFiles, sigLoadPckFiles(), ProjectSettings* This, const String& p_path)
{
    const bool result = originalLoadPckFiles(This, p_path);

    for (auto& modDirectoryPath : modDirectoryPaths)
    {
        String path;
        copyFrom(&path, modDirectoryPath.c_str());

        originalLoadPckFiles(This, path);
    }

    return result;
}

void initPckLoader()
{
    processDirectoryPaths(modDirectoryPaths, reverseLoadOrder);

    if (!modDirectoryPaths.empty())
        INSTALL_HOOK(LoadPckFiles);
}