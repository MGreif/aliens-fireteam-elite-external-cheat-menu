#pragma once
#ifndef GAME_POINTERS_AFE
#define GAME_POINTERS_AFE

#include "Memory.h"


//////////////////////// Weapons


//// Primary


LPVOID primaryMagazinePointerchain[] = {
    (LPVOID)0x050C9BF8,
    (LPVOID)0x148,
    (LPVOID)0x0,
    (LPVOID)0xA8,
    (LPVOID)0xA8,
    (LPVOID)0x7DC,
};

/*
LPVOID primaryMagazinePointerchain[] = {
    (LPVOID)0x050CB010,
    (LPVOID)0x100,
    (LPVOID)0x20,
    (LPVOID)0x138,
    (LPVOID)0x60,
    (LPVOID)0x70,
    (LPVOID)0x3B0,
    (LPVOID)0x7DC,
};
*/
UINT8 primaryMagazinePointerchainSize = 6;


LPVOID primaryTotalAmmoPointerchain[] = {
    (LPVOID)0x050CC278,
    (LPVOID)0x28,
    (LPVOID)0x8,
    (LPVOID)0x170,
    (LPVOID)0xB8,
    (LPVOID)0x120,
    (LPVOID)0x0,
    (LPVOID)0x7D4
};

/*
LPVOID RifleTotalAmmoPointerchain[] = {
    (LPVOID)0x050CB010,
    (LPVOID)0x100,
    (LPVOID)0x20,
    (LPVOID)0x138,
    (LPVOID)0x60,
    (LPVOID)0x70,
    (LPVOID)0x3B0,
    (LPVOID)0x7D4
};
*/
UINT8 primaryTotalAmmoPointerchainSize = 8;


LPVOID primarySpreadPointerchain[] = {
    (LPVOID)0x050CC278,
    (LPVOID)0x28,
    (LPVOID)0x8,
    (LPVOID)0x170,
    (LPVOID)0xB8,
    (LPVOID)0x120,
    (LPVOID)0x0,
    (LPVOID)0x7E8,
};

UINT8 primarySpreadPointerchainSize = 8;


// NoRecoil is not working. But heavy machine gun recoil was 0.65f when +15% on lancer buff

LPVOID primaryADSRecoilPointerchain[] = {
    (LPVOID)0x050CAF48,
    (LPVOID)0x130,
    (LPVOID)0x370,
    (LPVOID)0x20,
    (LPVOID)0xA8,
    (LPVOID)0xA8,
    (LPVOID)0x5C8,
    (LPVOID)0x204,
};


UINT8 primaryADSRecoilPointerchainSize = 8;


LPVOID primaryNoADSRecoilPointerchain[] = {
    primaryADSRecoilPointerchain[0],
    primaryADSRecoilPointerchain[1],
    primaryADSRecoilPointerchain[2],
    primaryADSRecoilPointerchain[3],
    primaryADSRecoilPointerchain[4],
    primaryADSRecoilPointerchain[5],
    primaryADSRecoilPointerchain[6],
    (LPVOID)0x1EC,
};


UINT8 primaryNoADSRecoilPointerchainSize = 8;


//// Secondary

LPVOID secondaryMagazinePointerchain[] = {
    (LPVOID)0x050C9B80,
    (LPVOID)0x210,
    (LPVOID)0x868,
    (LPVOID)0xA8,
    (LPVOID)0x30,
    (LPVOID)0xA8,
    (LPVOID)0xB0,
    (LPVOID)0x7DC,
};

/*
LPVOID secondaryMagazinePointerchain[] = {
    (LPVOID)0x050CB370,
    (LPVOID)0xB0,
    (LPVOID)0x20,
    (LPVOID)0x138,
    (LPVOID)0x8,
    (LPVOID)0x3B0,
    (LPVOID)0x70,
    (LPVOID)0x7DC,
};
*/
UINT8 secondaryMagazinePointerchainSize = 8;


LPVOID secondaryTotalAmmoPointerchain[] = {
    (LPVOID)0x0524F208,
    (LPVOID)0x18,
    (LPVOID)0x110,
    (LPVOID)0xE0,
    (LPVOID)0x250,
    (LPVOID)0x120,
    (LPVOID)0x8,
    (LPVOID)0x7D4
};

/*
LPVOID secondaryTotalAmmoPointerchain[] = {
    (LPVOID)0x050CB370,
    (LPVOID)0xB0,
    (LPVOID)0x20,
    (LPVOID)0x138,
    (LPVOID)0x8,
    (LPVOID)0x3B0,
    (LPVOID)0x70,
    (LPVOID)0x7D4,
};
*/
UINT8 secondaryTotalAmmoPointerchainSize = 8;


