#pragma once

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
