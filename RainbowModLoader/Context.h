#pragma once

#define LOG(x, ...) \
    { \
        if (enableDebugConsole) \
        { \
            printf("[RML] "##x##"\n", __VA_ARGS__); \
        } \
    }

extern bool enableDebugConsole;

extern bool reverseLoadOrder;
extern std::vector<std::string> workDirectoryPaths;
extern std::vector<std::string> modDirectoryPaths;
extern std::vector<std::string> cpkDirectoryPaths;
extern std::vector<std::wstring> dllFilePaths;

extern void initModLoader();
extern void initWorkLoader();
extern void initPckLoader();
extern void initCpkRedirector();
extern void initCodeLoader();

extern void init();
