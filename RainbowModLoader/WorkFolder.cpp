#include "Context.h"
#include "Types.h"
#include "Utilities.h"

std::vector<std::string> workDirectoryPaths;

HOOK(FileAccess*, __fastcall, FileAccessOpen, sigFileAccessOpen2(), const String& p_path, int p_mode_flags, Error* p_error)
{
    if (!beginsWith(&p_path, "res://"))
        return originalFileAccessOpen(p_path, p_mode_flags, p_error);

    FileAccess* fileAccess = (FileAccess*)memoryAllocStatic(64);
    fileAccessWindowsCtor(fileAccess);

    for (auto& workDirectoryPath : workDirectoryPaths)
    {
        String redirectedPath;
        replace(&p_path, redirectedPath, "res:/", workDirectoryPath.c_str());

        if (fileAccessOpen(fileAccess, redirectedPath, p_mode_flags) != 0) // Returns 0 if file was found
            continue;

        if (p_error) 
            *p_error = 0;

        return fileAccess;
    }

    fileAccessWindowsDtor(fileAccess, 1);

    return originalFileAccessOpen(p_path, p_mode_flags, p_error);
}

void initWorkFolder()
{
    processDirectoryPaths(workDirectoryPaths, !reverseLoadOrder);

    if (!workDirectoryPaths.empty())
        INSTALL_HOOK(FileAccessOpen);
}
