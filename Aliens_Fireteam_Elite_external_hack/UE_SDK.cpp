#include "Memory.h"

#include "UE_SDK.hpp"

#define DEBUG_SDK false

namespace UE_SDK {

    BOOL UE_SDK::Remote_SDK::getFName(UINT64 id, char* out) {
        UINT16 chunkId = id >> 16;
        DEBUG_SDK && printf("[%x] Chunk: %u, pFNamePool: %p\n", id, chunkId, pFNamePool);

        void* pChunk = RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)pFNamePool);

        DEBUG_SDK && printf("[nameIndex: %x] ChunkId: %d pChunk: %p result: %p\n", id, chunkId, pChunk, (uintptr_t)pChunk + (uintptr_t)0x10 + (uintptr_t)chunkId * 8);


        void* chunk = RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)((char*)pChunk + (char)0x10 + (char)chunkId * 8));
        DEBUG_SDK && printf("[%x] chunkId: %d; chunk: %p\n", id, chunkId, chunk);


        UINT64 pFNameEntry = 0x000000000001FFFF & (id * 2);
        DEBUG_SDK && printf("[%x] nameOffset: %u\n", id, pFNameEntry);

        char* pName = (char*)((uintptr_t)chunk + (uintptr_t)pFNameEntry);
        DEBUG_SDK && printf("[%x] chunk(%p) + pFNameEntry (%p) + 0x2 = %p\n", id, chunk, pFNameEntry, pName);
        DEBUG_SDK && printf("[%x] pName: %p Length: %u\n", id, pName);

        char tempName[NAME_LENGTH] = { 0 };

        if (!ReadProcessMemory(pSdk->mem->hProcess, pName, tempName, NAME_LENGTH, NULL)) {
            printf("[!] Could not read memory! Error: %u\n", GetLastError());
            return false;
        }

        // Name legally contains / | . _ -


        int skip = 0;

        if (tempName[2] <= 0x2D)
        {
            tempName[2] = '_';
            skip = 3;
        }
        else if (tempName[1] <= 0x2D) {
            tempName[1] = '_';
            skip = 2;
        }
        else if (tempName[0] <= 0x2D) {
            tempName[0] = '_';
            skip = 1;
        }


        if (strncmp(tempName, "y", 1) == 0) {
            return true;
        }

        for (int i = 2; i < strlen(tempName); i++) {
            if (tempName[i] <= 0x2D) {
                tempName[i] = 0x00;
            }
        }

        if (strlen((char*)tempName + skip) == 1) {
            return true;
        }

        memcpy(out, (char*)tempName + skip, strlen(tempName));
        if (strlen(out) == 0) {
            printf("[!] Not found: id:%u chunkId:%u pChunk:%p pFname: %p\n", id, chunkId, chunk, pName);
        }
        DEBUG_SDK && printf("Name: %s\n", out);
        return true;
    }





    UE_SDK::Remote_SDK::Remote_SDK(uintptr_t pGObjects, uintptr_t pFNamePool) {
        this->pGObjectArray = pGObjects;
        this->pFNamePool = pFNamePool;
        pSdk = this;
    }

    void UE_SDK::Remote_SDK::initMem(Mem* mem) {
        this->mem = mem;
        this->moduleBaseAddress = mem->GetModuleBaseAddress(mem->processPid, mem->processName);
    }



    bool UE_SDK::printAllFNames(HANDLE hProcess, LPVOID baseAddress) {
        char name[NAME_LENGTH] = { 0 };

        if (!pSdk->getFName((UINT64)0x00061299, name)) {
            printf("Could not get id: %x", 0x00061299);
        }



        return true;
    }


    UINT64 UE_SDK::Remote_SDK::getFNameForUObject(uintptr_t uObject, char name[NAME_LENGTH]) {
        auto current = uObject;
        if (current < 0xFFFFFF || current > 0x7FF000000000) return 0;
        DEBUG_SDK&& printf("UObject at : %p\n", current);
        UINT64 nameIndex = RemoteMem().read<UINT64>(mem->hProcess, (UINT64*)((char*)current + 0x18));
        if (nameIndex == NULL) {
            //  printf("[!] Not able to get nameindex!\n");
            return 0;
        }

        if (nameIndex > 0xFFFFFFF) {
            //  printf("[!] NameIndex too big!\n");
            return 0;
        }

        DEBUG_SDK&&  printf("Found nameIndex: %p at: %p\n", nameIndex, (UINT64*)((char*)current + 0x18));

        pSdk->getFName(nameIndex, name);


        return nameIndex;
    }


    bool UE_SDK::traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size) {
        UObject* pUOBject = (UObject*)malloc(size);

        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)_pUObject, (LPVOID)pUOBject, size, NULL)) {
            printf("[!] Could not read memory (0x%p)\n", _pUObject);
            return false;
        }

        // Assert that UObject points to a valid UObject
        char UObjectName[NAME_LENGTH] = { 0 };
        if (!pSdk->getFName(pUOBject->name, UObjectName)) {
            printf("[!] Could not get name for uobject (0x%p)!\n", _pUObject);
            return false;
        }


        if (pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassPrivate) || pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassOuter)) {
            printf("pClassPrivate (%p) or pClassOuter (%p) is not a valid pointer ....\n", pUOBject->pClassPrivate, pUOBject->pClassOuter);
            return false;
        }

        printf("[%s]\nFlags: %u\nClassPrivate: %p\nClassOuter: %p\n", UObjectName, pUOBject->flags, pUOBject->pClassPrivate, pUOBject->pClassOuter);


        for (int i = sizeof(UObject); i < size; i = i + 8) {

            uintptr_t ptr = *reinterpret_cast<uintptr_t*>((uintptr_t)(char*)pUOBject + i);

            if (pSdk->mem->IsBadReadPtr((LPVOID)ptr)) continue;


            if (ptr > 0x7FF000000000) {
                // Function
            }
            else {


                UObject* pUOBject = (UObject*)malloc(size);

                if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)ptr, (LPVOID)pUOBject, size, NULL)) {
                    continue;
                }

                if (pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassPrivate) || pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassOuter)) {
                    continue;
                }


                // Object/Class
                char UObjectName[NAME_LENGTH] = { 0 };
                if (!pSdk->getFName(pUOBject->name, UObjectName)) {
                    printf("[!] Could not get name for uobject!\n");
                    continue;
                }
                printf("-[%p] %s (0x%x)\n", ptr, UObjectName, i);


                uintptr_t child = { 0 };

                if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)((char*)_pUObject + i), &child, sizeof(uintptr_t), NULL)) {
                    continue;
                }

                //traverseUObjectForMembersEtc(hProcess, baseAddress, child, 0x10);
                free(pUOBject);
            }




        }



        free(pUOBject);
    }


    bool UE_SDK::printFNameForUObjects(uintptr_t* uObjects, size_t size) {

        printf("[index] [nameIndex] [pUObject] name\m");
        for (int i = 0; i < size; i++) {
            auto current = uObjects[i];

            // Get vtablePointer to validate if UObject is a class
            uintptr_t vtablePointer = RemoteMem().read<uintptr_t>(pSdk->mem->hProcess, (uintptr_t*)current);
            if (vtablePointer < 0x00007FF000000000) continue;
            char name[NAME_LENGTH] = { 0 };

            UINT64 nameIndex = pSdk->getFNameForUObject(current, name);
            if (!nameIndex) continue;
            if (strlen(name) > 0) {
                printf("[%d] [%p] [%p] %s (%u)\n", i, nameIndex, current, name, strlen(name));
            }
        }
        return true;
    }


    bool UE_SDK::printAllGObjects() {
        //LPVOID addr = getDynamicMemoryAddress(hProcess, baseAddressDataCollector, GObjectsPointerchain, GObjectsPointerchainSize);

        const size_t arraySize = 800000;
        uintptr_t pGObjectsController = 0x00007FF7D5357A08;

        printf("addr: %p hProcess: %p\n", (char*)pGObjectsController + 0x350, pSdk->mem->hProcess);
        uintptr_t* GObjectsArray = (uintptr_t*)calloc(arraySize, sizeof(uintptr_t));

        if (!ReadProcessMemory(pSdk->mem->hProcess, (char*)pGObjectsController + 0x350, GObjectsArray, arraySize * sizeof(uintptr_t), NULL)) {
            printf("[!] Could not read memory! Error %u\n", GetLastError());
            return false;
        }

        return UE_SDK::printFNameForUObjects(GObjectsArray, arraySize);
    }




    void UE_SDK::init() {
        printf("hello");
    }
}