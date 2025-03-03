#include "Memory.h"
#include <iostream>
#include <stdio.h>
#include "log.hpp"
#define PROCESS_NAME L"Endeavor-Win64-Shipping.exe"
#define DEBUG_AFE true

uintptr_t GetModuleBaseAddress(DWORD pid, const wchar_t moduleName[]) {
    MODULEENTRY32 moduleEntry = { sizeof(MODULEENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (Module32First(snapshot, &moduleEntry)) {
        do {
            if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                CloseHandle(snapshot);
                return (uintptr_t)moduleEntry.modBaseAddr;
            }
        } while (Module32Next(snapshot, &moduleEntry));
    }
    CloseHandle(snapshot);
    return 0;
}

DWORD GetAlienFireteamElitePid() {
    PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    DWORD found = 0;
    if (Process32First(snapshot, &process)) {
        do {
            if (wcsncmp(process.szExeFile, PROCESS_NAME, sizeof(PROCESS_NAME)) == 0) {
                CloseHandle(snapshot);
                return process.th32ProcessID;
            }
        } while (Process32Next(snapshot, &process));
    }
    CloseHandle(snapshot);
    return 0;
}


// Used to dereference a pointer chain
// Usage: getDynamicMemoryAddress([0x12D,0x10,0xD]) this dereferences 0x12D, adds 10, dereferences 12D+10 adds D, dereferences the result of [12D+10]+D
LPVOID getDynamicMemoryAddress(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength);








BOOL writeMemory(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT8 pointerChainSize, char buffer[], int bufferSize) {

    LPVOID address = Mem::getDynamicMemoryAddress(hProcess, baseAddress, pointerChain, pointerChainSize);

    DWORD oldProtect = 0;



    if (!VirtualProtectEx(hProcess, address, bufferSize, PAGE_READWRITE, &oldProtect)) {
        error_trace("writeMemory", "Could not virtual protect");
    }

    BOOL writeResult = WriteProcessMemory(hProcess, address, buffer, bufferSize, NULL);

    if (!writeResult) {
        error_trace("writeMemory", "Could not write to memory");

    }


    if (!VirtualProtectEx(hProcess, address, bufferSize, oldProtect, &oldProtect)) {
        error_trace("writeMemory", "Could not restore virtual protect");

    }

    return writeResult;

}


// Accepts a MemoryPatch with only size and patchCode provided
// patchMemory() will fill the originalCode itself using ReadProcessMemory
BOOL patchMemory(HANDLE hProcess, LPVOID lpAddress, MemoryPatch mp) {


    if (!ReadProcessMemory(hProcess, lpAddress, mp.originalCode, mp.size, 0)) {
        error_trace("patch", "Could not read original code in memory");
        return false;
    }


    DWORD oldProtect;

    if (!VirtualProtectEx(hProcess, lpAddress, mp.size, PAGE_READWRITE, &oldProtect)) {
        error_trace("patch", "Could not change page permissions");
        return false;
    }


    if (!WriteProcessMemory(hProcess, lpAddress, mp.patchCode, mp.size, 0)) {
        error_trace("patch", "Could not write process memory");
        return false;
    }


    if (!VirtualProtectEx(hProcess, lpAddress, mp.size, oldProtect, &oldProtect)) {
        error_trace("patch", "Could not change page permissions back to original");
        return false;
    }

    return true;
}


bool Mem::IsBadReadPtr(HANDLE hProcess, LPVOID p)
{
    MEMORY_BASIC_INFORMATION mbi = { 0 };
    if (::VirtualQueryEx(hProcess, p, &mbi, sizeof(mbi)))
    {
        DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);
        // check the page is not a guard page
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;
        return b;
    }
    debug("[IsBadReadPtr] Could not VirtualQueryEx: %p\n", p);

    return true;
}

bool Mem::IsBadReadPtr(LPVOID p)
{
    return Mem::IsBadReadPtr(hProcess, p);
}

Mem::Mem(HANDLE _hProcess, const wchar_t processName[]) {

    hProcess = _hProcess;
    this->processPid = this->GetRemoteProcessPid(processName);
    this->processName = processName;
}


