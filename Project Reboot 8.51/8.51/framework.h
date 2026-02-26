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

// Offsets namespace for PE.h and other files
namespace Offsets {
    inline uintptr_t ProcessEvent = 0x01E7A0F0;
    inline uintptr_t GObjects = 0x05A47438;
}

// Initialize GObjects for UE4
inline void InitGObjects() {
    auto GObjectsAddress = GET_OFFSET(0x7A5B0A0); // Update this offset for 8.51
    SDK::TUObjectArray* GObjs = (SDK::TUObjectArray*)GObjectsAddress;
    if (!GObjs) {
        Log("Failed to initialize GObjects!");
        return;
    }
    Log("GObjects initialized successfully!");
}

// Global helper functions
namespace SDK {
    inline TUObjectArray* GetGObjects() {
        return (TUObjectArray*)(GET_OFFSET(0x7A5B0A0));
    }

    inline UEngine* GetEngine() {
        return UEngine::GetEngine();
    }

    inline UWorld* GetWorld() {
        return UWorld::GetWorld();
    }
}

inline SDK::AFortGameModeAthena* GetGameMode() {
    if (!SDK::UWorld::GetWorld()) return nullptr;
    return (SDK::AFortGameModeAthena*)SDK::UWorld::GetWorld()->AuthorityGameMode;
}

inline SDK::AFortGameStateAthena* GetGameState() {
    if (!SDK::UWorld::GetWorld()) return nullptr;
    return (SDK::AFortGameStateAthena*)SDK::UWorld::GetWorld()->GameState;
}

inline SDK::UWorld* GetWorld() {
    return SDK::UWorld::GetWorld();
}

inline SDK::UGameplayStatics* GetStatics() {
    return SDK::UGameplayStatics::GetDefaultObj();
}

inline SDK::UKismetSystemLibrary* GetString() {
    return SDK::UKismetSystemLibrary::GetDefaultObj();
}

inline SDK::UKismetMathLibrary* GetMath() {
    return SDK::UKismetMathLibrary::GetDefaultObj();
}

inline SDK::UFortKismetLibrary* GetFortKismet() {
    return SDK::UFortKismetLibrary::GetDefaultObj();
}

// Template for StaticFindObject
template<typename T>
inline T* StaticFindObject(const std::string& Path) {
    return (T*)SDK::UObject::FindObject<SDK::UObject>(Path);
}

template<typename T>
inline T* StaticFindObject(const wchar_t* Path) {
    return (T*)SDK::UObject::FindObject<SDK::UObject>(Path);
}

// Template for StaticLoadObject
template<typename T>
inline T* StaticLoadObject(const std::string& Path) {
    return (T*)SDK::UObject::FindObject<SDK::UObject>(Path);
}

template<typename T>
inline T* StaticLoadObject(const wchar_t* Path) {
    return (T*)SDK::UObject::FindObject<SDK::UObject>(Path);
}

// Template for GetDefaultObj
template<typename T>
inline T* GetDefObj() {
    return T::GetDefaultObj();
}

// Cast helper
template<typename T, typename U>
inline T* Cast(U* Object) {
    if (!Object) return nullptr;
    if (Object->IsA(T::StaticClass()))
        return (T*)Object;
    return nullptr;
}

// HookVTable helper
template<typename T>
inline void HookVTable(int Index, void* HookFunc, void** OriginalFunc = nullptr) {
    void** VTable = *(void***)T::StaticClass()->DefaultObject;
    if (OriginalFunc) *OriginalFunc = VTable[Index];
    DWORD OldProtect;
    VirtualProtect(&VTable[Index], sizeof(void*), PAGE_READWRITE, &OldProtect);
    VTable[Index] = HookFunc;
    VirtualProtect(&VTable[Index], sizeof(void*), OldProtect, &OldProtect);
}

// SpawnActor helper
template<typename T = SDK::AActor>
inline T* SpawnActor(SDK::FVector Location = { 0, 0, 0 }, SDK::FRotator Rotation = { 0, 0, 0 }, SDK::AActor* Owner = nullptr, SDK::UClass* Class = nullptr) {
    if (!Class) Class = T::StaticClass();
    
    SDK::FTransform Transform{};
    Transform.Translation = Location;
    Transform.Rotation = SDK::FQuat(Rotation);
    Transform.Scale3D = SDK::FVector{ 1, 1, 1 };
    
    SDK::FActorSpawnParameters SpawnParams{};
    SpawnParams.Owner = Owner;
    SpawnParams.SpawnCollisionHandlingOverride = SDK::ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    return (T*)GetWorld()->SpawnActor(Class, &Transform, SpawnParams);
}

