// dllmain.cpp : Point d'entrée pour le Statics Gameserver V1.0.0
#include "framework.h"
#include "GameMode.h"
#include "PC.h"
#include "Farming.h"
#include "Teams.h"
#include "Pawn.h"
#include "Vehicles.h"
#include "backend.h"
#include "Bots.h"
#include "PlayerBots.h"
#include "ServerBotManager.h"

DWORD Main(LPVOID)
{
    AllocConsole();
    FILE* streamPtr;
    freopen_s(&streamPtr, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Statics Gameserver V1.0.0 - Fortnite 8.51");

    LOG_("=====================================");
    LOG_("  STATICS GAMESERVER V1.0.0");
    LOG_("  Fortnite Chapter 1 - Season 8");
    LOG_("  Advanced Bot System Enabled");
    LOG_("=====================================");

    MH_Initialize();
    InitGObjects();

    Sleep(5000); // Reboot Launcher wait

    LOG_("Loading Athena Terrain...");

    ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(GetWorld(), L"open Athena_Terrain", nullptr);
    GetEngine()->GameInstance->LocalPlayers.Remove(0);

    // Log VTable offsets for debugging
    void** VTAblAAAA = *(void***)GetDefObj<UAbilitySystemComponent>();
    LOG_("AbilitySystemComponent VFT: 0x{:x}", __int64(VTAblAAAA) - __int64(GetModuleHandleW(0)));

    void** VFTYAYONG = *(void***)GetDefObj<AAthena_PlayerController_C>();
    LOG_("PlayerController VFT: 0x{:x}", __int64(VFTYAYONG) - __int64(GetModuleHandleW(0)));

    void** TESTSETESTEST = *(void***)GetDefObj<AFortGameSessionDedicatedAthena>();
    LOG_("GameSessionAthena VFT: 0x{:x}", __int64(TESTSETESTEST) - __int64(GetModuleHandleW(0)));

    void** WWGang = *(void***)GetDefObj<APlayerPawn_Athena_C>();
    LOG_("PlayerPawn_Athena_C VFT: 0x{:x}", __int64(WWGang) - __int64(GetModuleHandleW(0)));

    void** cccc = *(void***)GetDefObj<UFortGameplayAbility>();
    LOG_("UFortGameplayAbility VFT: 0x{:x}", __int64(cccc) - __int64(GetModuleHandleW(0)));

    // Patch warmup crash
    auto Addr = GetOffsetBRUH(0xFF343C);
    DWORD oldProtection;
    VirtualProtect((LPVOID)Addr, 1, PAGE_EXECUTE_READWRITE, &oldProtection);
    *(uint8_t*)Addr = 0x85;
    DWORD tempProtection;
    VirtualProtect((LPVOID)Addr, 1, oldProtection, &tempProtection);
    LOG_("Warmup crash patch applied");

    // Install hooks
    LOG_("Installing hooks...");

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x2D39300), TickFlushHook, (void**)&TickFlushOG);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x2D39300));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0xCF2E80), DispatchReqHook, (void**)&DispatchReqOG);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0xCF2E80));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x2C03D20), KickPlayer, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x2C03D20));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x12E7410), ChangeGameSessionId, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x12E7410));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x15392D0), ValFailure1, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x15392D0));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0xBCFB60), NoReserve, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0xBCFB60));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x30100A0), UWorldGetNetMode, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x30100A0));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x29A40F0), AActorGetNetMode, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x29A40F0));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x13876D0), NoMcp, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x13876D0));

    LOG_("Core hooks installed");

    // Initialize subsystems
    InitHoksPC();
    LOG_("PC hooks initialized");
    
    InitAbilities();
    LOG_("Abilities initialized");
    
    InitFarming();
    LOG_("Farming initialized");
    
    InitGameModeHooks();
    LOG_("GameMode hooks initialized");
    
    InitPawnHooks();
    LOG_("Pawn hooks initialized");
    
    InitVehicleHooks();
    LOG_("Vehicle hooks initialized");

    Backend::Setup();
    LOG_("Backend initialized");

    // Setup server tickrate
    VirtualHook(GetEngine(), 0x50, GetMaxTickRate);
    LOG_("Server tickrate set to 30 Hz");

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x1E054E0), CollectGarbage, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x1E054E0));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0xFA9B20), PickTeamHook, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0xFA9B20));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0xFB9830), SetMegaStormStuffHOOK, (void**)&SetMegaStormStuffidkREALOG);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0xFB9830));
    
    MH_CreateHook((LPVOID)GetOffsetBRUH(0x18A7A60), sub_7FF6B9B17A60, (void**)&sub_7FF6B9B17A60_OG);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x18A7A60));

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x830630), CanActivateAbility, nullptr);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x830630));

    auto NotifyAcceptingConnectionSomethingIDK = GetOffsetBRUH(0x3012AEA);
    auto BYTE = (uint8_t*)NotifyAcceptingConnectionSomethingIDK;
    LOG_("Connection byte patch: Old=0x{:x}, New=0x74", *BYTE);
    *BYTE = 0x74;

    MH_CreateHook((LPVOID)GetOffsetBRUH(0x8C1F70), K2_CommitExecuteHook, (void**)&K2_CommitExecute);
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x8C1F70));

    LOG_("=====================================");
    LOG_("  All hooks installed successfully!");
    LOG_("  Server is ready for connections");
    LOG_("  Bot system is operational");
    LOG_("=====================================");

    return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
