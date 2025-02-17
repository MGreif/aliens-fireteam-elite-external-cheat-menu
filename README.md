# Aliens: Fireteam Elite external cheat menu

Architecture: x86_64

OS: Windows 

### Disclaimer

This project is for educational purposes. I am not responsible for any bans/consequences you might get for using this.
AFE is an amazing game and please use this menu with caution. Do not annoy or disturb other players using this.
This project is just a private project to learn more about CPP, Gamehacking, Unreal-Engine and memory management in general!
This code is not clean and mostly used to simply get to the finish in a quick manner.

### Info

I have not tested the offsets in an online lobby. I only tried them in a self-hosted bot lobby.
This project is also for the sole purpose of me learning CPP, WinAPI and GameHacking in general.
This means, that the CPP code is not perfect and i will try to improve it.

#### Online with other players

If you apply no-spread, it will also affect you in an online session with other players.
The ammo seems to be broken a bit. You can set it to 9999 but after the actual ammo is empty, you will not do any damage to others. You must not reload tho.


## Features

Features labled with [ingame] should only be activated in an active game. If not, it will not crash, but simply not work.

- Set all consumables to 999
- Set ammo to 9999 (both guns) [ingame]
- Set ingame consumables (top and right) to 99 [ingame]
- No spread [ingame]
- No recoil [ingame]
- Infinite stamina (quite amateur way right now, ill change it in the future) [ingame]
- Godmode (infinite health) [ingame]
- Superspeed [ingame]
- Consistently setting enemy healt to 0 thus enabling instakill.
I am working on more.


# SDK

To learn more about Unreal Engine (rather than just scanning and reversing the game) i decided to write a small SDK for UE v4.25.
This will also enable me to write better checks/hacks for the game by seeing the reversed implementation details and direct offsets.

The SDK:
- scans for the GObjects and GNames(FNamePool in > v4)
- Print all GOBjects
- Recursively prints Details about GOBjects (Member classes and Properties with offsets)
- Recursively prints Details about GOBjects and properties (based on UClass definitions)

## Usage

```
hack.exe sdk print-names                                      # Prints all GObjects names
hack.exe sdk object-info <address>  <max-levels>              # Prints info about a specified UOBject
hack.exe sdk find-names  <name>                               # Find UOBjects that start with a specific string
hack.exe sdk get-properties  <uobject-address> <max-levels>   # Find UOBjects properties
```

## TODO

- Better FName lookup (feels only 80% reliable because i currently dont use the FNameEntry length (cant find it lol))
- Use Size of GObjects when looking it up/printing details (Currently the user needs to specify a size)


