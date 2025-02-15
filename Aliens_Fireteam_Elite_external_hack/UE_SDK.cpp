#include "Memory.h"

#include "UE_SDK.hpp"

#define DEBUG_SDK false
#define ERROR_TRACE false

namespace UE_SDK {
    bool UObject::findName() {
        pSdk->getFName(this->name, this->asciiName);
        if (strlen(this->asciiName) <= 1) {
            ERROR_TRACE&& printf("[!] No name found...\n");
            return false;
        }
        return true;
    }
    bool UObject::isUObject(uintptr_t pTarget) {
        bool result = true;
        UObject* uObject = (UObject*)malloc(sizeof(UObject));
        DEBUG_SDK&& printf("pTarget: %p, hProcess: %x, moduleBaseAddress: %p\n", pTarget, pSdk->mem->hProcess, (LPVOID)pSdk->mem->moduleBaseAddress);
        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)pTarget, uObject, sizeof(UObject), NULL)) {
            ERROR_TRACE&& printf("[!][isUObject] Could not save UOBject (%p) to %p. Error: %u\n", pTarget, uObject, GetLastError());
            free(uObject);
            return false;
        }

        if (pSdk->mem->IsBadReadPtr((LPVOID)uObject->pClassPrivate) || pSdk->mem->IsBadReadPtr((LPVOID)uObject->vTable)) {
            ERROR_TRACE&& printf("[!][isUObject] ClassPrivate (%p) or VTable (%p) is not a valid pointer. Error: %u\n", uObject->pClassPrivate, uObject->vTable, GetLastError());
            free(uObject);
            return false;
        }

        result = uObject->findName();


        free(uObject);
        return result;
    }

    bool UProperty::findName() {
        pSdk->getFName(this->name, this->asciiName);
        if (strlen(this->asciiName) <= 1) {
            ERROR_TRACE&& printf("[!] No name found...\n");
            return false;
        }



        return true;
    }
    bool UProperty::isUProperty(uintptr_t pTarget) {
        bool result = true;
        UProperty* uProperty = (UProperty*)malloc(sizeof(UProperty));
        memset(uProperty, 0, sizeof(uProperty));
        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)pTarget, uProperty, sizeof(UProperty), NULL)) {
            ERROR_TRACE&& printf("[!][isUProperty] Could not save UProperty (%p) to %p. Error: %u\n", pTarget, uProperty, GetLastError());
            free(uProperty);
            return false;
        }

        DEBUG_SDK&& printf("NameIndex: %p\n", uProperty->name);

        if (pSdk->mem->IsBadReadPtr((LPVOID)uProperty->pOwner) || pSdk->mem->IsBadReadPtr((LPVOID)uProperty->vTable)) {
            ERROR_TRACE&& printf("[!][isUProperty] pOwner (%p) or VTable (%p) is not a valid pointer. Error: %u\n", uProperty->pOwner, uProperty->vTable, GetLastError());
            free(uProperty);
            return false;
        }

        if (uProperty->pOwner == 0x0 && uProperty->vTable == 0x0 && uProperty->name == 0x0) {
            ERROR_TRACE&& printf("[!][isUProperty] is empty\n");
            return false;
        }



        result = uProperty->findName();

        if (strncmp(uProperty->asciiName, "None", 4) == 0) {
            result = false;
        }
        free(uProperty);
        return result;
    }


    BOOL UE_SDK::Remote_SDK::getFName(UINT64 id, char* out) {
        UINT16 chunkId = id >> 16;
        DEBUG_SDK && printf("[%x] Chunk: %u, pFNamePool: %p\n", id, chunkId, pFNamePool);

        void* pChunk = (void*)pFNamePool; //RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)pFNamePool);

        DEBUG_SDK && printf("[%x] ChunkId: %d pChunk: %p result: %p\n", id, chunkId, pChunk, (uintptr_t)pChunk + (uintptr_t)0x10 + (uintptr_t)chunkId * 8);


        void* chunk = RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)((char*)pChunk + (char)0x10 + (char)chunkId * 8));
        DEBUG_SDK && printf("[%x] chunkId: %d; chunk: %p\n", id, chunkId, chunk);


        UINT64 pFNameEntry = 0x000000000001FFFF & (id * 2);
        DEBUG_SDK && printf("[%x] nameOffset: %p\n", id, pFNameEntry);

        char* pName = (char*)((uintptr_t)chunk + (uintptr_t)pFNameEntry);
        DEBUG_SDK && printf("[%x] chunk(%p) + pFNameEntry (%p) + 0x2 = %p\n", id, chunk, pFNameEntry, pName);
        DEBUG_SDK && printf("[%x] pName: %p Length: %u\n", id, pName);

        char tempName[NAME_LENGTH] = { 0 };

        if (!ReadProcessMemory(pSdk->mem->hProcess, pName, tempName, NAME_LENGTH, NULL)) {
            ERROR_TRACE && printf("[!][Get-Temp-Name] Could not read memory! Error: %u\n", GetLastError());
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
            ERROR_TRACE&& printf("[!][Get-Name] Not found: id:%u chunkId:%u pChunk:%p pFname: %p\n", id, chunkId, chunk, pName);
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

        DEBUG_SDK&& printf("Found nameIndex: %p at: %p\n", nameIndex, (UINT64*)((char*)current + 0x18));

        pSdk->getFName(nameIndex, name);


        return nameIndex;
    }



    bool UE_SDK::printAllFNames(HANDLE hProcess, LPVOID baseAddress) {
        char name[NAME_LENGTH] = { 0 };

        if (!pSdk->getFName((UINT64)0x00061299, name)) {
            ERROR_TRACE&& printf("[!][Get-Name] Could not get id: %x", 0x00061299);
        }



        return true;
    }

    bool UE_SDK::traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel) {
        if (level == maxLevel) return true;
        UObject* pUOBject = (UObject*)malloc(size);
        memset(pUOBject, 0, size);

        DEBUG_SDK&& printf("Reading _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);


        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)_pUObject, (LPVOID)pUOBject, size, NULL)) {
            ERROR_TRACE&& printf("[!][Read-UObject] Could not read memory (0x%p)\n", _pUObject);
            return false;
        }
        DEBUG_SDK&& printf("Saved _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);
        DEBUG_SDK&& printf("Getting name for UOBject %p. Name index: %x\n", _pUObject, pUOBject->name);

        // Assert that UObject points to a valid UObject
        char UObjectName[NAME_LENGTH] = { 0 };
        if (!pSdk->getFName(pUOBject->name, UObjectName)) {
            ERROR_TRACE&& printf("[!][Get-Name] Could not get name for uobject (0x%p)!\n", _pUObject);
            return false;
        }

        DEBUG_SDK&& printf("Name for UOBject %p: %s\n", _pUObject, UObjectName);


        if (pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassPrivate) || pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassOuter)) {
            ERROR_TRACE&& printf("[!] pClassPrivate (%p) or pClassOuter (%p) is not a valid pointer ....\n", pUOBject->pClassPrivate, pUOBject->pClassOuter);
            return false;
        }

        DEBUG_SDK&& printf("[%s]\n", UObjectName);


        for (int i = 0x20; i < size; i = i + 8) {

            uintptr_t ptr = *reinterpret_cast<uintptr_t*>((uintptr_t)(char*)pUOBject + i);

            DEBUG_SDK&&  printf("Child QWORD (%p) (0x%x)\n", ptr, i);


            if (pSdk->mem->IsBadReadPtr((LPVOID)ptr)) {
                ERROR_TRACE&& printf("[!] Child QWORD (%p) (0x%x) is not a valid pointer ....\n", ptr, i);
                continue;
            };


            if (ptr > 0x7FF000000000) {
                // Function

                for (int i = 0; i < level; i++) {
                    printf("-");
                }
                printf("- [%p] [0x%x] Function\n", ptr, i);

            }
            else if (UProperty::isUProperty(ptr)) {
                UProperty* next = (UProperty*)ptr;
                printf("---");
                UProperty* uProperty = (UProperty*)malloc(sizeof(UProperty));
                do {
                    DEBUG_SDK&& printf("Next UProperty: %p\n", next);

                    memset(uProperty, 0, sizeof(uProperty));

                    if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)next, uProperty, sizeof(UProperty), NULL)) {
                        ERROR_TRACE&& printf("[!][isUProperty] Could not save UProperty (%p) to %p. Error: %u\n", next, uProperty, GetLastError());
                        free(uProperty);
                        continue;
                    }

                    uProperty->findName();


                    printf("- [%p] [0x%x] [UProperty] %s\n", next, i, uProperty->asciiName);
                    next = uProperty->pNext;
                    DEBUG_SDK&& printf("Next UProperty2: %p\n", next);

                } while (next != nullptr);

                
            }
            else if (UObject::isUObject(ptr)) {
                UObject* pChildUOBject = (UObject*)malloc(size);
                memset(pChildUOBject, 0, size);
                DEBUG_SDK&& printf("Found UOBject at (%p). Reading UBOject into %p\n", ptr, pChildUOBject);

                if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)ptr, (LPVOID)pChildUOBject, size, NULL)) {
                    ERROR_TRACE&& printf("[!][Read-UObject] Could not read pUObject (%p)\n", pChildUOBject);
                    free(pChildUOBject);
                    continue;
                }

                DEBUG_SDK&& printf("Wrote UOBject (%p) into %p\n", ptr, pChildUOBject);


                if (pSdk->mem->IsBadReadPtr((LPVOID)pChildUOBject->pClassPrivate)) {
                    DEBUG_SDK&& printf("[!][Bad-Pointer] pUOBject pClassPrivate (%p) is a bad pointer\n", pChildUOBject->pClassPrivate);
                    free(pChildUOBject);
                    continue;
                }

                DEBUG_SDK&& printf("UOBject (%p) seems to be valid. Getting Name for Index %p\n", ptr, pChildUOBject->name);


                // Object/Class
                char UObjectName[NAME_LENGTH] = { 0 };
                if (!pSdk->getFName(pChildUOBject->name, UObjectName)) {
                    ERROR_TRACE&& printf("[!][Get-Name] Could not get name for uobject!\n");
                    free(pChildUOBject);
                    continue;
                }

                DEBUG_SDK&& printf("UOBject (%p) has Name: %s\n", ptr, UObjectName);


                for (int i = 0; i < level; i++) {
                    printf("-");
                }
                printf("- [%p] [0x%x] [UObject] %s\n", ptr, i, UObjectName);


                uintptr_t child = { 0 };

                //if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)((char*)_pUObject + i), &child, sizeof(uintptr_t), NULL)) {
                //    ERROR_TRACE&& printf("Could not read child UOBject %p\n", (char*)_pUObject + i);
                //    continue;
                //}
                DEBUG_SDK&& printf("Traversing into (%p)\n", ptr);
                free(pChildUOBject);

                traverseUObjectForMembersEtc(ptr, 0xDD, level + 1, maxLevel);
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
            ERROR_TRACE&& printf("[!][Read GOBjects] Could get read GOBjects array! Error %u\n", GetLastError());
            return false;
        }

        return UE_SDK::printFNameForUObjects(GObjectsArray, arraySize);
    }




    void UE_SDK::init() {
        printf("hello");
    }
}