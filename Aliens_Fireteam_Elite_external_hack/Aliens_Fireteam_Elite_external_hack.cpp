// Aliens_Fireteam_Elite_external_hack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include "Memory.h"
#include <stdio.h>
#include "LoopThread.hpp"
#include "HackLogic.hpp"

#define PROCESS_NAME L"Endeavor-Win64-Shipping.exe"
#define WIN_ERROR 1
#define WIN_SUCCESS 0
extern MemoryPatchLoopUnitOfWork* unitsOfWork[];
extern UINT8 unitsOfWorkSize;
extern MemoryPatchLoopUnitOfWork infiniteStaminaUnitOfWork;
extern MemoryPatchLoopUnitOfWork noRecoilUnitOfWork;
extern MemoryPatchLoopUnitOfWork godmodeUnitOfWork;

int main(int argc, char** argv)
{

    DWORD pid = GetAlienFireteamElitePid();

    if (!pid) {
        std::cout << "Process not found ..." << std::endl;
        return WIN_ERROR;
    }
    else {
        std::cout << "Found process. ID: " << pid << std::endl;
    }

    std::cout << "Starting hack ...." << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (!hProcess) {
        std::cout << "Could not get process ..." << std::endl;
        return WIN_ERROR;
    }

    BOOL stop = false;
    uintptr_t baseAddress = GetModuleBaseAddress(pid, PROCESS_NAME);


    MemoryPatchLoopParam params = {
        hProcess,
        (LPVOID)baseAddress,
        unitsOfWork,
        unitsOfWorkSize
    };

    HANDLE hThread = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)startMemoryPatchLoop, (LPVOID)&params, 0, NULL);
    if (!hThread) {
        std::cout << "Could not start new thread ..." << std::endl;
    }
    else {
        std::cout << "Started new thread. Thread ID: " << GetThreadId(hThread) << std::endl;
    }

    while (!stop) {

        char buffer[5]{ 0 };

        printf("Options:\n");
        printf("[In an active game]\n");
        printf("1: Set current weapon ammo to 9999\n");
        printf("2: Remove weapon spread\n");
        printf("3: Toggle Infinite stamina %s\n", infiniteStaminaUnitOfWork.active ? "[active]" : "");
        printf("5: Set top and right consumable amount to 99\n");
        printf("6: Set max walk speed to 5000\n");
        printf("7: Set sprint speed to 5000\n");
        printf("8: Toggle weapon no-recoil %s\n", noRecoilUnitOfWork.active ? "[active]" : "");
        printf("9: Toggle godmode %s\n\n", godmodeUnitOfWork.active ? "[active]" : "");
        printf("[In the hub-world]\n");
        printf("4: Set all consumables to 999\n");
        printf("0: Exit\n\n\n");
        scanf_s("%s",&buffer,sizeof(buffer));
        fflush(stdin);
        system("cls");
        if (strncmp(buffer, "1",1) == 0) {
            if (!setPrimaryMagazine(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setPrimaryTotal(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setSecondaryMagazine(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            if (!setSecondaryTotal(hProcess, (LPVOID)baseAddress)) {
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }

        }
        else if (strncmp(buffer, "2", 1) == 0) {
            if (!removeWeaponSpray(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not remove weapon spread" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Removed weapon spread :)" << std::endl;
            }
        }
        else if (strncmp(buffer, "3", 1) == 0) {
            infiniteStaminaUnitOfWork.active = !infiniteStaminaUnitOfWork.active;
        }
        else if (strncmp(buffer, "4", 1) == 0) {
            if (!setConsumablesTo999(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set consumables" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set all consumables to 999 :)" << std::endl;
            }
        }
        else if (strncmp(buffer, "5", 1) == 0) {
            if (!setIngameTopAndRightConsumableTo99(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set consumable" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set consumable to 99 :)" << std::endl;
            }
        }
        else if (strncmp(buffer, "6", 1) == 0) {
            if (!setWalkSpeedTo5000(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set walk speed" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set walk speed to 5000 :). This might get reset on walk speed changes in game" << std::endl;
            }
        }
        else if (strncmp(buffer, "7", 1) == 0) {
            if (!setSprintSpeedTo5000(hProcess, (LPVOID)baseAddress)) {
                std::cout << "Could not set sprint speed" << std::endl;
                std::cout << GetLastError() << std::endl;
                return WIN_ERROR;
            }
            else {
                std::cout << "Set sprint speed to 5000 :). This might get reset on walk speed changes in game" << std::endl;
            }
        }
        else if (strncmp(buffer, "8", 1) == 0) {
            noRecoilUnitOfWork.active = !noRecoilUnitOfWork.active;
        }
        else if (strncmp(buffer, "9", 1) == 0) {
            godmodeUnitOfWork.active = !godmodeUnitOfWork.active;
        }

        else if (strncmp(buffer, "0", 1) == 0) {
            break;
        }
        else {
            printf("Unknown...");
        }
        printf("\n\n");

    }

    std::cout << "Exiting hack :)" << std::endl;
}
