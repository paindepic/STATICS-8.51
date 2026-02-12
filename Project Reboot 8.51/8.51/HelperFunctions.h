#pragma once

// Helper functions used across the codebase

// Get current GameMode
inline AFortGameModeAthena* GetGameMode()
{
    return (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
}

// Get current GameState
inline AFortGameStateAthena* GetGameState()
{
    return (AFortGameStateAthena*)UWorld::GetWorld()->GameState;
}

// Get current World
inline UWorld* GetWorld()
{
    return UWorld::GetWorld();
}

// Get GameplayStatics singleton
inline UGameplayStatics* GetStatics()
{
    return UGameplayStatics::GetDefaultObj();
}

// Get Engine instance
inline UEngine* GetEngine()
{
    return UEngine::GetEngine();
}

// Get FortKismetLibrary singleton
inline UFortKismetLibrary* GetFortKismet()
{
    return UFortKismetLibrary::GetDefaultObj();
}

// Get default object for a class
template<typename T>
inline T* GetDefObj()
{
    return T::GetDefaultObj();
}

// Get offset from image base (for hooks)
inline uintptr_t GetOffsetBRUH(uintptr_t offset)
{
    extern uintptr_t ImageBase;
    return ImageBase + offset;
}

// Spawn an actor of a given type
template<typename T>
inline T* SpawnActor(FTransform Transform = FTransform())
{
    return (T*)UGameplayStatics::BeginSpawningActorFromClass(GetWorld(), T::StaticClass(), Transform, false, nullptr);
}

// CreateNetDriver function (0x13CB180 for 8.51)
static UNetDriver* (*CreateNetDriver)(UEngine*, UWorld*, FName) = decltype(CreateNetDriver)(GetOffsetBRUH(0x13CB180));

// InitListen function (0x7EDB70 for 8.51)
static bool (*InitListen)(UNetDriver*, UWorld*, FURL&, bool, FString&) = decltype(InitListen)(GetOffsetBRUH(0x7EDB70));

// SetWorld function (0x7EC710 for 8.51)
static void (*SetWorld)(UNetDriver*, UWorld*) = decltype(SetWorld)(GetOffsetBRUH(0x7EC710));

// Listen function - wraps the network setup for Volcano compatibility
static void Listen()
{
    Log("Starting network listener...");
    
    FName NetDriverDefinition = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
    
    UNetDriver* Driver = CreateNetDriver(GetEngine(), GetWorld(), NetDriverDefinition);
    if (!Driver)
    {
        Log("Failed to create NetDriver!");
        return;
    }
    
    Driver->World = GetWorld();
    Driver->NetDriverName = NetDriverDefinition;
    
    FURL NewURL;
    NewURL.Port = 7777;
    
    FString Error;
    if (!InitListen(Driver, GetWorld(), NewURL, true, Error))
    {
        Log("InitListen failed: %s", Error.ToString().c_str());
        return;
    }
    
    GetWorld()->NetDriver = Driver;
    SetWorld(Driver, GetWorld());
    
    for (int i = 0; i < GetWorld()->LevelCollections.Num(); i++)
    {
        GetWorld()->LevelCollections[i].NetDriver = Driver;
    }
    
    Log("Network listener started successfully on port 7777!");
}

// VirtualHook function wrapper
inline void VirtualHook(void* object, int vtableIndex, void* hookFunction, void** originalFunction = nullptr)
{
    void** vtable = *(void***)object;
    if (originalFunction) *originalFunction = vtable[vtableIndex];
    DWORD oldProtect;
    VirtualProtect(&vtable[vtableIndex], sizeof(void*), PAGE_READWRITE, &oldProtect);
    vtable[vtableIndex] = hookFunction;
    VirtualProtect(&vtable[vtableIndex], sizeof(void*), oldProtect, &oldProtect);
}

// IsFalse helper - returns false for hooks that need to disable functionality
inline bool IsFalse()
{
    return false;
}

// Global variables referenced by gaymode.h
namespace Globals {
    inline FString PlaylistName = L"Playlist_DefaultDuo";
    inline bool bLategame = false;
}