uintptr_t Mem::GetModuleBaseAddress(DWORD pid, const wchar_t moduleName[]) {
    MODULEENTRY32 moduleEntry = { sizeof(MODULEENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (Module32First(snapshot, &moduleEntry)) {
        do {
            if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                CloseHandle(snapshot);
                return (uintptr_t)moduleEntry.modBaseAddr;
            }
        } while (Module32Next(snapshot, &moduleEntry));
    }
    CloseHandle(snapshot);
    return 0;
}

DWORD Mem::GetRemoteProcessPid(const wchar_t* processName) {
    PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    DWORD found = 0;
    if (Process32First(snapshot, &process)) {
        do {
            if (wcsncmp(process.szExeFile, processName, sizeof(processName)) == 0) {
                CloseHandle(snapshot);
                return process.th32ProcessID;
            }
        } while (Process32Next(snapshot, &process));
    }
    CloseHandle(snapshot);
    return 0;
}

LPVOID Mem::getDynamicMemoryAddress(LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength) {
    return Mem::getDynamicMemoryAddress(hProcess, baseAddress, pointerChain, chainLength);
}

LPVOID Mem::getDynamicMemoryAddress(LPVOID* pointerChain, UINT chainLength) {
    return Mem::getDynamicMemoryAddress((LPVOID)moduleBaseAddress, pointerChain, chainLength);
}

// Used to dereference a pointer chain
// Usage: getDynamicMemoryAddress([0x12D,0x10,0xD]) this dereferences 0x12D, adds 10, dereferences 12D+10 adds D, dereferences the result of [12D+10]+D
LPVOID Mem::getDynamicMemoryAddress(HANDLE hProcess, LPVOID baseAddress, LPVOID* pointerChain, UINT chainLength) {
    int arraylength = chainLength;

    uintptr_t intermediaryPointerBuffer = NULL;
    char buffer[1024] = { 0 };
    LPVOID base = (LPVOID)((uintptr_t)pointerChain[0] + (uintptr_t)baseAddress);

    if (chainLength == 1) {
        return pointerChain[0];
    }

    debug("First chain element: %p\n", pointerChain[0]);
    debug("Module base: %p\n", baseAddress);
    debug("intermediatPointerBuffer: %p\n", intermediaryPointerBuffer);


    debug("Reading initial base: %p\n", base);

    if (!ReadProcessMemory(hProcess, base, &intermediaryPointerBuffer, sizeof(uintptr_t), NULL)) {
        error_trace("getDynamicMemoryAddress", "Could not read memory, aborting ....");
        return 0;
    }

    debug("Base pointer: %p\n", intermediaryPointerBuffer);
    // Leave out the first and last entry. The first is instantly dereferenced and the last is not dereferenced
    for (int i = 1; i < arraylength - 1; i++) {



        debug("pointer: %p\n", intermediaryPointerBuffer);

        LPVOID newPointer = LPVOID((uintptr_t)intermediaryPointerBuffer + (uintptr_t)pointerChain[i]);
        debug("Dereferencing pointer at level %d: %p \n", i, intermediaryPointerBuffer);

        debug("Adding offset: %p + %x -> %p\n", intermediaryPointerBuffer, (int)pointerChain[i], newPointer);

        if (!ReadProcessMemory(hProcess, newPointer, &intermediaryPointerBuffer, sizeof(intermediaryPointerBuffer), NULL)) {
            std::cout << "Could not read memory, aborting ...." << std::endl;
            return 0;
        }

    }


    LPVOID finalAddress = LPVOID((uintptr_t)intermediaryPointerBuffer + (uintptr_t)pointerChain[arraylength - 1]);

    debug("Final result: %p\n", finalAddress);

    return finalAddress;
}

constexpr bool compareArrays(char array1[], char array2[], size_t size) {
    for (int i = 0; i < size; i++) {
        bool sameItem = array1[i] == array2[i];
        bool hasWildcard = array2[i] == '??' || array1[i] == '??';
        if (!sameItem && !hasWildcard) {
            return false;
        }
    }
    return true;
}

// Amateur find algo
UINT64 find(char fullArray[], size_t fullArrayLength, char arrayToFind[], size_t arrayToFindLength) {
    for (int i = 0; i < fullArrayLength; i++) {
        if (i + arrayToFindLength > fullArrayLength) {
            return 0;
        }
        if (compareArrays(fullArray + i, arrayToFind, arrayToFindLength)) {
            return i;
        }
    }
    return 0;
}

uintptr_t Mem::findPattern(char pattern[], size_t patternSize) {

    /* // Find test
    char arr1[] = {0x00, 0x00, 0x12, 0x34, 0x00, 0x56, 0x00};
    char arr2[] = { 0x12, 0x34, 0x00, 0x56, 0x00};
    char arr3[] = { 0x12 };

    return find(arr1, 7, arr2, 5, 0);
    return 0;
    */

    UINT64 found = NULL;
    LPCVOID baseAddress = (LPCVOID)moduleBaseAddress;

    do {
        MEMORY_BASIC_INFORMATION mbi = { 0 };
        if (!VirtualQueryEx(hProcess, baseAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) {
            printf("[!][findPattern] Could not VirtualQuery\n");
            return NULL;
        }

        if ((mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) > 0) {
            printf("[!][findPattern] Page not accessible protect: 0x%x\n", mbi.Protect);
            return NULL;
        }


        char* buffer = (char*)malloc(mbi.RegionSize);
        memset(buffer, 0, mbi.RegionSize);
        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(hProcess, baseAddress, buffer, mbi.RegionSize, &bytesRead)) {
            printf("[!][findPattern] Could not read memory (%u bytes) at %p\n", mbi.RegionSize, baseAddress);
            return false;
        }

        if (bytesRead == 0) {
            printf("[!][findPattern] Read no bytes at %p\n", baseAddress);
            return false;
        }



        found = find(buffer, mbi.RegionSize, pattern, patternSize);
        free(buffer);
        if (!found) baseAddress = (char*)baseAddress + mbi.RegionSize;
    } while (found == 0);

    if (found) {
        return (uintptr_t)baseAddress + found;
    }

    return NULL;
}