LPVOID secondarySpreadPointerchain[] = {
    secondaryTotalAmmoPointerchain[0],
    secondaryTotalAmmoPointerchain[1],
    secondaryTotalAmmoPointerchain[2],
    secondaryTotalAmmoPointerchain[3],
    secondaryTotalAmmoPointerchain[4],
    secondaryTotalAmmoPointerchain[5],
    secondaryTotalAmmoPointerchain[6],
    (LPVOID)0x7E8,
};


UINT8 secondarySpreadPointerchainSize = 8;



LPVOID secondaryADSRecoilPointerchain[] = {
    (LPVOID)0x050CC240,
    (LPVOID)0x8,
    (LPVOID)0x20,
    (LPVOID)0x28,
    (LPVOID)0x8,
    (LPVOID)0x370,
    (LPVOID)0x5A0,
    (LPVOID)0x204,
};


UINT8 secondaryADSRecoilPointerchainSize = 8;

// 0.6451613307 Heavy machine rifle +15% lancer buff recoil value
// 22352606CB4 secondary total ammo
// 22352607004 ADS recoil
// 22352606FEC No ADS recoil

LPVOID secondaryNoADSRecoilPointerchain[] = {
    secondaryADSRecoilPointerchain[0],
    secondaryADSRecoilPointerchain[1],
    secondaryADSRecoilPointerchain[2],
    secondaryADSRecoilPointerchain[3],
    secondaryADSRecoilPointerchain[4],
    secondaryADSRecoilPointerchain[5],
    secondaryADSRecoilPointerchain[6],
    (LPVOID)0x1EC,
};


UINT8 secondaryNoADSRecoilPointerchainSize = 8;







LPVOID heldWeaponMagazinePointerchain[] = {
    (LPVOID)0x00000000050C9A90,
    (LPVOID)0x120,
    (LPVOID)0x260,
    (LPVOID)0x0,
    (LPVOID)0x2C0,
    (LPVOID)0x260,
    (LPVOID)0x240,
    (LPVOID)0x7DC,
};

UINT8 heldWeaponMagazinePointerchainSize = 8;

LPVOID heldWeaponTotalAmmoPointerchain[] = {
    (LPVOID)0x00000000050C9A90,
    (LPVOID)0x120,
    (LPVOID)0x260,
    (LPVOID)0x0,
    (LPVOID)0x2C0,
    (LPVOID)0x260,
    (LPVOID)0x240,
    (LPVOID)0x7D4
};

UINT8 heldWeaponTotalAmmoPointerchainSize = 8;

///////////////////////// Consumables
// All the consumables are in the same memory location.


LPVOID consumablePointerchainBase[] = {
    (LPVOID)0x052C5840,
    (LPVOID)0x30,
    (LPVOID)0x9F0,
    (LPVOID)0x110,
    (LPVOID)0x20,
    (LPVOID)0xD0,
    (LPVOID)0x60,
};


LPVOID incendiaryRoundsConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x218
};

LPVOID ingameIncendiaryRoundsConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x21C
};


LPVOID staticGridConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x258
};

LPVOID ingameStaticGridConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x25C
};

LPVOID mineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x358
};

LPVOID ingameMineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x35C
};

LPVOID droneConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x378
};


LPVOID ingameDroneConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x37C
};

LPVOID cryoGridConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x458
};

LPVOID ingameCryoGridConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x45C
};

LPVOID phosphorusMineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x598
};

LPVOID ingamePhosphorusMineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x59C
};

LPVOID electroshockRoundsConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x738
};

LPVOID ingameElectroshockRoundsConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x73C
};

LPVOID normalSentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x838
};

LPVOID ingameNormalSentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x83C
};

LPVOID electroshockSentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x9B8
};

LPVOID ingameElectroshockSentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0x9BC
};


LPVOID incendiarySentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0xF98
};

LPVOID ingameIncendiarySentryConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0xF9C
};

LPVOID electroshockMineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0xFB8
};

LPVOID ingameElectroshockMineConsumablePointerchain[] = {
    consumablePointerchainBase[0],
    consumablePointerchainBase[1],
    consumablePointerchainBase[2],
    consumablePointerchainBase[3],
    consumablePointerchainBase[4],
    consumablePointerchainBase[5],
    consumablePointerchainBase[6],
    (LPVOID)0xFBC
};

