#pragma once

extern bool reverseLoadOrder;
extern std::vector<std::string> workDirectoryPaths;
extern std::vector<std::string> modDirectoryPaths;
extern std::vector<std::wstring> dllFilePaths;

extern void init();

extern void initModLoader();
extern void initWorkLoader();
extern void initPckLoader();
extern void initCodeLoader();