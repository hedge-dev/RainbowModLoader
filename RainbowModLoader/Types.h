#pragma once

typedef int Error;
class FileAccess;
class String;
class ProjectSettings;

static FUNCTION_PTR(void, __fastcall, memoryFreeStaticUint8, 0x140114E90, void* memory, bool p_pad_align);

static FUNCTION_PTR(void, __fastcall, cowDataUnref, 0x140114E90, void* memory);

static FUNCTION_PTR(void, __fastcall, copyFrom, 0x1401285E0, String* This, const char* p_cstr);
static FUNCTION_PTR(bool, __fastcall, beginsWith, 0x1401182E0, const String* This, const char* p_string);
static FUNCTION_PTR(void, __fastcall, replace, 0x14011A5B0, const String* This, String& p_return, const char* p_key, const char* p_width);

static FUNCTION_PTR(FileAccess*, __fastcall, createFileAccessWindows, 0x14010D570);
static FUNCTION_PTR(void, __fastcall, fileAccessWindowsDtor, 0x14030AD10, FileAccess* This, int flags);

static FUNCTION_PTR(Error, __fastcall, fileAccessOpen, 0x140309690, FileAccess* This, const String& p_path, int p_mode_flags);
static FUNCTION_PTR(void, __fastcall, fileAccessFixPath, 0x1402B7E40, FileAccess* This, String& p_return, const String& p_path);

template<class T>
class CowData
{
public:
    T* ptr{};

    ~CowData()
    {
        if (ptr && InterlockedDecrement((unsigned volatile*)((char*)ptr - 16)) == 0)
            memoryFreeStaticUint8(ptr, true);
    }
};

class String
{
public:
    CowData<wchar_t> cowData;
};