// The ingameXXX variants are using the address next to the actual amount the user owns.
// The address next to the amount is simple the amount the user has access to ingame
// But to actually use all of these. The consumable count needs to be adjusted.
// For this reason we also have to find and set the ingameConsumableTopAmount and right amount
// maybe also left and bottom. I already found Top and chaning the value is not visible in the UI
// But if the value is 0 and the UI shows 10, then we cannot place/use the consumable


UINT8 consumablePointerchainSize = 8;


////// Ingame consumables

UINT8 ingameConsumableIncendiarySentryDisplayPointerchainSize = 8;



LPVOID ingameConsumableTopAmountPointerchain[] = {
    (LPVOID)0x052C5840,
    (LPVOID)0x30,
    (LPVOID)0x2A0,
    (LPVOID)0x1A0,
    (LPVOID)0x30,
    (LPVOID)0x550,
    (LPVOID)0x38,
    (LPVOID)0xC40,
};

UINT8 ingameConsumableTopAmountPointerchainSize = 8;

LPVOID ingameConsumableRightAmountPointerchain[] = {
    (LPVOID)0x052C5840,
    (LPVOID)0x30,
    (LPVOID)0x250,
    (LPVOID)0x50,
    (LPVOID)0x6C8,
    (LPVOID)0x550,
    (LPVOID)0x40,
    (LPVOID)0xC40,
};


UINT8 ingameConsumableRightAmountPointerchainSize = 8;


///////////////////////// Mechanics


// Pointerchain to weapon spread
LPVOID weaponSpreadPointerchain[] = {
    (LPVOID)0x00000000050C9A90,
    (LPVOID)0x120,
    (LPVOID)0x260,
    (LPVOID)0x0,
    (LPVOID)0x2C0,
    (LPVOID)0x260,
    (LPVOID)0x240,
    (LPVOID)0x7E8,
};

UINT8 weaponSpreadPointerchainSize = 8;


char weaponSpreadPatchOnAimPatchCode[] = {
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
};

char weaponSpreadPatchOnAimOriginalCode[sizeof(weaponSpreadPatchOnAimPatchCode)] = { 0 };

MemoryPatch weaponSpreadPatchOnAim = {
    weaponSpreadPatchOnAimOriginalCode,
    weaponSpreadPatchOnAimPatchCode,
    8,
    0x14F3B72 // 0x14F3B60 (SetSpread base) + 0x12
};


char weaponSpreadPatchOnShootPatchCode[] = {
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
};

char weaponSpreadPatchOnShootOriginalCode[sizeof(weaponSpreadPatchOnShootPatchCode)] = { 0 };

MemoryPatch weaponSpreadPatchOnShoot = {
    weaponSpreadPatchOnShootOriginalCode,
    weaponSpreadPatchOnShootPatchCode,
    8,
    0x14E5230 // 0x14E5030 (PostFire base) + 0x200
};




//// Stamina float address


LPVOID HUBstaminaPointerchain[] = {
    (LPVOID)0x052F7A50,
    (LPVOID)0x8,
    (LPVOID)0x48,
    (LPVOID)0x20,
    (LPVOID)0x6C8,
    (LPVOID)0x140,
    (LPVOID)0x0,
    (LPVOID)0x6C,
};

UINT8 HUBstaminaPointerchainSize = 8;



LPVOID staminaPointerchain[] = {
    (LPVOID)0x050C99F0,
    (LPVOID)0x498,
    (LPVOID)0x28,
    (LPVOID)0x8,
    (LPVOID)0x420,
    (LPVOID)0x70,
    (LPVOID)0xA08,
    (LPVOID)0x6C,
};

UINT8 staminaPointerchainSize = 8;

LPVOID maxWalkSpeedPointerchain[] = {
    staminaPointerchain[0],
    staminaPointerchain[1],
    staminaPointerchain[2],
    staminaPointerchain[3],
    staminaPointerchain[4],
    staminaPointerchain[5],
    staminaPointerchain[6],
    (LPVOID)0x3C,
};

UINT8 maxWalkSpeedPointerchainSize = 8;

LPVOID sprintSpeedPointerchain[] = {
    staminaPointerchain[0],
    staminaPointerchain[1],
    staminaPointerchain[2],
    staminaPointerchain[3],
    staminaPointerchain[4],
    staminaPointerchain[5],
    staminaPointerchain[6],
    (LPVOID)0x54,
};

UINT8 sprintSpeedPointerchainSize = 8;

