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



class Mem {
public:
    HANDLE hProcess = NULL;
    DWORD processPid = NULL;
    const wchar_t* processName = NULL;
    Mem(HANDLE _hProcess, const wchar_t* processName) {
        hProcess = _hProcess;
        this->processPid = this->GetRemoteProcessPid(processName);
        this->processName = processName;
    }
    template <typename MemType> MemType readRemote(MemType* address) {
        return readRemote(address, sizeof(MemType));
    }
    template <typename MemType> MemType readRemote(MemType* address, size_t size) {
        MemType buf = { 0 };
        DWORD oldProtect;

        if (IsBadReadPtr(hProcess, address)) {
            return NULL;
        }

        if (!ReadProcessMemory(hProcess, address, &buf, size, NULL)) {
            printf("[!] Could not read memory! Error: %u\n", GetLastError());
            return NULL;
        }

        return buf;
    }
    LPVOID static getDynamicMemoryAddress(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength);
    LPVOID getDynamicMemoryAddress(LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength);
    LPVOID getDynamicMemoryAddress(LPVOID* pointerChain, UINT chainLength);
    uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName);
    DWORD GetRemoteProcessPid(const wchar_t* processName);
    bool static IsBadReadPtr(HANDLE hProcess, LPVOID p);
    bool IsBadReadPtr(LPVOID p);
};


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

        if (Mem::IsBadReadPtr(hProcess, address)) {
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