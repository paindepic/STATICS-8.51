#pragma once
#include "framework.h"
#include "PC.h"

namespace StaticsGameServer
{
    struct BotConfig
    {
        FString BotName;
        int32 DesiredTeam;
        bool bIsActive;
        FVector SpawnLocation;
        FRotator SpawnRotation;
    };

    static TArray<BotConfig> BotConfigurations;
    static int32 MaxBots = 10;
    static bool bBotsInitialized = false;

    inline void InitializeBotConfigs()
    {
        if (bBotsInitialized)
            return;

        for (int32 i = 0; i < MaxBots; i++)
        {
            BotConfig Config;
            Config.BotName = FString(std::wstring(L"Bot_" + std::to_wstring(i)).c_str());
            Config.DesiredTeam = (i % 2) + 2;
            Config.bIsActive = false;
            Config.SpawnLocation = FVector(0.0f, 0.0f, 10000.0f);
            Config.SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
            BotConfigurations.Add(Config);
        }

        bBotsInitialized = true;
    }

    inline AFortPlayerPawnAthena* SpawnBotAtLocation(const FVector& Location, const FRotator& Rotation)
    {
        if (!GetWorld() || !GetGameMode())
            return nullptr;

        FTransform SpawnTransform;
        SpawnTransform.Translation = Location;
        SpawnTransform.Rotation = FRotToQuat(Rotation);
        SpawnTransform.Scale3D = FVector(1.0f, 1.0f, 1.0f);

        AFortPlayerPawnAthena* BotPawn = nullptr;
        
        static UClass* BotPawnClass = StaticFindObject<UClass>("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
        if (BotPawnClass)
        {
            BotPawn = (AFortPlayerPawnAthena*)GetStatics()->BeginDeferredActorSpawnFromClass(
                GetWorld(), 
                BotPawnClass, 
                SpawnTransform, 
                ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
            );

            if (BotPawn)
            {
                GetStatics()->FinishSpawningActor(BotPawn, SpawnTransform);
            }
        }

        return BotPawn;
    }

    inline void SetupBotInventory(AFortPlayerControllerAthena* BotPC)
    {
        if (!BotPC || !BotPC->WorldInventory)
            return;

        static UFortItemDefinition* WoodItem = UObject::FindObject<UFortItemDefinition>("WoodItemData.WoodItemData");
        static UFortItemDefinition* StoneItem = UObject::FindObject<UFortItemDefinition>("StoneItemData.StoneItemData");
        static UFortItemDefinition* MetalItem = UObject::FindObject<UFortItemDefinition>("MetalItemData.MetalItemData");

        if (WoodItem)
            Inventory::AddItem(BotPC, WoodItem, 500);
        if (StoneItem)
            Inventory::AddItem(BotPC, StoneItem, 300);
        if (MetalItem)
            Inventory::AddItem(BotPC, MetalItem, 200);

        static UAthenaPickaxeItemDefinition* DefaultPickaxe = UObject::FindObject<UAthenaPickaxeItemDefinition>("DefaultPickaxe.DefaultPickaxe");
        if (DefaultPickaxe && DefaultPickaxe->WeaponDefinition)
        {
            Inventory::AddItem(BotPC, DefaultPickaxe->WeaponDefinition, 1);
        }
    }

    inline void AssignBotToTeam(AFortPlayerStateAthena* BotPlayerState, int32 TeamId)
    {
        if (!BotPlayerState)
            return;

        BotPlayerState->TeamIndex = TeamId;
        BotPlayerState->SquadId = TeamId - 2;
        
        BotPlayerState->OnRep_PlayerTeam();
        BotPlayerState->OnRep_PlayerTeamPrivate();
        BotPlayerState->OnRep_TeamIndex(0);
        BotPlayerState->OnRep_SquadId();

        FGameMemberInfo MemberInfo;
        MemberInfo.TeamIndex = BotPlayerState->TeamIndex;
        MemberInfo.SquadId = BotPlayerState->SquadId;
        MemberInfo.MemberUniqueId = BotPlayerState->UniqueId;

        GetGameState()->GameMemberInfoArray.Members.Add(MemberInfo);
        GetGameState()->GameMemberInfoArray.MarkArrayDirty();
    }

    inline void ProcessBotLogic(AFortPlayerPawnAthena* BotPawn)
    {
        if (!BotPawn || !BotPawn->Controller)
            return;

        auto BotPC = Cast<AFortPlayerControllerAthena>(BotPawn->Controller);
        if (!BotPC)
            return;

        static float LastBotUpdateTime = 0.0f;
        float CurrentTime = GetStatics()->GetTimeSeconds(GetWorld());
        
        if (CurrentTime - LastBotUpdateTime < 1.0f)
            return;

        LastBotUpdateTime = CurrentTime;

        if (BotPawn->GetHealth() <= 0.0f)
        {
            if (GetGameState()->IsRespawningAllowed((AFortPlayerStateAthena*)BotPC->PlayerState))
            {
                BotPC->RespawnPlayerAfterDeath(true);
            }
        }
    }

    inline void SpawnBotsForTesting(int32 NumBots)
    {
        if (!GetWorld() || !GetGameMode())
            return;

        InitializeBotConfigs();

        for (int32 i = 0; i < NumBots && i < BotConfigurations.Num(); i++)
        {
            FVector SpawnLoc = BotConfigurations[i].SpawnLocation;
            SpawnLoc.X += i * 200.0f;
            
            auto BotPawn = SpawnBotAtLocation(SpawnLoc, BotConfigurations[i].SpawnRotation);
            if (BotPawn)
            {
                BotConfigurations[i].bIsActive = true;
                
                if (BotPawn->Controller)
                {
                    auto BotPC = Cast<AFortPlayerControllerAthena>(BotPawn->Controller);
                    if (BotPC && BotPC->PlayerState)
                    {
                        auto BotPS = (AFortPlayerStateAthena*)BotPC->PlayerState;
                        AssignBotToTeam(BotPS, BotConfigurations[i].DesiredTeam);
                        SetupBotInventory(BotPC);
                        GrantAbilitySet(BotPS);
                    }
                }
            }
        }
    }

    inline void UpdateAllBots()
    {
        for (int32 i = 0; i < BotConfigurations.Num(); i++)
        {
            if (BotConfigurations[i].bIsActive)
            {
            }
        }
    }

    inline void CleanupInactiveBots()
    {
        for (int32 i = 0; i < BotConfigurations.Num(); i++)
        {
            if (BotConfigurations[i].bIsActive)
            {
            }
        }
    }

    static void (*ServerAttemptAircraftJumpOG)(AFortPlayerControllerAthena* a1, FRotator& a2);
    static void ServerAttemptAircraftJumpHook(AFortPlayerControllerAthena* PC, FRotator& Rotation)
    {
        if (!PC || !PC->PlayerState)
        {
            return ServerAttemptAircraftJumpOG(PC, Rotation);
        }

        auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
        
        if (PlayerState->bIsABot)
        {
            FVector BotJumpLocation = GetGameMode()->SafeZoneLocations.Num() > 0 ? 
                GetGameMode()->SafeZoneLocations[0] : FVector(0.0f, 0.0f, 10000.0f);
            BotJumpLocation.Z = 14567.0f;

            FTransform SpawnTransform;
            SpawnTransform.Translation = BotJumpLocation;
            SpawnTransform.Scale3D = FVector(1.0f, 1.0f, 1.0f);

            auto NewPawn = GetGameMode()->SpawnDefaultPawnAtTransform(PC, SpawnTransform);
            if (NewPawn)
            {
                PC->Possess(NewPawn);
                return;
            }
        }

        return ServerAttemptAircraftJumpOG(PC, Rotation);
    }

    inline void InitBotHooks()
    {
        MH_CreateHook((LPVOID)GetOffsetBRUH(0x1003280), ServerAttemptAircraftJumpHook, (void**)&ServerAttemptAircraftJumpOG);
        MH_EnableHook((LPVOID)GetOffsetBRUH(0x1003280));
    }
}
