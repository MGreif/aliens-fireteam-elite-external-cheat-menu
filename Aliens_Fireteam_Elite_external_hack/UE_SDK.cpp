#include "Memory.h"
#include "log.hpp"
#include "UE_SDK.hpp"

#define DEBUG_SDK false
#define ERROR_TRACE false
#define MAX_PROPERTIES 8924
#define PROP_PREFIX_LENGTH 1024

/// <summary>
/// Unreal Engine Software Development Kit
/// </summary>
namespace UE_SDK {
    /// <summary>
    /// Populates the UObject instance with the found FNamePool name
    /// </summary>
    /// <returns>success boolean</returns>
    bool UObject::findName() {
        if (!pSdk->getFName(this->name, this->asciiName)) {
            return false;
        }
        if (strlen(this->asciiName) <= 1) {
            error_trace("UObject find name", "No name found...\n");
            return false;
        }
        return true;
    }

    /// <summary>
    /// Assertion function to validate if a UObject pointer is a valid UObject.
    /// </summary>
    /// <param name="pTarget">Target pointer</param>
    /// <returns>Boolean if the pointer points to a valid UObject</returns>
    bool UObject::isUObject(uintptr_t pTarget) {
        bool result = true;
        debug("Getting UObject for %p\n", pTarget);
        UObject uObject = UObject::from((UObject*)pTarget);

        if (uObject.isEmpty()) {
            error_trace("isUObject", "Could not instantiate UObject\n");
            return false;
        }

        if (pSdk->mem->IsBadReadPtr((LPVOID)uObject.pClassPrivate) || pSdk->mem->IsBadReadPtr((LPVOID)uObject.vTable)) {
            error_trace("isUObject", "ClassPrivate (%p) or VTable (%p) is not a valid pointer. Error: %u\n", uObject.pClassPrivate, uObject.vTable, GetLastError());
            return false;
        }

        return result;
    }
    bool UObject::isEmpty() {
        UObject empty = { 0 };

        if (memcmp(this, &empty, sizeof(UObject)) == 0) {
            return true;
        }
        return false;
    }

    /// <summary>
    /// Creates a local UObject copy populated with the FNamePool name for it.
    /// </summary>
    /// <param name="pTarget"></param>
    /// <returns></returns>
    UObject UObject::from(UObject* pTarget) {
        UObject obj = pSdk->mem->readRemote(pTarget);
        if (!obj.findName()) {
            return UObject{0};
        }

        return obj;
    }

    /// <summary>
    /// Populates the UProperty instance with the found FNamePool name
    /// </summary>
    /// <returns></returns>
    bool UProperty::findName() {
        if (!pSdk->getFName(this->name, this->asciiName)) {
            return false;
        }
        if (strlen(this->asciiName) <= 1) {
            error_trace("UProperty find name", "No name found...\n");
            return false;
        }
        return true;
    }

