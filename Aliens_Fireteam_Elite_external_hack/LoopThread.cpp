// This file contains the logic for an memory adjustment interval
// It creates a thread that continuosly and independently loops through predefined 
// units of work that can either be activated or not
// They contain callback functions that do the work to manipulate the memory
#include "iostream"
#include "LoopThread.hpp"
#include "HackLogic.hpp"


// Unfortunately this is the current WIP implementation for no recoil....
// The recoil often gets adjusted by certain events (i.e. lancers passive or some abilities)
// I was to lazy to figure out the unreal-engine functions and implement conditional checks wether a "setNumericalAttribute" function was called with recoil params etc.
// So thats how it is now lol
// Also, with lancer its working quite reliably, but with others the secondary weapon offsets may be wrong. A fix is WIP rn.


BOOL noRecoilUnitOfWorkCallback(HANDLE hProcess, LPVOID baseAddress) {
    if (!setPrimaryNoRecoil(hProcess, baseAddress)) {
        std::cout << "Could not remove primary weapon recoil" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }


    if (!setSecondaryNoRecoil(hProcess, baseAddress)) {
        std::cout << "Could not remove secondary weapon recoil" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }

    return true;
};


BOOL infiniteStaminaUnitOfWorkCallback(HANDLE hProcess, LPVOID baseAddress) {
    if (!setFullStamina(hProcess, baseAddress)) {
        std::cout << "Could not set full stamina" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }
    return true;
};

BOOL godmodeCallback(HANDLE hProcess, LPVOID baseAddress) {
    if (!setHealthToMax(hProcess, baseAddress)) {
        std::cout << "Could not set health to max" << std::endl;
        std::cout << GetLastError() << std::endl;
        return false;
    }
    return true;
};


MemoryPatchLoopUnitOfWork noRecoilUnitOfWork = {
    (MemoryPatchCallback)noRecoilUnitOfWorkCallback,
    false,
};

MemoryPatchLoopUnitOfWork infiniteStaminaUnitOfWork = {
    (MemoryPatchCallback)infiniteStaminaUnitOfWorkCallback,
    false,
};

MemoryPatchLoopUnitOfWork godmodeUnitOfWork = {
    (MemoryPatchCallback)godmodeCallback,
    false,
};

MemoryPatchLoopUnitOfWork* unitsOfWork[] = {
    &noRecoilUnitOfWork,
    &infiniteStaminaUnitOfWork,
    &godmodeUnitOfWork
};

UINT8 unitsOfWorkSize = 3;

void startMemoryPatchLoop(LPVOID lpParam) {
    MemoryPatchLoopParam* params = reinterpret_cast<MemoryPatchLoopParam*>(lpParam);
    std::cout << "Hello from thread: " << GetCurrentThreadId() << std::endl;
    std::cout << "Starting memory patch loop" << std::endl;

    while (true) {
        for (int i = 0; i < params->unitOfWorksSize; i++) {
            MemoryPatchLoopUnitOfWork* currentUnit = params->unitOfWorks[i];
            if (currentUnit->active) {
                if (!currentUnit->callback(params->hProcess, params->baseAddress)) {
                    std::cout << "Could not perform unit of work ... deactivating" << std::endl;
                    currentUnit->active = false;
                }
            }
        }
        Sleep(100);
    }
};



