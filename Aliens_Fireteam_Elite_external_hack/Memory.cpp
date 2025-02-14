#include "Memory.h"
#include <iostream>
#include <stdio.h>
#define PROCESS_NAME L"Endeavor-Win64-Shipping.exe"
#define DEBUG_AFE false

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
        std::cout << "Could not VirtualProtect" << std::endl;
        std::cout << GetLastError() << std::endl;
    }

    BOOL writeResult = WriteProcessMemory(hProcess, address, buffer, bufferSize, NULL);

    if (!writeResult) {
        std::cout << "Could not write memory" << std::endl;
        std::cout << GetLastError() << std::endl;
    }


    if (!VirtualProtectEx(hProcess, address, bufferSize, oldProtect, &oldProtect)) {
        std::cout << "Could not restore VirtualProtect" << std::endl;
        std::cout << GetLastError() << std::endl;
    }

    return writeResult;

}


// Accepts a MemoryPatch with only size and patchCode provided
// patchMemory() will fill the originalCode itself using ReadProcessMemory
BOOL patchMemory(HANDLE hProcess, LPVOID lpAddress, MemoryPatch mp) {


    if (!ReadProcessMemory(hProcess, lpAddress, mp.originalCode, mp.size, 0)) {
        std::cout << "Could not read original code in memory" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }


    DWORD oldProtect;

    if (!VirtualProtectEx(hProcess, lpAddress, mp.size, PAGE_READWRITE, &oldProtect)) {
        std::cout << "Could not change page permissions" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }


    if (!WriteProcessMemory(hProcess, lpAddress, mp.patchCode, mp.size, 0)) {
        std::cout << "Could not write process memory" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }


    if (!VirtualProtectEx(hProcess, lpAddress, mp.size, oldProtect, &oldProtect)) {
        std::cout << "Could not change page permissions back to original" << std::endl;
        std::cout << GetLastError() << std::endl;
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
    return Mem::getDynamicMemoryAddress((LPVOID)GetModuleBaseAddress(processPid, processName), pointerChain, chainLength);
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

    DEBUG_AFE&& std::cout << "First chain element: " << pointerChain[0] << std::endl;
    DEBUG_AFE&& std::cout << "Module base: " << baseAddress << std::endl;
    DEBUG_AFE&& std::cout << "intermediatPointerBuffer: " << intermediaryPointerBuffer << std::endl;


    DEBUG_AFE&& std::cout << "Reading initial base: " << base << std::endl;

    if (!ReadProcessMemory(hProcess, base, &intermediaryPointerBuffer, sizeof(uintptr_t), NULL)) {
        std::cout << "Could not read memory, aborting ...." << std::endl;
        std::cout << GetLastError() << std::endl;
        return 0;
    }

    DEBUG_AFE&& printf("Base pointer: %p\n", intermediaryPointerBuffer);
    // Leave out the first and last entry. The first is instantly dereferenced and the last is not dereferenced
    for (int i = 1; i < arraylength - 1; i++) {



        DEBUG_AFE&& printf("pointer: %p\n", intermediaryPointerBuffer);

        LPVOID newPointer = LPVOID((uintptr_t)intermediaryPointerBuffer + (uintptr_t)pointerChain[i]);
        DEBUG_AFE&& printf("Dereferencing pointer at level %d: %p \n", i, intermediaryPointerBuffer);

        DEBUG_AFE&& printf("Adding offset: %p + %x -> %p\n", intermediaryPointerBuffer, (int)pointerChain[i], newPointer);

        if (!ReadProcessMemory(hProcess, newPointer, &intermediaryPointerBuffer, sizeof(intermediaryPointerBuffer), NULL)) {
            std::cout << "Could not read memory, aborting ...." << std::endl;
            return 0;
        }

    }


    LPVOID finalAddress = LPVOID((uintptr_t)intermediaryPointerBuffer + (uintptr_t)pointerChain[arraylength - 1]);

    DEBUG_AFE&& printf("Final result: %p\n", finalAddress);

    return finalAddress;
}