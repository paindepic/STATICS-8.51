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
#include "minhook/minhook.h"

// UE4 SDK
#include "SDK/UE4.h"
#include "SDK/Fortnite.h"

// Project Reboot 8.51
#include "Globals.h"
#include "MissingFunctions.h"
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
#include "ue.h"

// Logging
#define Log(fmt, ...) printf("[Project Reboot 8.51] " fmt "\n", ##__VA_ARGS__)
#define LOG_(fmt, ...) Log(fmt, ##__VA_ARGS__)

// Helper macros
#define IMAGE_BASE ((uintptr_t)GetModuleHandleW(0))
#define GET_OFFSET(addr) (IMAGE_BASE + addr)

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
    inline void VirtualHook(void* object, int vtableIndex, void* hookFunction, void** originalFunction = nullptr) {
        void** vtable = *(void***)object;
        if (originalFunction) *originalFunction = vtable[vtableIndex];
        DWORD oldProtect;
        VirtualProtect(&vtable[vtableIndex], sizeof(void*), PAGE_READWRITE, &oldProtect);
        vtable[vtableIndex] = hookFunction;
        VirtualProtect(&vtable[vtableIndex], sizeof(void*), oldProtect, &oldProtect);
    }

    inline uintptr_t GetOffsetBRUH(uintptr_t offset) {
        return IMAGE_BASE + offset;
    }
}

// Global variables
namespace Globals {
    inline bool bCreativeEnabled = false;
    inline bool bSTWEnabled = false;
    inline bool bDebugMode = false;
    inline bool bBotsEnabled = true;
    inline int MaxBots = 50;
    inline bool bTeamsEnabled = true;
    inline bool bFarmingEnabled = true;
    inline bool bBuildingEnabled = true;
    inline bool bVehiclesEnabled = true;
    inline bool bLootingEnabled = true;
    inline bool bAbilitiesEnabled = true;
    inline bool bBackendEnabled = false;
    inline std::string BackendURL = "http://localhost:3000";
}

// Initialize GObjects for UE4
inline void InitGObjects() {
    auto GObjectsAddress = GET_OFFSET(0x7A5B0A0); // Update this offset for 8.51
    auto GObjects = (FUObjectArray*)GObjectsAddress;
    if (!GObjects) {
        Log("Failed to initialize GObjects!");
        return;
    }
    Log("GObjects initialized successfully!");
}