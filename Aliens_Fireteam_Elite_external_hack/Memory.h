#pragma once

#ifndef MEMORY_AFT

#define MEMORY_AFT
#include <windows.h>
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



#endif // !MEMORY_AFT