    /// <summary>
    /// Assertion function to validate if a UProperty pointer is a valid property.
    /// </summary>
    /// <param name="pTarget">Target pointer</param>
    /// <returns>Boolean if the pointer points to a valid UProperty</returns>
    bool UProperty::isUProperty(uintptr_t pTarget) {
        bool result = true;
        UProperty* uProperty = (UProperty*)malloc(sizeof(UProperty));
        memset(uProperty, 0, sizeof(uProperty));
        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)pTarget, uProperty, sizeof(UProperty), NULL)) {
            error_trace("isUProperty", "Could not save UProperty (%p) to %p. Error: %u\n", pTarget, uProperty, GetLastError());
            free(uProperty);
            return false;
        }

        debug("NameIndex: %p\n", uProperty->name);

        if (pSdk->mem->IsBadReadPtr((LPVOID)uProperty->pOwner) || pSdk->mem->IsBadReadPtr((LPVOID)uProperty->vTable)) {
            error_trace("isUProperty", "pOwner (%p) or VTable (%p) is not a valid pointer. Error: %u\n", uProperty->pOwner, uProperty->vTable, GetLastError());
            free(uProperty);
            return false;
        }

        if (uProperty->pOwner == 0x0 && uProperty->vTable == 0x0 && uProperty->name == 0x0) {
            error_trace("isUProperty", "is empty\n");
            return false;
        }



        result = uProperty->findName();

        if (strncmp(uProperty->asciiName, "None", 4) == 0) {
            result = false;
        }
        free(uProperty);
        return result;
    }

    /// <summary>
    /// Checks if a UProperty is empty
    /// </summary>
    /// <returns>If the UProperty is empty</returns>
    bool UProperty::isEmpty() {
        UProperty empty = { 0 };

        if (memcmp(this, &empty, sizeof(UProperty)) == 0) {
            return true;
        }
        return false;
    }

    /// <summary>
    /// Uses the FNamePool to get the correct ASCII string for a nameIndex id.
    /// </summary>
    /// <param name="id">NameIndex id</param>
    /// <param name="out">Output name string</param>
    /// <returns>Success boolean</returns>
    BOOL UE_SDK::Remote_SDK::getFName(UINT32 id, char* out) {

        if (id == NULL) {
            error_trace("getFName","Not able to get nameindex!\n");
            return false;
        }

        if (id >= 0xFFFFFF6) {
            error_trace("getFName", "NameIndex too big: %p!\n", id);
            return false;
        }

        UINT16 chunkId = id >> 16;
        debug("[%x] Chunk: %u, pFNamePool: %p\n", id, chunkId, pFNamePool);

        void* pChunk = (void*)pFNamePool; //RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)pFNamePool);

        debug("[%x] ChunkId: %d pChunk: %p result: %p\n", id, chunkId, pChunk, (uintptr_t)pChunk + (uintptr_t)0x10 + (uintptr_t)chunkId * 8);


        void* chunk = RemoteMem().read<void*>(pSdk->mem->hProcess, (void**)((char*)pChunk + (char)0x10 + (char)chunkId * 8));
        debug("[%x] chunkId: %d; chunk: %p\n", id, chunkId, chunk);


        UINT64 pFNameEntry = 0x000000000001FFFF & (id * 2);
        debug("[%x] nameOffset: %p\n", id, pFNameEntry);

        char* pName = (char*)((uintptr_t)chunk + (uintptr_t)pFNameEntry);
        debug("[%x] chunk(%p) + pFNameEntry (%p) + 0x2 = %p\n", id, chunk, pFNameEntry, pName);
        debug("[%x] pName: %p Length: %u\n", id, pName);

        char tempName[NAME_LENGTH] = { 0 };

        if (!ReadProcessMemory(pSdk->mem->hProcess, pName, tempName, NAME_LENGTH, NULL)) {
            error_trace("[Get-Temp-Name]","Could not read memory!");
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


        // Getting rid of often ocurring false positives
        if (strncmp(tempName, "y", 1) == 0) {
            return false;
        }

        for (int i = 2; i < strlen(tempName); i++) {
            if (tempName[i] <= 0x2D) {
                tempName[i] = 0x00;
            }
        }

        if (strlen((char*)tempName + skip) == 1) {
            return false;
        }

        // Check for still invalid names
        if (
            ((tempName + skip)[0] > 0x7B || (tempName + skip)[0] < 0x2D) &&
            ((tempName + skip)[1] > 0x7B || (tempName + skip)[1] < 0x2D)
            ) {
            return false;
        }

        memcpy(out, (char*)tempName + skip, strlen(tempName));
        if (strlen(out) == 0) {
            error_trace("Get-Name","Not found: id:%u chunkId:%u pChunk:%p pFname: %p\n", id, chunkId, chunk, pName);
        }
        debug("Name: %s\n", out);
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

    /// <summary>
    /// Uses the FNamePool to get the ASCII name for UObjects
    /// </summary>
    /// <param name="uObject">pointer to UObject</param>
    /// <param name="name">output string for the name</param>
    /// <returns>FNamePool nameIndex value</returns>
    UINT64 UE_SDK::Remote_SDK::getFNameForUObject(uintptr_t uObject, char name[NAME_LENGTH]) {
        auto current = uObject;
        if (current < 0xFFFFFF || current > 0x7FF000000000) return 0;
        DEBUG_SDK&& printf("UObject at : %p\n", current);
        UINT64 nameIndex = RemoteMem().read<UINT64>(mem->hProcess, (UINT64*)((char*)current + 0x18));

        DEBUG_SDK&& printf("Found nameIndex: %p at: %p\n", nameIndex, (UINT64*)((char*)current + 0x18));

        pSdk->getFName(nameIndex, name);


        return nameIndex;
    }

    /// <summary>
    /// Returns a heap allocated array of UObject pointers found in the Unreal Engine's GObject Array
    /// </summary>
    /// <param name="amount">Amount of pointers to search</param>
    /// <returns>Heap allocated array of UObject pointers found in the Unreal Engine's GObject Array</returns>
    UObject** Remote_SDK::buildUObjectArray(size_t amount) {
        UObject** valids = (UObject**)calloc(amount, sizeof(UObject*));
        UObject** temp = (UObject**)calloc(amount, sizeof(UObject*));
        mem->readRemote<UObject*>((UObject**)pGObjectArray, temp, amount * sizeof(UObject*));

        UINT32 validCount = 0;
        for (int i = 0; i < amount; i++) {
            if (temp[i] == nullptr) {
                continue;
            }
            UObject* pUObject = temp[i];
            if (!mem->IsBadReadPtr(pUObject)) {
                valids[validCount++] = pUObject;
            }
        }
        free(temp);
        pUObjects = valids;
        pUObjectsSize = validCount;
        return valids;
    }


    /// <summary>
    /// Recursive function that gets the full name of a UObject by traversing up its inheritance chain printing the full inheritence.
    /// </summary>
    /// <param name="pUObject">Pointer to UObject</param>
    /// <param name="out">Pointer to the output string (FULL_NAME_LENGTH size)</param>
    /// <returns>Success boolean</returns>
    BOOL UE_SDK::Remote_SDK::getFullFName(UObject* pUObject, char* out) {
        char fullName[FULL_NAME_LENGTH] = { 0 };
        UINT16 namepointer = 0;
        char delimiter[] = "$";
        UObject* next = pUObject;

        do {
            UObject curr = UObject::from(next);
            if (curr.isEmpty()) {
                error_trace("GetFullName", "Curr was empty...");
                return false;
            }
            int nameLen = strlen(curr.asciiName);
            debug("Name: %s, len: %u\n", curr.asciiName, nameLen);
            strncpy_s(fullName + namepointer, FULL_NAME_LENGTH-namepointer, curr.asciiName, nameLen);
            debug("Fullname: %s, len: %u\n", fullName, strlen(fullName));

            namepointer += nameLen;

            memcpy(fullName + namepointer, delimiter, 1);
            namepointer += 1;
            next = (UObject*)curr.pClassOuter;
        } while (next != nullptr);
        strncpy_s(out, FULL_NAME_LENGTH, fullName, _TRUNCATE);
        return true;
    }

    /// <summary>
    /// Receives a ptr to a UObject. Stores this UObject locally and iterates over its content searching for valid pointers.
    /// This function prints information about the found pointers.
    /// It also works recursively.
    /// </summary>
    /// <param name="_pUObject">Target pointer</param>
    /// <param name="size">Search size</param>
    /// <param name="level">Recursion level</param>
    /// <param name="maxLevel">Max recursion level</param>
    /// <param name="onlyProperties">Whether to show only the found properties. This argument is deprecated
    /// as is searches in the body of the UObject instance and not in the class definition like getPropertiesForUObject</param>
    /// <returns>Success boolean</returns>
    bool UE_SDK::traverseUObjectForMembersEtc(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel, bool onlyProperties) {
        if (level == maxLevel) return true;
        UObject* pUOBject = (UObject*)malloc(size);
        memset(pUOBject, 0, size);

        debug("Reading _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);


        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)_pUObject, (LPVOID)pUOBject, size, NULL)) {
            error_trace("traverseUObjectForMembersEtc","Could not read memory (0x%p)\n", _pUObject);
            return false;
        }
        debug("Saved _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);
        debug("Getting name for UOBject %p. Name index: %x\n", _pUObject, pUOBject->name);

        // Assert that UObject points to a valid UObject
        char UObjectName[NAME_LENGTH] = { 0 };
        if (!pSdk->getFName(pUOBject->name, UObjectName)) {
            error_trace("traverseUObjectForMembersEtc", "Could not get name for uobject (0x%p)!\n", _pUObject);
            return false;
        }

        debug("Name for UOBject %p: %s\n", _pUObject, UObjectName);


        if (pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassPrivate)) {
            error_trace("traverseUObjectForMembersEtc", "[!] pClassPrivate (%p) ....\n", pUOBject->pClassPrivate);
            return false;
        }

        info("[%s]\n", UObjectName);


        for (int i = 0x20; i < size; i = i + 8) {

            uintptr_t ptr = *reinterpret_cast<uintptr_t*>((uintptr_t)(char*)pUOBject + i);

            debug("Child QWORD (%p) (0x%x)\n", ptr, i);


            if (pSdk->mem->IsBadReadPtr((LPVOID)ptr)) {
                error_trace("traverseUObjectForMembersEtc", "Child QWORD (%p) (0x%x) is not a valid pointer ....\n", ptr, i);
                continue;
            };


            if (ptr > 0x7FF000000000) {
                // Function
                if (onlyProperties) continue;

                for (int i = 0; i < level; i++) {
                    printf("-");
                }
                printf("- [%p] [0x%x] Function\n", ptr, i);
                continue;

            }
            if (UProperty::isUProperty(ptr)) {
                UProperty* next = (UProperty*)ptr;
                UProperty* uProperty = (UProperty*)malloc(sizeof(UProperty));
                do {
                    debug("Next UProperty: %p\n", next);

                    memset(uProperty, 0, sizeof(uProperty));

                    if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)next, uProperty, sizeof(UProperty), NULL)) {
                        error_trace("isUProperty", "Could not save UProperty(% p) to % p.Error: % u\n", next, uProperty, GetLastError());
                        free(uProperty);
                        next = nullptr;
                        continue;
                    }

                    uProperty->findName();


                    if (next && next != (UProperty*)ptr) {
                        printf("---");
                    }
                    printf("- [%p] [0x%x] [UProperty] %s (class-offset 0x%x)\n", next, i, uProperty->asciiName, uProperty->Internal_offset);
                    if (next == uProperty->pNext) {
                        break;
                    }

                    next = uProperty->pNext;
                } while (next != nullptr);

                
            }
            if (UObject::isUObject(ptr)) {
                // Object/Class
                UObject ChildUOBject = UObject::from((UObject*)ptr);


                if (pSdk->mem->IsBadReadPtr((LPVOID)ChildUOBject.pClassPrivate)) {
                    error_trace("traverseUObjectForMembersEtc","pUOBject pClassPrivate (%p) is a bad pointer\n", ChildUOBject.pClassPrivate);
                    continue;
                }

                debug("UOBject (%p) has Name: %s\n", ptr, ChildUOBject.asciiName);

                char fullname[FULL_NAME_LENGTH] = { 0 };
                pSdk->getFullFName((UObject*)ptr, fullname);

                for (int i = 0; i < level; i++) {
                    printf("-");
                }

                if (!onlyProperties) {
                    printf("- [%p] [0x%x] [UObject] %s ~ %s\n", ptr, i, ChildUOBject.asciiName, fullname);
                }



                uintptr_t child = { 0 };

                //if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)((char*)_pUObject + i), &child, sizeof(uintptr_t), NULL)) {
                //    ERROR_TRACE&& printf("Could not read child UOBject %p\n", (char*)_pUObject + i);
                //    continue;
                //}
                traverseUObjectForMembersEtc(ptr, 0x1111, level + 1, maxLevel, onlyProperties);
            }

        }

        free(pUOBject);
    }

    /// <summary>
    /// This functions returns a pointer to a heap allocated array filled with locally stored/copied valid UProperties.
    /// It iterates over the copied data from the target UObject pointer and searches for valid UProperty pointers.
    /// </summary>
    /// <param name="_pUObject">Target pointer</param>
    /// <param name="size">Search size</param>
    /// <returns>Pointer to heap allocated array of UProperties</returns>
    UProperty* getPropertiesForUObject(uintptr_t _pUObject, size_t size) {
        UProperty* properties = (UProperty*)calloc(MAX_PROPERTIES, sizeof(UProperty));
        memset(properties, 0, MAX_PROPERTIES * sizeof(UProperty));
        int propertycount = 0;

        UObject* pUOBject = (UObject*)malloc(size);
        memset(pUOBject, 0, size);

        debug("Reading _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);


        if (!ReadProcessMemory(pSdk->mem->hProcess, (LPCVOID)_pUObject, (LPVOID)pUOBject, size, NULL)) {
            error_trace("printFullInfoAboutUObject", "Could not read memory (0x%p)\n", _pUObject);
            return nullptr;
        }
        debug("Saved _pUOBject %p into pUOBject %p (heap)\n", _pUObject, pUOBject);
        debug("Getting name for UOBject %p. Name index: %x\n", _pUObject, pUOBject->name);

        // Assert that UObject points to a valid UObject
        char UObjectName[NAME_LENGTH] = { 0 };
        if (!pSdk->getFName(pUOBject->name, UObjectName)) {
            error_trace("printFullInfoAboutUObject", "Could not get name for uobject (0x%p)!\n", _pUObject);
            return nullptr;
        }

        debug("Name for UOBject %p: %s\n", _pUObject, UObjectName);


        if (pSdk->mem->IsBadReadPtr((LPVOID)pUOBject->pClassPrivate)) {
            error("[!] pClassPrivate (%p) or pClassOuter (%p) is not a valid pointer ....\n", pUOBject->pClassPrivate);
            return nullptr;
        }

        for (int i = 0x20; i < size; i = i + 8) {

            uintptr_t ptr = *(uintptr_t*)((char*)pUOBject + i);


            if (pSdk->mem->IsBadReadPtr((LPVOID)ptr)) {
                error_trace("printFullInfoAboutUObject", "Child QWORD (%p) (0x%x) is not a valid pointer ....\n", ptr, i);
                continue;
            };

            if (UProperty::isUProperty(ptr)) {
                UProperty* next = (UProperty*)ptr;

                UProperty* uProperty = (UProperty*)malloc(sizeof(UProperty));
                do {

                    memset(uProperty, 0, sizeof(UProperty));

                    if (!ReadProcessMemory(pSdk->mem->hProcess, (LPVOID)next, uProperty, sizeof(UProperty), NULL)) {
                        error_trace("isUProperty", "Could not save UProperty(% p) to % p.Error: % u\n", next, uProperty, GetLastError());
                        next = nullptr;
                        continue;
                    }

                    uProperty->findName();

                    debug("Saving uproperty (%s) to %p\n", uProperty->asciiName, properties + propertycount);
                    if (propertycount > MAX_PROPERTIES) {
                        error("Properties exhausted. Property count: %u. Max properties: %u\n", propertycount, MAX_PROPERTIES);
                        break;
                    }
                    memcpy(properties + propertycount, uProperty, sizeof(UProperty));
                    propertycount++;
                    
                    if (next == uProperty->pNext) {
                        break;
                    }
                        
                    next = uProperty->pNext;
                } while (next != nullptr);
                free(uProperty);


            }
        }
        free(pUOBject);
        return properties;
    }



    /// <summary>
    /// Receives a pointer and size, locally stores the remote UObject to iterate over its 8bit alligned pointers (e.g. 0x00, 0x08 ...).
    /// It iterates over all pointers and saves pointers pointing to valid UProperties. (returned by getPropertiesForUObject)
    /// It also iterates over all pointers of the class definition (UClass) of this UObject.
    /// </summary>
    /// <param name="_pUObject">Target pointer</param>
    /// <param name="size">Search size for this target</param>
    /// <param name="level">current recursion level</param>
    /// <param name="maxLevel">max recursion level</param>
    /// <param name="prefix">recursion prefix for logging</param>
    /// <returns>boolean</returns>
    bool UE_SDK::getAllPropertiesForUObject(uintptr_t _pUObject, size_t size, UINT8 level, UINT8 maxLevel, char* prefix) {
        if (level == maxLevel) return true;
        char localPrefix[PROP_PREFIX_LENGTH] = { 0 };
        int prefixLength = strlen(prefix);
        if (level != 0) {
            strncpy_s(localPrefix, PROP_PREFIX_LENGTH, prefix, PROP_PREFIX_LENGTH);
        }
        UObject baseObject = UObject::from((UObject*)_pUObject);
        if (baseObject.isEmpty()) {
            return true;
        }
        info("[%s] base class (%p)\n", baseObject.asciiName, _pUObject);
        UProperty* properties = getPropertiesForUObject(_pUObject, size);
        if (properties == nullptr) {
            return false;
        }
        UObject classPrivate = UObject::from((UObject*)baseObject.pClassPrivate);

        info("[%s] class private (%p)\n", classPrivate.asciiName, baseObject.pClassPrivate);
        UProperty* propertiesOfInnerClass = getPropertiesForUObject(baseObject.pClassPrivate, size);
        if (propertiesOfInnerClass == nullptr) {
            return true;
        }
        info("Found %u Properties in the base object %p\n", getSizeOfArray(properties), _pUObject);
        info("Found %u Properties in the classPrivate/innerClass object %p\n", getSizeOfArray(propertiesOfInnerClass), baseObject.pClassPrivate);
        // todo merge class and classprivate properties together
        for (int i = 0; i < MAX_PROPERTIES; i++) {
            UProperty prop = propertiesOfInnerClass[i];
            if (prop.isEmpty()) {
                continue;
            }
            uintptr_t pPropCarrier = pSdk->mem->readRemote((uintptr_t*)((char*)_pUObject + prop.Internal_offset));

            UObject propCarrier = UObject::from((UObject*)pPropCarrier);

            char newPrefix2[PROP_PREFIX_LENGTH] = { 0 };
            sprintf_s(newPrefix2, "%s / c:%s p:%s o:0x%x", localPrefix, classPrivate.asciiName, prop.asciiName, prop.Internal_offset);
            info("[%s] %s (0x%x) (0x%p)\n", newPrefix2, prop.asciiName, prop.Internal_offset, _pUObject + prop.Internal_offset);

            if (propCarrier.isEmpty()) {
                continue;
            }

            getAllPropertiesForUObject(pPropCarrier, size, level + 1, maxLevel, newPrefix2);

        }
        free(properties);
        free(propertiesOfInnerClass);
        return true;
    }


    /// <summary>
    /// Prints the FName for all UObjects of the given array.
    /// </summary>
    /// <param name="uObjects">Array containing UObject pointers</param>
    /// <param name="size">Size of the array</param>
    /// <returns>Success boolean</returns>
    bool UE_SDK::printFNameForUObjects(uintptr_t* uObjects, size_t size) {

        info("[index] [nameIndex] [pUObject] name\n");
        for (int i = 0; i < size; i++) {
            auto current = uObjects[i];

            // Get vtablePointer to validate if UObject is a class
            uintptr_t vtablePointer = RemoteMem().read<uintptr_t>(pSdk->mem->hProcess, (uintptr_t*)current);
            if (vtablePointer < 0x00007FF000000000) continue;
            char name[NAME_LENGTH] = { 0 };

            UINT64 nameIndex = pSdk->getFNameForUObject(current, name);
            if (!nameIndex) continue;
            if (strlen(name) > 0) {
                info("[%d] [%p] [%p] %s (%u)\n", i, nameIndex, current, name, strlen(name));
            }
        }
        return true;
    }

}