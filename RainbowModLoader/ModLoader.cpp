#include "Context.h"
#include "Utilities.h"

bool enableDebugConsole;
bool reverseLoadOrder;

void loadMod(const std::string& filePath)
{
    if (filePath.empty())
        return;

    const INIReader mod(filePath);

    if (mod.ParseError() != 0)
    {
        LOG(" - Failed to load \"%s\"", filePath.c_str())
        return;
    }

    LOG(" - %s", mod.GetString("Desc", "Title", std::string()).c_str())

    const std::string directoryPath = getDirectoryPath(filePath);

    const long includeDirCount = mod.GetInteger("Main", "IncludeDirCount", 0);

    for (long i = 0; i < includeDirCount; i++)
    {
        const std::string includeDir = mod.GetString("Main", "IncludeDir" + std::to_string(i), std::string());

        if (includeDir.empty())
            continue;

        const std::string fullIncludeDir = directoryPath + includeDir;

        workDirectoryPaths.push_back(fullIncludeDir + "/work");
        modDirectoryPaths.push_back(fullIncludeDir);
        cpkDirectoryPaths.push_back(fullIncludeDir + "/PCCriPak");
    }

    const std::string dllFilePath = mod.GetString("Main", "DLLFile", std::string());

    if (!dllFilePath.empty())
        dllFilePaths.push_back(convertMultiByteToWideChar(directoryPath + dllFilePath));
}

void loadModsDatabase(const std::string& filePath)
{
    if (filePath.empty())
        return;

    const INIReader modsDatabase(filePath);

    if (enableDebugConsole)
    {
        std::string relativePath = std::filesystem::relative(filePath, std::filesystem::current_path()).string();

        if (relativePath.empty() || relativePath.size() > filePath.size())
            relativePath = filePath;

        if (modsDatabase.ParseError() != 0)
            LOG("Failed to load \"%s\"", filePath.c_str())

        else
            LOG("ModsDB: %s", relativePath.c_str())
    }

    reverseLoadOrder = modsDatabase.GetBoolean("Main", "ReverseLoadOrder", true);

    const long activeModCount = modsDatabase.GetInteger("Main", "ActiveModCount", 0);

    for (long i = 0; i < activeModCount; i++)
    {
        const std::string activeMod = modsDatabase.GetString("Main", "ActiveMod" + std::to_string(i), std::string());

        if (activeMod.empty())
            continue;

        const std::string modFilePath = modsDatabase.GetString("Mods", activeMod, std::string());
        loadMod(modFilePath);
    }
}

void initModLoader()
{
    const INIReader main("cpkredir.ini");

    if (!main.GetBoolean("CPKREDIR", "Enabled", true))
        return;

    enableDebugConsole = main.GetString("CPKREDIR", "LogType", std::string()) == "console";

    if (enableDebugConsole)
    {
        if (!GetConsoleWindow())
            AllocConsole();

        freopen("CONOUT$", "w", stdout);
    }

    const std::string modsDbFilePath = main.GetString("CPKREDIR", "ModsDbIni", "mods/ModsDb.ini");
    loadModsDatabase(modsDbFilePath);

    workDirectoryPaths.emplace_back("../content/acorn/work");

    CommonLoader::CommonLoader::InitializeAssemblyLoader((getDirectoryPath(modsDbFilePath) + "Codes.dll").c_str());
    CommonLoader::CommonLoader::RaiseInitializers();
}