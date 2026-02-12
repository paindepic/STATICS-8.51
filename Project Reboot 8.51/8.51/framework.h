#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <format>
#include <fstream>
#include <sstream>
#include <random>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cwchar>
#include <cctype>
#include <cstdio>
#include <cstdarg>
#include <cassert>

// MinHook
#include "minhook/MinHook.h"

// UE4 SDK - Fortnite 8.51 Generated SDK
#include "SDK/SDK.hpp"

// Use SDK namespace
using namespace SDK;

// Logging
#define Log(fmt, ...) printf("[Project Reboot 8.51] " fmt "\n", ##__VA_ARGS__)
#define LOG_(fmt, ...) Log(fmt, ##__VA_ARGS__)

// Helper macros
#define IMAGE_BASE ((uintptr_t)GetModuleHandleW(0))
#define GET_OFFSET(addr) (IMAGE_BASE + addr)

// External ImageBase declaration for use in other files
extern uintptr_t ImageBase;

// Memory utilities
namespace Memory {
    inline void WriteMemory(uintptr_t address, const void* data, size_t size) {
        DWORD oldProtect;
        VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy((void*)address, data, size);
        VirtualProtect((LPVOID)address, size, oldProtect, &oldProtect);
    }

    template<typename T>
    inline T ReadMemory(uintptr_t address) {
        T value;
        memcpy(&value, (void*)address, sizeof(T));
        return value;
    }

    inline void NopMemory(uintptr_t address, size_t size) {
        DWORD oldProtect;
        VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
        memset((void*)address, 0x90, size);
        VirtualProtect((LPVOID)address, size, oldProtect, &oldProtect);
    }
}

// Hooking utilities
namespace Hooking {
    inline uintptr_t GetOffsetBRUH(uintptr_t offset) {
        return ImageBase + offset;
    }
}

// Initialize GObjects for UE4
// Note: InitGObjects() is already defined in SDK/Basic.hpp
// We just need to call it from dllmain.cpp

// Project Reboot 8.51 - Core headers
#include "Globals.h"
#include "HelperFunctions.h"
#include "MissingFunctions.h"
#include "HookImplementations.h"
#include "ue.h"

// Project Reboot 8.51 - Feature modules
#include "Misc.h"
#include "PE.h"
#include "Net.h"
#include "NetDriver.h"
#include "GameMode.h"
#include "Controller.h"
#include "AbilitySystemComponent.h"
#include "Pawn.h"
#include "FortWeapon.h"
#include "FortPickup.h"
#include "BuildingActor.h"
#include "BuildingContainer.h"
#include "FortInventory.h"
#include "Inventory.h"
#include "Looting.h"
#include "QuestManager.h"
#include "AIController.h"
#include "ServerBotManager.h"
#include "PlayerBots.h"
#include "Bots.h"
#include "BotNames.h"
#include "Vehicles.h"
#include "Farming.h"
#include "Teams.h"
#include "Abilities.h"
#include "PC.h"
#include "backend.h"
#include "gaymode.h"
