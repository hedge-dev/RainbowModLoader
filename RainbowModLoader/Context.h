#pragma once

extern bool reverseLoadOrder;
extern std::vector<std::string> workDirectoryPaths;
extern std::vector<std::string> modDirectoryPaths;

extern void init();

extern void initMods();
extern void initWorkFolder();
extern void initMountPck();