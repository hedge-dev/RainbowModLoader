#pragma once

#include "SigScan.h"

typedef int Error;
class FileAccess;
class String;
class ProjectSettings;

static FUNCTION_PTR(void*, __fastcall, memoryAllocStatic, sigMemoryAllocStatic(), size_t length);
static FUNCTION_PTR(void, __fastcall, memoryFreeStaticUint8, sigMemoryFreeStaticUint8(), void* memory, bool p_pad_align);

static FUNCTION_PTR(void, __fastcall, copyFrom, sigCopyFrom(), String* This, const char* p_cstr);
static FUNCTION_PTR(bool, __fastcall, beginsWith, sigBeginsWith(), const String* This, const char* p_string);
static FUNCTION_PTR(void, __fastcall, replace, sigReplace(), const String* This, String& p_return, const char* p_key, const char* p_width);

static FUNCTION_PTR(FileAccess*, __fastcall, fileAccessWindowsCtor, sigFileAccessWindowsCtor(), FileAccess* This);
static FUNCTION_PTR(void, __fastcall, fileAccessWindowsDtor, sigFileAccessWindowsDtor(), FileAccess* This, int flags);

static FUNCTION_PTR(Error, __fastcall, fileAccessOpen, sigFileAccessOpen(), FileAccess* This, const String& p_path, int p_mode_flags);

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