// SpawnPickup helper
inline SDK::AFortPickup* SpawnPickup(SDK::UFortItemDefinition* ItemDef, int Count, int LoadedAmmo, SDK::FVector Location, 
    SDK::EFortPickupSourceTypeFlag SourceType, SDK::EFortPickupSpawnSource SpawnSource, SDK::AFortPawn* PawnWhoDropped = nullptr) {
    SDK::AFortPickup* Pickup = SpawnActor<SDK::AFortPickup>(Location);
    if (!Pickup) return nullptr;
    
    Pickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;
    Pickup->PrimaryPickupItemEntry.Count = Count;
    Pickup->PrimaryPickupItemEntry.LoadedAmmo = LoadedAmmo;
    Pickup->OnRep_PrimaryPickupItemEntry();
    Pickup->PawnWhoDroppedPickup = PawnWhoDropped;
    
    return Pickup;
}

inline SDK::AFortPickup* SpawnPickup(SDK::FFortItemEntry& ItemEntry, SDK::FVector Location,
    SDK::EFortPickupSourceTypeFlag SourceType, SDK::EFortPickupSpawnSource SpawnSource) {
    return SpawnPickup(ItemEntry.ItemDefinition, ItemEntry.Count, ItemEntry.LoadedAmmo, Location, SourceType, SpawnSource, nullptr);
}

// IsFalse helper
inline bool IsFalse(void*) { return false; }

// GetOffsetBRUH helper - consistent naming
inline uintptr_t GetOffsetBRUH(uintptr_t Offset) {
    return IMAGE_BASE + Offset;
}

// Ability system helpers - these are defined in Abilities.h
extern __int64 (*Fgameplauyabilirtyspecctor)(void*, void*, char, int, void*);
extern __int64* (*GiveAbility)(void*, void*, SDK::FGameplayAbilitySpec);
extern char (*InternalTryActivateAbility)(SDK::UAbilitySystemComponent*, SDK::FGameplayAbilitySpecHandle, SDK::FPredictionKey, SDK::UGameplayAbility**, void*, SDK::FGameplayEventData*);

// Aliases for compatibility
#define AbilitySpecConstructor Fgameplauyabilirtyspecctor
#define GiveAbilityOG GiveAbility

// Building helpers
inline SDK::ABuildingSMActor* (*ReplaceBuildingActor)(SDK::ABuildingSMActor*, char, SDK::UClass*, int, uint8, bool, SDK::AController*) = nullptr;
inline bool (*CantBuild)(SDK::UWorld*, SDK::UClass*, SDK::FVector, SDK::FRotator, char, void*, char*) = nullptr;

// Bot globals
inline SDK::AFortAthenaMutator_Bots* BotMutator = nullptr;
inline std::vector<SDK::UAthenaCharacterItemDefinition*> Characters;
inline std::vector<SDK::UAthenaBackpackItemDefinition*> Backpacks;
inline std::vector<SDK::UAthenaPickaxeItemDefinition*> Pickaxes;
inline std::vector<SDK::UAthenaDanceItemDefinition*> Dances;
inline std::vector<SDK::UAthenaGliderItemDefinition*> Gliders;
inline std::vector<SDK::UAthenaContrailItemDefinition*> Contrails;

// Actor arrays
inline SDK::TArray<SDK::AActor*> PlayerStarts;
inline SDK::TArray<SDK::AActor*> BuildingFoundations;

// Building item definitions
inline SDK::UFortBuildingItemDefinition* Floor = nullptr;
inline SDK::UFortBuildingItemDefinition* Roof = nullptr;
inline SDK::UFortBuildingItemDefinition* Stair = nullptr;
inline SDK::UFortBuildingItemDefinition* Wall = nullptr;
inline SDK::UFortEditToolItemDefinition* EditTool = nullptr;

// Bot names
inline std::vector<std::string> BotDisplayNames;

// Apply character customization helper
inline void ApplyCharacterCustomization(SDK::AFortPlayerStateAthena* PlayerState, SDK::AFortPlayerPawnAthena* Pawn) {
    if (!PlayerState || !Pawn) return;
    // Implementation would go here
}

// HOKSREAL placeholder
inline void HOKSREAL() {
    Log("HOKSREAL called - placeholder implementation");
}