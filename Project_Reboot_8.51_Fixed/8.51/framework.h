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

// Forward declarations for hook initialization functions
void InitHoksPC();
void InitAbilities();
void InitFarming();
void HOKSREAL();
void InitPawnHooks();
void InitVehicleHooks();
void InitPCOffsets();

// Hook function declarations from dllmain.cpp
bool TickFlushHook(void* a1);
extern void* TickFlushOG;
void* DispatchReqHook(void* a1, void* a2);
extern void* DispatchReqOG;
void KickPlayer(void* a1);
void ChangeGameSessionId(void* a1);
void ValFailure1(void* a1);
void NoReserve(void* a1);
int UWorldGetNetMode(void* a1);
int AActorGetNetMode(void* a1);
void NoMcp(void* a1);
void PickTeamHook(void* a1);
void SetMegaStormStuffHOOK(void* a1);
extern void* SetMegaStormStuffidkREALOG;
void sub_7FF6B9B17A60(AFortWeapon* a1, unsigned int a2);
extern void* sub_7FF6B9B17A60_OG;
int GetMaxTickRate(void* a1);
void CollectGarbage(void* a1);
void CanActivateAbility(void* a1);
void K2_CommitExecuteHook(UFortGameplayAbility* a1);
extern void* K2_CommitExecute;

// Project headers - Ordered to minimize dependencies
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
#include "Inventory.h"

// Logging macros
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
        if (!object) return;
        void** vtable = *(void***)object;
        if (!vtable) return;
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

// Global variables namespace
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
    inline bool bEventEnabled = false;
    inline bool bLategame = false;
    inline std::string BackendURL = "http://localhost:3000";
    
    // GameMode variables
    inline int MaxPlayersPerTeam = 1;
    inline int NextTeamIndex = 2;
    inline int NextBotTeamIndex = 100;
    inline int CurrentPlayersOnTeam = 0;
}

// Initialize GObjects for UE4
inline void InitGObjects() {
    auto GObjectsAddress = GET_OFFSET(0x7A5B0A0); // Offset for 8.51
    auto GObjects = (FUObjectArray*)GObjectsAddress;
    if (!GObjects) {
        Log("Failed to initialize GObjects!");
        return;
    }
    Log("GObjects initialized successfully!");
}

// Helper template for SpawnActor
inline UWorld* GetWorld() {
    return UWorld::GetWorld();
}

inline AGameModeBase* GetGameMode() {
    auto World = GetWorld();
    return World ? World->AuthorityGameMode : nullptr;
}

inline AGameStateBase* GetGameState() {
    auto World = GetWorld();
    return World ? World->GameState : nullptr;
}

inline UEngine* GetEngine() {
    return UEngine::GetEngine();
}

template<typename T>
inline T* GetDefObj() {
    return (T*)T::StaticClass()->DefaultObject;
}
