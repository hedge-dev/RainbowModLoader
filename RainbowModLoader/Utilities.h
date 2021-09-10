#pragma once

inline void processDirectoryPaths(std::vector<std::string>& directoryPaths, const bool reverse)
{
    std::vector<std::string> newDirectoryPaths;

    const std::filesystem::path currentPath = std::filesystem::current_path();

    for (auto& directoryPath : directoryPaths)
    {
        if (!std::filesystem::is_directory(directoryPath))
            continue;

        const std::string relativePath = std::filesystem::relative(directoryPath, currentPath).u8string();
        newDirectoryPaths.push_back(relativePath.size() < directoryPath.size() ? relativePath : directoryPath);
    }

    if (reverse)
        std::reverse(newDirectoryPaths.begin(), newDirectoryPaths.end());

    std::swap(directoryPaths, newDirectoryPaths);
}

inline void processFilePaths(std::vector<std::wstring>& filePaths, const bool reverse)
{
    std::vector<std::wstring> newFilePaths;

    const std::filesystem::path currentPath = std::filesystem::current_path();

    for (auto& filePath : filePaths)
    {
        if (!std::filesystem::is_regular_file(filePath))
            continue;

        const std::wstring relativePath = std::filesystem::relative(filePath, std::filesystem::current_path()).wstring();
        newFilePaths.push_back(relativePath.size() < filePath.size() ? relativePath : filePath);
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