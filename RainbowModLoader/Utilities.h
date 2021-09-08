#pragma once

inline void processDirectoryPaths(std::vector<std::string>& directoryPaths, const bool reverse)
{
    std::vector<std::string> newDirectoryPaths;

    for (auto& directoryPath : directoryPaths)
    {
        WCHAR directoryPathWchar[0x400];
        WCHAR fullPathWchar[0x400];

        MultiByteToWideChar(CP_UTF8, 0, directoryPath.c_str(), -1, directoryPathWchar, _countof(directoryPathWchar));
        GetFullPathNameW(directoryPathWchar, _countof(fullPathWchar), fullPathWchar, nullptr);

        const DWORD dwAttrib = GetFileAttributesW(fullPathWchar);
        if (dwAttrib == INVALID_FILE_ATTRIBUTES || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0) // Skip if directory doesn't exist
            continue;

        CHAR fullPath[0x400];
        WideCharToMultiByte(CP_UTF8, 0, fullPathWchar, -1, fullPath, _countof(fullPath), 0, 0);

        newDirectoryPaths.emplace_back(fullPath);
    }

    if (reverse)
        std::reverse(newDirectoryPaths.begin(), newDirectoryPaths.end());

    std::swap(directoryPaths, newDirectoryPaths);
}