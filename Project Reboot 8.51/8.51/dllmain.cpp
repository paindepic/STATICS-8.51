// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

// Global variables
uintptr_t ImageBase = (uintptr_t)GetModuleHandleW(0);

// Function prototypes
void InitConsole();
void LoadWorld();
void Hook();
void WaitForLogin();
DWORD Main(LPVOID);

// Console initialization
void InitConsole() {
    AllocConsole();
    FILE* fptr;
    freopen_s(&fptr, "CONOUT$", "w+", stdout);
    SetConsoleTitleA("Project Reboot 8.51 | STATICS 8.51");
    Log("Project Reboot 8.51 - STATICS 8.51 Edition");
    Log("Combining Volcano 8.51 with Spectra 10.40 AI improvements");
}

// Load the Athena_Terrain world
void LoadWorld() {
    Log("Loading World!");
    if (!Globals::bCreativeEnabled && !Globals::bSTWEnabled) {
        UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"open Athena_Terrain", nullptr);
    }
    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0);
}

// Hook all the modules
void Hook() {
    // Initialize MinHook
    MH_Initialize();
    Log("MinHook Initialized!");

    // Hook all modules from both projects
    PE::HookAll();
    Misc::HookAll();
    GameMode::HookAll();
    Controller::HookAll();
    AbilitySystemComponent::HookAll();
    Pawn::HookAll();
    FortWeapon::HookAll();
    FortPickup::HookAll();
    BuildingActor::HookAll();
    BuildingContainer::HookAll();
    Net::HookAll();
    NetDriver::HookAll();
    AIController::HookAll();
    ServerBotManager::HookAll();
    QuestManager::HookAll();

    // Volcano-specific hooks
    InitHoksPC();
    InitAbilities();
    InitFarming();
    HOKSREAL();
    InitPawnHooks();
    InitVehicleHooks();

    // Enable all hooks
    MH_EnableHook(MH_ALL_HOOKS);
    Log("All hooks enabled!");
}

// Wait for player login
static void WaitForLogin() {
    Log("Waiting for login!");

    FName Frontend = UKismetStringLibrary::Conv_StringToName(L"Frontend");
    FName MatchState = UKismetStringLibrary::Conv_StringToName(L"InProgress");

    while (true) {
        UWorld* CurrentWorld = ((UWorld*)UWorld::GetWorld());
        if (CurrentWorld) {
            if (CurrentWorld->Name == Frontend) {
                auto GameMode = (AGameMode*)CurrentWorld->AuthorityGameMode;
                if (GameMode->GetMatchState() == MatchState) {
                    break;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 1));
    Log("Logged in!");
}

// Main thread function
DWORD Main(LPVOID) {
    InitConsole();
    InitGObjects();

    // Wait for engine to be ready
    while (UEngine::GetEngine() == 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    WaitForLogin();

    // Execute console command for logging
    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogPackageLocalizationCache", nullptr);

    // Set client/server flags
    *(bool*)(ImageBase + 0x637925B) = false; // GIsClient
    *(bool*)(ImageBase + 0x637925C) = true;  // GIsServer

    // Wait for world to be ready
    while (UWorld::GetWorld() == nullptr)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Num() == 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    Sleep(1000);
    LoadWorld();

    // Initialize bot manager
    if (Globals::bBotsEnabled) {
        ServerBotManager::Initialize();
        Log("Bot manager initialized!");
    }

    // Initialize backend if enabled
    if (Globals::bBackendEnabled) {
        Backend::Setup();
        Log("Backend initialized!");
    }

    // Apply various patches and hooks from Volcano
    auto Addr = Hooking::GetOffsetBRUH(0xFF343C);
    DWORD oldProtection;
    VirtualProtect((LPVOID)Addr, 1, PAGE_EXECUTE_READWRITE, &oldProtection);
    *(uint8_t*)Addr = 0x85;
    VirtualProtect((LPVOID)Addr, 1, oldProtection, &oldProtection);

    // Additional Volcano hooks
    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x2D39300), TickFlushHook, (void**)&TickFlushOG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x2D39300));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0xCF2E80), DispatchReqHook, (void**)&DispatchReqOG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0xCF2E80));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x2C03D20), KickPlayer, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x2C03D20));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x12E7410), ChangeGameSessionId, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x12E7410));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x15392D0), ValFailure1, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x15392D0));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0xBCFB60), NoReserve, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0xBCFB60));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x30100A0), UWorldGetNetMode, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x30100A0));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x29A40F0), AActorGetNetMode, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x29A40F0));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x13876D0), NoMcp, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x13876D0));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x8C1F70), K2_CommitExecuteHook, (void**)&K2_CommitExecute);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x8C1F70));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0xFA9B20), PickTeamHook, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0xFA9B20));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0xFB9830), SetMegaStormStuffHOOK, (void**)&SetMegaStormStuffidkREALOG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0xFB9830));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x18A7A60), sub_7FF6B9B17A60, (void**)&sub_7FF6B9B17A60_OG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x18A7A60));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x830630), CanActivateAbility, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x830630));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x1E054E0), CollectGarbage, nullptr);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x1E054E0));

    // Virtual hook for max tick rate
    Hooking::VirtualHook(GetEngine(), 0x50, GetMaxTickRate);

    // NotifyAcceptingConnection patch
    auto NotifyAcceptingConnectionSomethingIDK = Hooking::GetOffsetBRUH(0x3012AEA);
    auto BYTE = (uint8_t*)NotifyAcceptingConnectionSomethingIDK;
    Log("Old NotifyAcceptingConnection byte: 0x%X", *BYTE);
    *BYTE = 0x74;
    Log("New NotifyAcceptingConnection byte: 0x%X", *BYTE);

    Log("Project Reboot 8.51 initialized successfully!");
    Log("STATICS 8.51 edition - All systems operational");

    return 1;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}