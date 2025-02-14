#pragma once

#ifndef MEMORY_AFT

#define MEMORY_AFT
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>


struct MemoryPatch {
	char* originalCode;
	char* patchCode;
	size_t size;
	uintptr_t address;
};
DWORD GetAlienFireteamElitePid();
uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName);
LPVOID getDynamicMemoryAddress(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength);
BOOL writeMemory(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT8 pointerChainSize, char buffer[], int bufferSize);

BOOL patchMemory(HANDLE hProcess, LPVOID baseAddress, MemoryPatch mp);


bool IsBadReadPtr(HANDLE hProcess, LPVOID p);

class RemoteMem {
public:
    HANDLE hProcess = NULL;
    RemoteMem(HANDLE _hProcess) {
        hProcess = _hProcess;
    }
    RemoteMem() {}
    template <typename MemType> MemType read(HANDLE hProcess, MemType* address) {
        MemType buf = { 0 };
        DWORD oldProtect;

        if (IsBadReadPtr(hProcess, address)) {
       //     printf("[!] %p is a bad pointer!\n", address);
            return NULL;
        }

        if (!ReadProcessMemory(hProcess, address, &buf, sizeof(MemType), NULL)) {
            printf("[!] Could not read memory! Error: %u\n", GetLastError());
            return NULL;
        }

        return buf;
    }
};



#endif // !MEMORY_AFT