LPVOID ingameHealthPointerchain[] = {
    (LPVOID)0x052C5840,
    (LPVOID)0x30,
    (LPVOID)0x260,
    (LPVOID)0x190,
    (LPVOID)0x8,
    (LPVOID)0x140,
    (LPVOID)0x28,
    (LPVOID)0x38,
};

UINT8 ingameHealthPointerchainSize = 8;

LPVOID ingameHealthPointerchain2[] = {
    ingameHealthPointerchain[0],
    ingameHealthPointerchain[1],
    ingameHealthPointerchain[2],
    ingameHealthPointerchain[3],
    ingameHealthPointerchain[4],
    ingameHealthPointerchain[5],
    ingameHealthPointerchain[6],
    (LPVOID)0x3C,
};

UINT8 ingameHealthPointerchain2Size = 8;



// Entity list

LPVOID ingameEntityListPointerchain[]{
    (LPVOID)0x0524F1F8,
    (LPVOID)0x40,
    (LPVOID)0x240,
    (LPVOID)0xE0,
    (LPVOID)0xAC0,
};

UINT8 ingameEntityListPointerchainSize = 4;


// Unreal Engine 4.25


// USE THE DATA_COLLECTOR_DLL baseAddress for this!!!!
LPVOID GObjectsPointerchain[] = {
    (LPVOID)0xFC5A8
};

UINT8 GObjectsPointerchainSize = 1;



// Created with ReClass.NET 1.2 by KN4CK3R

class GamePointerChain1
{
public:
    char pad_0000[64]; //0x0000
    class GameInner1* pGameInner1; //0x0040
}; //Size: 0x0048

class GameInner1
{
public:
    char pad_0000[576]; //0x0000
    class GameInner2* pGameInner2; //0x0240
    char pad_0248[1472]; //0x0248
}; //Size: 0x0808

class GameInner2
{
public:
    char pad_0000[224]; //0x0000
    class GameInner3* pGameInner3; //0x00E0
    char pad_00E8[32]; //0x00E8
}; //Size: 0x0108

class EntityWrapper
{
public:
    char pad_0000[2576]; //0x0000
    class Entity* pEntity; //0x0A10
    char pad_0A18[496]; //0x0A18
}; //Size: 0x0C08

class N000003F6
{
public:
    char pad_0000[8]; //0x0000
}; //Size: 0x0008

class Entity
{
public:
    char pad_0008[8]; //0x0008
    class N00000634* N0000042A; //0x0010
    char pad_0018[8]; //0x0018
    class N0000067E* N0000042C; //0x0020
    char pad_0028[16]; //0x0028
    uint32_t N0000042F; //0x0038
    float fHealth; //0x003C
    char pad_0040[1096]; //0x0040

    virtual void Function0() {}
 //   virtual void Function1();
 //   virtual void Function2();
 //   virtual void Function3();
 //   virtual void Function4();
 //   virtual void Function5();
 //   virtual void Function6();
 //   virtual void Function7();
 //   virtual void Function8();
 //   virtual void Function9();
}; //Size: 0x0488

class GameInner3
{
public:
    char pad_0000[2752]; //0x0000
    class EntityWrapper* (*pEntityWrapperArray)[20]; //0x0AC0
    char pad_0AC8[320]; //0x0AC8
}; //Size: 0x0C08

class N00000669
{
public:
    char pad_0000[8]; //0x0000
}; //Size: 0x0008

class GamePointerChain2Stable
{
public:
    char pad_0000[120]; //0x0000
    class N000006A1* N0000067C; //0x0078
}; //Size: 0x0080

class N000006A1
{
public:
    char pad_0000[112]; //0x0000
    class N000006E7* N000006B0; //0x0070
    char pad_0078[144]; //0x0078
}; //Size: 0x0108

class N000006E7
{
public:
    char pad_0000[416]; //0x0000
    class N0000078D* N0000071C; //0x01A0
    char pad_01A8[608]; //0x01A8
}; //Size: 0x0408

class N0000078D
{
public:
    char pad_0000[16]; //0x0000
    class EntityWrapper* (*N00000790)[20]; //0x0010
}; //Size: 0x0018

class N000007FC
{
public:
    char pad_0000[8]; //0x0000
}; //Size: 0x0008

class N00000634
{
public:
    char pad_0000[72]; //0x0000
}; //Size: 0x0048

class N0000067E
{
public:
    char pad_0000[72]; //0x0000
}; //Size: 0x0048


class UObject {
public:
    uintptr_t vTable;
    UINT32 flags;
    UINT32 internalIndex;
    uintptr_t pClassPrivate;
    uintptr_t name;
    uintptr_t pClassOuter;
};



#endif // !GAME_POINTERS_AFE
