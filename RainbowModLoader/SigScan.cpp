#include "SigScan.h"
#include <Psapi.h>

MODULEINFO moduleInfo;

const MODULEINFO& getModuleInfo()
{
    if (moduleInfo.SizeOfImage)
        return moduleInfo;

    ZeroMemory(&moduleInfo, sizeof(MODULEINFO));
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &moduleInfo, sizeof(MODULEINFO));

    return moduleInfo;
}

void* sigScan(const char* signature, const char* mask)
{
    const MODULEINFO& moduleInfo = getModuleInfo();
    const size_t length = strlen(mask);

    for (size_t i = 0; i < moduleInfo.SizeOfImage; i++)
    {
        char* memory = (char*)moduleInfo.lpBaseOfDll + i;

        size_t j;
        for (j = 0; j < length; j++)
        {
            if (mask[j] != '?' && signature[j] != memory[j])
                break;
        }

        if (j == length) 
            return memory;
    }

    return nullptr;
}

#define SIG_SCAN(x, signature, mask) \
    void* _##x; \
    void* x() \
    { \
        if (!_##x) _##x = sigScan(signature, mask); \
        return _##x; \
    }

SIG_SCAN(sigInitOsWindows, "\x89\x4C\x24\x08\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x89\x9C\x24\x00\x00\x00\x00\x4C\x8B\xEA\x48\x63\xD9\x33\xD2\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8D\x15\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8B\xFB\x48\x89\x5C\x24\x00\xB8\x00\x00\x00\x00\x48\xF7\xE3\x48\xC7\xC1\x00\x00\x00\x00\x48\x0F\x40\xC1\x48\x8B\xC8\xE8\x00\x00\x00\x00\x4C\x8B\xF0\x48\x89\x84\x24\x00\x00\x00\x00\x33\xDB\x4D\x85\xFF\x0F\x8E\x00\x00\x00\x00\x4C\x8B\xE0\x4C\x2B\xE8\x4C\x8D\x73\xFF\x0F\x1F\x40\x00\x0F\x1F\x84\x00\x00\x00\x00\x00\x4B\x8B\x2C\x2C\x48\x89\x5C\x24\x00\x48\x89\x5C\x24\x00\x89\x5C\x24\x28\x48\x89\x5C\x24\x00\x45\x8B\xCE\x4C\x8B\xC5\x33\xD2\xB9\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x48\x63\xF0\x8D\x4E\x01\x48\x63\xC9\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x5C\x24\x00\x48\x89\x5C\x24\x00\x89\x74\x24\x28\x48\x89\x44\x24\x00\x45\x8B\xCE\x4C\x8B\xC5\x33\xD2\xB9\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x88\x1C\x3E\x49\x89\x3C\x24\x4D\x8D\x64\x24\x00\x49\x83\xEF\x01\x75\x8D\x4C\x8B\xB4\x24\x00\x00\x00\x00\x4C\x8B\x7C\x24\x00\x4D\x8D\x46\x08\x8B\x94\x24\x00\x00\x00\x00\xFF\xCA\x41\xB1\x01\x49\x8B\x0E\xE8\x00\x00\x00\x00\x85\xC0\x74\x25\x4D\x85\xFF\x7E\x11\x49\x8B\x0C\xDE\xE8\x00\x00\x00\x00\x48\xFF\xC3\x49\x3B\xDF\x7C\xEF\x49\x8B\xCE\xE8\x00\x00\x00\x00\xBB\x00\x00\x00\x00\xEB\x44\xE8\x00\x00\x00\x00\x84\xC0\x74\x0A\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4D\x85\xFF\x7E\x13\x66\x90\x49\x8B\x0C\xDE\xE8\x00\x00\x00\x00\x48\xFF\xC3\x49\x3B\xDF\x7C\xEF\x49\x8B\xCE\xE8\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x8B\xC3\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x41\x5F\x41\x5E\x41\x5D\x41\x5C\x5F\x5E\x5D\xC3", "xxxxxxxxxxxxxxxx????x????xxxxxxx?????xxxx????xxxxxxxxxxxx?x????xxxx????x????x????xxxxxxx?x????xxxxxx????xxxxxxxx????xxxxxxx????xxxxxxx????xxxxxxxxxxxxxxxxxx????xxxxxxxx?xxxx?xxxxxxxx?xxxxxxxxx????xx????xxxxxxxxxx????xxxxxxx?xxxx?xxxxxxxx?xxxxxxxxx????xx????xxxxxxxxxxx?xxxxxxxxxx????xxxx?xxxxxxx????xxxxxxxxx????xxxxxxxxxxxxxx????xxxxxxxxxxxx????x????xxx????xxxxxxxx?x????x????xxxxxxxxxxxx????xxxxxxxxxxxx????xxxx?x????xxxxxx?x????xxxxxx????xxx????xxxxxxxxxxxx");
SIG_SCAN(sigLoadPckFiles, "\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x00\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x4C\x8B\xEA\x45\x33\xE4\x44\x89\x65\x77\x45\x8B\xF4\x4C\x89\x65\xEF\x4C\x8D\x0D\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\x48\x8D\x55\x7F\x49\x8B\xCD\xE8\x00\x00\x00\x00\x90\x48\x8B\xD0\x49\x8B\xCD\xE8\x00\x00\x00\x00\x4C\x89\x65\x77\x49\x8B\xD5\x48\x8D\x4D\x77\xE8\x00\x00\x00\x00\x90\x48\x8B\x5D\x77\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\x5D\x7F\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\x7F\xE8\x00\x00\x00\x00\x90\xC7\x45\x00\x00\x00\x00\x00\x48\x8D\x55\x7F\x49\x8B\xCD\xE8\x00\x00\x00\x00\x84\xC0\x75\x1C\x45\x33\xC0\x48\x8D\x15\x00\x00\x00\x00\x49\x8B\xCD\xE8\x00\x00\x00\x00\x83\xF8\xFF\x75\x05\x40\xB7\x01\xEB\x03", "xxxx?xxxx?xxxxxxxxxxxxxxxx?xxx????xxx?????xxxxxxxxxxxxxxxxxxxx????xxx????xxxxxxxx????xxxxxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxx????xxxx????xxxxx????xxx?????xxxxxxxx????xxxxxxxxxx????xxxx????xxxxxxxxxx");
SIG_SCAN(sigFileAccessOpen2, "\x40\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x50\x48\xC7\x45\x00\x00\x00\x00\x00\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\xBC\x24\x00\x00\x00\x00\x49\x8B\xF0\x44\x8B\xF2\x48\x8B\xD9\x45\x33\xFF\x44\x89\x7D\x38\x48\x8D\x55\xD0\xE8\x00\x00\x00\x00\x90\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\xD0\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\xD0\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\xD0\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x8B\x3D\x00\x00\x00\x00\x38\x47\x30\x75\x78\x41\xF6\xC6\x02\x75\x72\x48\x8D\x55\xF0\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\xD0\x48\x8D\x4D\xD8\xE8\x00\x00\x00\x00\x4C\x8B\x07\x4D\x85\xC0\x74\x52\x4D\x8B\x40\x10\x4C\x8B\x57\x08\x4D\x3B\xC2\x74\x45\x4C\x8B\x4D\xE0\x48\x8B\x4D\xD8\x49\x8B\x40\x30\x48\x3B\xC1\x75\x09\x4D\x3B\x48\x38\x0F\x92\xC2\xEB\x03\x0F\x97\xC2\x84\xD2\x74\x06\x4D\x8B\x40\x10\xEB\x19\x48\x3B\xC8\x75\x09\x4D\x39\x48\x38\x0F\x92\xC0\xEB\x03\x0F\x97\xC0\x84\xC0\x74\x75\x4D\x8B\x40\x08\x4D\x3B\xC2\x75\xC3\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x8B\x00\x45\x8B\xC6\x48\x8B\xD3\x48\x8B\xCF\xFF\x10\x48\x85\xF6\x74\x02\x89\x06\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x84\xC0\x74\x6E\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\xD8\xE8\x00\x00\x00\x00\x90\x41\xBF\x00\x00\x00\x00\x44\x89\x7D\x38\x48\x8D\x55\xD8\x48\x8B\xCB\xE8\x00\x00\x00\x00\x84\xC0\x75\x43\x45\x0F\xB6\xF7\xEB\x40\x4D\x85\xC0\x74\x8F\x49\x83\x78\x00\x00\x74\x88\x49\x8B\x48\x68\x48\x8B\x01\x49\x83\xC0\x40\x48\x8B\xD3\xFF\x50\x08\x48\x8B\xF8\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\x85\xF6\x0F\x84\x00\x00\x00\x00\xC7\x06\x00\x00\x00\x00\xE9\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx?????xxxx?xxxx?xxxx????xxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxx????xxxx????xxx????xxxxx????xxxx????xxx????xxxxx????xxxx????xxx????xxxxxxxxxxxxxxxxxxx????xxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxxx????xxxxxxx????xxxxx????xxx????xxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx??xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxx????xx????x????");

SIG_SCAN(sigMemoryAllocStatic, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x30\x48\x8B\xF9\xBA\x00\x00\x00\x00\x48\x83\xC1\x20\xE8\x00\x00\x00\x00\x48\x8B\xD8\x48\x85\xC0\x75\x31\x4C\x8D\x0D\x00\x00\x00\x00\x89\x44\x24\x20\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x33\xC0\x48\x8B\x5C\x24\x00\x48\x83\xC4\x30\x5F\xC3\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x43\x20\x48\x89\x3B\x48\x8B\x5C\x24\x00\x48\x83\xC4\x30\x5F\xC3", "xxxx?xxxxxxxxx????xxxxx????xxxxxxxxxxx????xxxxxx????xxx????xxx????x????xxxxxx?xxxxxxxxx????x????xxxxxxxxxxx?xxxxxx");
SIG_SCAN(sigMemoryFreeStaticUint8, "\x48\x85\xC9\x74\x22\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x4B\xE0\xE8\x00\x00\x00\x00\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxxxxxxxx????x????xxxxx????xxxxxx");
SIG_SCAN(sigCopyFrom, "\x48\x89\x4C\x24\x00\x53\x48\x83\xEC\x30\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8B\xD9\x48\xC7\x01\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\xC3\x48\x83\xC4\x30\x5B\xC3", "xxxx?xxxxxxxxx?????xxxxxx????x????xxxxxxxxxx");
SIG_SCAN(sigBeginsWith, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x40\x48\x8B\x31\x33\xDB\x48\x8B\xFA\x48\x85\xF6\x74\x0D\x48\x8D\x46\xF8\x48\x85\xC0\x74\x04\x8B\x00\xEB\x02\x8B\xC3\x85\xC0\x8D\x68\xFF\x0F\x44\xEB\x85\xED\x0F\x84\x00\x00\x00\x00\x48\x85\xFF\x0F\x84\x00\x00\x00\x00\x4C\x89\x74\x24\x00\x4C\x89\x7C\x24\x00\x48\x85\xF6\x74\x65\x4C\x8D\x76\xF8\x4D\x85\xF6\x74\x5C\x45\x8B\x3E\x45\x85\xFF\x74\x54\xE8\x00\x00\x00\x00\x85\xC0\x7F\x52\xC6\x44\x24\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x4C\x24\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x4D\x85\xF6\x48\x89\x4C\x24\x00\x41\xB8\x00\x00\x00\x00\x44\x0F\x44\xFB\x48\x8D\x0D\x00\x00\x00\x00\x49\x63\xC7\x45\x33\xC9\x48\x89\x44\x24\x00\xE8\x00\x00\x00\x00\xCC\xEB\xFE\x48\x8D\x35\x00\x00\x00\x00\x0F\xB6\x07\x4C\x8B\x7C\x24\x00\x4C\x8B\x74\x24\x00\x84\xC0\x74\x21\x48\x63\xD5\x48\x3B\xDA\x7D\x19\x0F\xB7\x0C\x5E\x0F\xBE\xC0\x3B\xC1\x75\x21\x0F\xB6\x47\x01\x48\xFF\xC7\x48\xFF\xC3\x84\xC0\x75\xE2\x80\x3F\x00\x0F\x94\xC0\x48\x8B\x5C\x24\x00\x48\x83\xC4\x40\x5F\x5E\x5D\xC3\x48\x8B\x5C\x24\x00\x32\xC0\x48\x83\xC4\x40\x5F\x5E\x5D\xC3", "xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxx????xxxx?xxxx?xxxxxxxxxxxxxxxxxxxxxxx????xxxxxxx??xxx????xxxx?xxx????xxx????xxxxxxx?xx????xxxxxxx????xxxxxxxxxx?x????xxxxxx????xxxxxxx?xxxx?xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xxxxxxxxxxxx?xxxxxxxxxx");
SIG_SCAN(sigReplace, "\x48\x8B\xC4\x48\x89\x50\x10\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x40\x48\xC7\x40\x00\x00\x00\x00\x00\x48\x89\x58\x08\x48\x89\x68\x18\x48\x89\x70\x20\x4D\x8B\xE1\x49\x8B\xF0\x4C\x8B\xFA\x4C\x8B\xF1\x45\x33\xED\x44\x89\x68\xB8\x41\x8B\xDD\x48\x89\x58\xC0\x41\x8B\xFD\x45\x33\xC0\x48\x8B\xD6\xE8\x00\x00\x00\x00\x8B\xE8\x85\xC0\x0F\x88\x00\x00\x00\x00\x66\x0F\x1F\x44\x00\x00\x44\x8B\xCD\x44\x2B\xCF\x44\x8B\xC7\x48\x8D\x54\x24\x00\x49\x8B\xCE\xE8\x00\x00\x00\x00\x90\x48\x8B\xD0\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x5C\x24\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x49\x8B\xD4\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x41\x8B\xCD\x38\x0E\x74\x10\x48\x8B\xC6\x66\x90\xFF\xC1\x48\x8D\x40\x01\x80\x38\x00\x75\xF5\x8D\x3C\x29\x44\x8B\xC7\x48\x8B\xD6\x49\x8B\xCE\xE8\x00\x00\x00\x00\x8B\xE8\x85\xC0\x0F\x89\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x49\x8B\x06\x48\x85\xC0\x74\x0A\x48\x83\xC0\xF8\x74\x04\x8B\x08\xEB\x03\x41\x8B\xCD\x44\x8D\x49\xFF\x85\xC9\x45\x0F\x44\xCD\x44\x2B\xCF\x44\x8B\xC7\x48\x8D\x54\x24\x00\x49\x8B\xCE\xE8\x00\x00\x00\x00\x90\x48\x8B\xD0\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x5C\x24\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x4D\x89\x2F\x48\x8D\x54\x24\x00\x49\x8B\xCF\xE8\x00\x00\x00\x00\x90\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8B\x5C\x24\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\xEB\x39\x48\x8B\x5C\x24\x00\x4D\x89\x2F\x49\x8B\xD6\x49\x8B\xCF\xE8\x00\x00\x00\x00\x90\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x49\x8B\xC7\x4C\x8D\x5C\x24\x00\x49\x8B\x5B\x30\x49\x8B\x6B\x40\x49\x8B\x73\x48\x49\x8B\xE3\x41\x5F\x41\x5E\x41\x5D\x41\x5C\x5F\xC3", "xxxxxxxxxxxxxxxxxxxxxxx?????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxxx?xxxxxxxxxxxxx?xxxx????xxxxxxxx?x????xxxxx?xxxxxxxxxx????xxxxxxxxxx????xxxxxxxx?x????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xxxx????xxxxxxxx?x????xxxxx?xxxxxxxxxx????xxxxxxxxxx????xxxxxxxx?xxxx????xxxx?????xxxx?xxxxxxxxxx????xxxxxxxxxx????xxxxxxx?xxxxxxxxxx????xxxx?????xxxxxxxxxx????xxxxxxxxxx????xxxxxxxx?xxxxxxxxxxxxxxxxxxxxxxxxx");
SIG_SCAN(sigFileAccessWindowsCtor, "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x33\xC0\x48\x89\x43\x10\x48\x89\x43\x18\x89\x43\x20\x48\x89\x43\x28\x48\x89\x43\x30\x48\x89\x43\x38\x48\x8B\xC3\x48\x83\xC4\x20\x5B\xC3", "xxxxxxxxxx????xxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
SIG_SCAN(sigFileAccessWindowsDtor, "\x40\x57\x48\x83\xEC\x30\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x8B\xF2\x48\x8B\xD9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\xE8\x00\x00\x00\x00\x90\x48\x8B\x7B\x38\x48\x85\xFF\x74\x19\x48\x8D\x4F\xF0\xE8\x00\x00\x00\x00\x90\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCF\xE8\x00\x00\x00\x00\x90\x48\x8B\x7B\x30\x48\x85\xFF\x74\x19\x48\x8D\x4F\xF0\xE8\x00\x00\x00\x00\x90\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCF\xE8\x00\x00\x00\x00\x90\x48\x8B\x7B\x28\x48\x85\xFF\x74\x19\x48\x8D\x4F\xF0\xE8\x00\x00\x00\x00\x90\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCF\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x40\xF6\xC6\x01\x74\x0E\xBA\x00\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\xC3\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24\x00\x48\x83\xC4\x30\x5F\xC3", "xxxxxxxxxx?????xxxx?xxxx?xxxxxxxx????xxxx????xxxxxxxxxxxxxxx????xxxxxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxxx????xxxx????xxxxxxxxxx????xxxx????xxxxxxxx?xxxx?xxxxxx");
SIG_SCAN(sigFileAccessOpen, "\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x60\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x41\x8B\xF0\x48\x8B\xEA\x48\x8B\xF9\x48\x83\xC1\x30\xE8\x00\x00\x00\x00\x45\x33\xE4\x4C\x89\xA4\x24\x00\x00\x00\x00\x48\x8D\x57\x30\x48\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x4C\x8B\xC5\x48\x8D\x94\x24\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x90\x48\x8B\xD0\x48\x8D\x4F\x28\xE8\x00\x00\x00\x00\x4C\x89\xA4\x24\x00\x00\x00\x00\x48\x8D\x57\x28\x48\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x83\x7F\x00\x00\x74\x09\x48\x8B\x07\x48\x8B\xCF\xFF\x50\x10\x83\xFE\x01\x75\x09\x48\x8D\x2D\x00\x00\x00\x00\xEB\x2C\x83\xFE\x02\x75\x09\x48\x8D\x2D\x00\x00\x00\x00\xEB\x1E\x83\xFE\x03\x75\x09\x48\x8D\x2D\x00\x00\x00\x00\xEB\x10\x83\xFE\x07\x0F\x85\x00\x00\x00\x00\x48\x8D\x2D\x00\x00\x00\x00\x48\x8D\x4F\x28\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x85\xC0\x75\x10\xB8\x00\x00\x00\x00\x66\x85\x44\x24\x00\x0F\x84\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x40\xF6\xC6\x02\x0F\x84\x00\x00\x00\x00\x40\xF6\xC6\x01\x0F\x85\x00\x00\x00\x00\x48\x8D\x57\x28\x48\x8D\x4F\x38\xE8\x00\x00\x00\x00\x4C\x89\xA4\x24\x00\x00\x00\x00\x48\x8D\x57\x38\x48\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x4C\x8D\x84\x24\x00\x00\x00\x00\x48\x8D\x94\x24\x00\x00\x00\x00\x48\x8D\x4F\x28\xE8\x00\x00\x00\x00\x90\x48\x8B\xD0\x48\x8D\x4F\x28\xE8\x00\x00\x00\x00\x4C\x89\xA4\x24\x00\x00\x00\x00\x48\x8D\x57\x28\x48\x8D\x8C\x24\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x85\xDB\x74\x18\x48\x8D\x4B\xF0\xE8\x00\x00\x00\x00\x85\xC0\x75\x0B\xB2\x01\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8D\x4F\x28\xE8\x00\x00\x00\x00\x4C\x8B\xC5\x48\x8B\xD0\x48\x8D\x4F\x10\xE8\x00\x00\x00\x00\x48\x83\x7F\x00\x00\x75\x1B\xC7\x47\x00\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x48\x83\xC4\x60\x41\x5F\x41\x5E\x41\x5C\x5F\x5E\x5D\x5B\xC3\x44\x89\x67\x20\x89\x77\x18\x33\xC0\x48\x83\xC4\x60\x41\x5F\x41\x5E\x41\x5C\x5F\x5E\x5D\x5B\xC3\xB8\x00\x00\x00\x00\x48\x83\xC4\x60\x41\x5F\x41\x5E\x41\x5C\x5F\x5E\x5D\x5B\xC3", "xxxxxxxxxxxxxxxxxxx?????xxxxxxxxxxxxxx????xxxxxxx????xxxxxxxx????x????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxxxxx????xxxx????xxxxxxxxx????xxxx????xxxxxxxx????x????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxx??xxxxxxxxxxxxxxxxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxxxx????xxx????xxxxx????xxxxxxx?x????xxxxx????xxxx?xx????xx?????xx????xxxxxx????xxxxxx????xxxxxxxxx????xxxx????xxxxxxxx????x????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxx????xxxx????x????xxxxx????xxxx????xxxxx????xxxxxxxxx????xxxx????xxxxxxxx????x????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxxx????xxxxxxxxxxx????xxx??xxxx?????x????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxx");