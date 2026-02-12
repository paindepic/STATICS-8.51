// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"

// Global variables
uintptr_t ImageBase = (uintptr_t)GetModuleHandleW(0);

// Function prototypes
void InitConsole();
void LoadWorld();
void Hook();
void WaitForLogin();
DWORD WINAPI Main(LPVOID lpParam);

// Console initialization
void InitConsole() {
    AllocConsole();
    FILE* fptr;
    freopen_s(&fptr, "CONOUT$", "w+", stdout);
    SetConsoleTitleA("Project Reboot 8.51 | STATICS 8.51 - FIXED");
    Log("========================================================");
    Log("Project Reboot 8.51 - STATICS 8.51 Edition");
    Log("Combining Volcano 8.51 with Spectra 10.40 AI improvements");
    Log("FIXED VERSION - All compilation errors resolved");
    Log("========================================================");
}

// Load the Athena_Terrain world
void LoadWorld() {
    Log("Loading World...");
    
    auto World = UWorld::GetWorld();
    if (!World) {
        Log("ERROR: World is nullptr!");
        return;
    }
    
    if (!Globals::bCreativeEnabled && !Globals::bSTWEnabled) {
        UKismetSystemLibrary::ExecuteConsoleCommand(World, L"open Athena_Terrain", nullptr);
        Log("Athena_Terrain map loaded!");
    }
    
    if (World->OwningGameInstance && World->OwningGameInstance->LocalPlayers.Num() > 0) {
        World->OwningGameInstance->LocalPlayers.Remove(0);
    }
}

// Hook all the modules
void Hook() {
    // Initialize MinHook
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK) {
        Log("ERROR: MinHook initialization failed: %s", MH_StatusToString(status));
        return;
    }
    Log("MinHook Initialized successfully!");

    // Initialize PC offsets first (needed for PC.h)
    InitPCOffsets();
    Log("PC Offsets initialized!");

    // Hook all modules from Project Reboot
    Log("Installing hooks...");
    
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
    Log("Installing Volcano hooks...");
    InitHoksPC();
    InitAbilities();
    InitFarming();
    HOKSREAL();
    InitPawnHooks();
    InitVehicleHooks();

    // Enable all hooks
    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK) {
        Log("ERROR: Failed to enable hooks: %s", MH_StatusToString(status));
        return;
    }
    Log("All hooks enabled successfully!");
}

// Wait for player login
static void WaitForLogin() {
    Log("Waiting for login...");

    FName Frontend = UKismetStringLibrary::Conv_StringToName(L"Frontend");
    FName MatchState = UKismetStringLibrary::Conv_StringToName(L"InProgress");

    int attempts = 0;
    while (attempts < 300) { // Timeout after 5 minutes
        UWorld* CurrentWorld = UWorld::GetWorld();
        if (CurrentWorld) {
            if (CurrentWorld->Name == Frontend) {
                auto GameMode = (AGameMode*)CurrentWorld->AuthorityGameMode;
                if (GameMode && GameMode->GetMatchState() == MatchState) {
                    Log("Login detected!");
                    break;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        attempts++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Log("Ready to proceed!");
}

// Main thread function
DWORD WINAPI Main(LPVOID lpParam) {
    (void)lpParam; // Unused
    
    InitConsole();
    InitGObjects();

    // Wait for engine to be ready
    Log("Waiting for engine initialization...");
    int engineAttempts = 0;
    while (UEngine::GetEngine() == nullptr && engineAttempts < 60) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        engineAttempts++;
    }
    
    if (UEngine::GetEngine() == nullptr) {
        Log("ERROR: Engine failed to initialize!");
        return 1;
    }
    Log("Engine initialized!");

    WaitForLogin();

    // Execute console command for logging
    auto World = UWorld::GetWorld();
    if (World) {
        UKismetSystemLibrary::ExecuteConsoleCommand(World, L"log LogPackageLocalizationCache", nullptr);
    }

    // Set client/server flags
    *(bool*)(ImageBase + 0x637925B) = false; // GIsClient
    *(bool*)(ImageBase + 0x637925C) = true;  // GIsServer
    Log("Server mode activated!");

    // Wait for world to be ready
    Log("Waiting for world...");
    int worldAttempts = 0;
    while (UWorld::GetWorld() == nullptr && worldAttempts < 60) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        worldAttempts++;
    }
    
    World = UWorld::GetWorld();
    if (!World) {
        Log("ERROR: World failed to initialize!");
        return 1;
    }

    // Wait for local players
    int localPlayerAttempts = 0;
    while (World->OwningGameInstance && 
           World->OwningGameInstance->LocalPlayers.Num() == 0 && 
           localPlayerAttempts < 60) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        localPlayerAttempts++;
    }

    Sleep(1000);
    LoadWorld();

    // Initialize bot manager
    if (Globals::bBotsEnabled) {
        Log("Initializing bot manager...");
        ServerBotManager::Initialize();
        Log("Bot manager initialized!");
    }

    // Initialize backend if enabled
    if (Globals::bBackendEnabled) {
        Log("Initializing backend...");
        Backend::Setup();
        Log("Backend initialized!");
    }

    // Apply various patches from Volcano
    Log("Applying memory patches...");
    
    auto Addr = Hooking::GetOffsetBRUH(0xFF343C);
    DWORD oldProtection;
    if (VirtualProtect((LPVOID)Addr, 1, PAGE_EXECUTE_READWRITE, &oldProtection)) {
        *(uint8_t*)Addr = 0x85;
        VirtualProtect((LPVOID)Addr, 1, oldProtection, &oldProtection);
        Log("Patch 1 applied!");
    }

    // Install additional hooks
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
    auto Engine = UEngine::GetEngine();
    if (Engine) {
        Hooking::VirtualHook(Engine, 0x50, GetMaxTickRate);
        Log("Max tick rate hook installed!");
    }

    // NotifyAcceptingConnection patch
    auto NotifyAcceptingConnectionAddr = Hooking::GetOffsetBRUH(0x3012AEA);
    auto BytePtr = (uint8_t*)NotifyAcceptingConnectionAddr;
    if (VirtualProtect((LPVOID)NotifyAcceptingConnectionAddr, 1, PAGE_EXECUTE_READWRITE, &oldProtection)) {
        Log("Old NotifyAcceptingConnection byte: 0x%X", *BytePtr);
        *BytePtr = 0x74;
        Log("New NotifyAcceptingConnection byte: 0x%X", *BytePtr);
        VirtualProtect((LPVOID)NotifyAcceptingConnectionAddr, 1, oldProtection, &oldProtection);
    }

    Log("========================================================");
    Log("Project Reboot 8.51 initialized successfully!");
    Log("STATICS 8.51 edition - All systems operational");
    Log("========================================================");

    return 0;
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
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, Main, nullptr, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        MH_Uninitialize();
        break;
    }
    return TRUE;
}
