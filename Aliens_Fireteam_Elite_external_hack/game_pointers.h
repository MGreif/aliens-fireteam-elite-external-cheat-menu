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
    (LPVOID)0x0524F208,
    (LPVOID)0x18,
    (LPVOID)0x110,
    (LPVOID)0xE0,
    (LPVOID)0x250,
    (LPVOID)0x120,
    (LPVOID)0x8,
    (LPVOID)0x7E8,
};



UINT8 secondarySpreadPointerchainSize = 8;



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

/// Its actually the same offset the the offsets for all the consumables in the HUB/menu
/// The only difference is the 4th offset being 108 and not 110.

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

/// Its actually the same offset the the offsets for all the consumables in the HUB/menu
/// The only difference is the 4th offset being 108 and not 110.

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


/// Stamina Patch ! DO NOT USE

char staminaReductionDampingPatchCode[] = {
    0x90,
    0x90,
    0x90,
    0x90,
    0x90,
};

char staminaReductionDampingOriginalCode[sizeof(staminaReductionDampingPatchCode)] = { 0 };

MemoryPatch staminaReductionDamping = {
    staminaReductionDampingOriginalCode,
    staminaReductionDampingPatchCode,
    5,
    0x900710 // 0x900710 (setPxJointLimitParametersDamping base) + 0x0
};


#endif // !GAME_POINTERS_AFE
