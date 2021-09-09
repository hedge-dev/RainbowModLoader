#pragma once

extern void* sigInitOsWindows(); // 0x1400FDD00
extern void* sigLoadPckFiles(); // 0x1402BF6C0
extern void* sigFileAccessOpen2(); // 0x1402B9AD0
extern void* sigPresent(); // 0x140109AD0

extern void* sigMemoryAllocStatic(); // 0x140114D60
extern void* sigMemoryFreeStaticUint8(); // 0x140114E90
extern void* sigCopyFrom(); // 0x1401285E0
extern void* sigBeginsWith(); // 0x1401182E0
extern void* sigReplace(); // 0x14011A5B0
extern void* sigFileAccessWindowsCtor(); // 0x14030A790
extern void* sigFileAccessWindowsDtor(); // 0x14030AD10
extern void* sigFileAccessOpen(); // 0x140309690

extern void* sigCriFsBindCpkInternal(); // 0x14210F170
extern void* sigCriFsBindDirectory(); // 0x14210BE20
extern void* sigCriFsSetPriority(); // 0x14210ED30
extern void* sigCriFsInitializeLibrary(); // 0x14210A484