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

inline void processFilePaths(std::vector<std::wstring>& filePaths, const bool reverse)
{
    std::vector<std::wstring> newFilePaths;

    for (auto& filePath : filePaths)
    {
        WCHAR fullPath[0x400];
        GetFullPathNameW(filePath.c_str(), _countof(fullPath), fullPath, nullptr);

        const DWORD dwAttrib = GetFileAttributesW(fullPath);
        if (dwAttrib == INVALID_FILE_ATTRIBUTES || (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0) // Skip if file doesn't exist
            continue;

        newFilePaths.emplace_back(fullPath);
    }

    if (reverse)
        std::reverse(newFilePaths.begin(), newFilePaths.end());

    std::swap(filePaths, newFilePaths);
}

inline std::wstring convertMultiByteToWideChar(const std::string& value)
{
    WCHAR wideChar[0x400];
    MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wideChar, _countof(wideChar));

    return std::wstring(wideChar);
}

inline std::string getDirectoryPath(const std::string& path)
{
    std::string directoryPath;

    const size_t index = path.find_last_of("\\/");
    if (index != std::string::npos)
        directoryPath = path.substr(0, index + 1);

    return directoryPath;
}