#pragma once
#include "framework.h"
#include "PC.h"

namespace StaticsGameserver
{
    // Bot management structure for Fortnite 8.51
    struct BotInfo
    {
        AController* BotController;
        AFortPlayerPawnAthena* BotPawn;
        FVector TargetLocation;
        bool bIsActive;
        float LastActionTime;
        int BotId;
    };

    static TArray<BotInfo> ActiveBots;
    static int NextBotId = 0;
    static bool bBotsInitialized = false;

    // Forward declarations
    void InitializeBots();
    void UpdateBots();
    void SpawnBotAtLocation(FVector Location);
    void RemoveBot(int BotId);
    void ClearAllBots();

    // Initialize bot system
    inline void InitializeBots()
    {
        if (bBotsInitialized)
            return;

        ActiveBots.Empty();
        NextBotId = 0;
        bBotsInitialized = true;

        LOG_("Bot system initialized for Fortnite 8.51");
    }

    // Spawn a bot at specific location
    inline void SpawnBotAtLocation(FVector Location)
    {
        if (!GetGameMode() || !GetWorld())
            return;

        BotInfo NewBot;
        NewBot.BotId = NextBotId++;
        NewBot.bIsActive = true;
        NewBot.LastActionTime = 0.0f;
        NewBot.TargetLocation = Location;

        // Spawn bot pawn
        FTransform SpawnTransform;
        SpawnTransform.Translation = Location;
        SpawnTransform.Scale3D = FVector{ 1.0f, 1.0f, 1.0f };

        auto BotPawn = SpawnActor<AFortPlayerPawnAthena>(AFortPlayerPawnAthena::StaticClass(), Location);
        if (BotPawn)
        {
            NewBot.BotPawn = BotPawn;
            BotPawn->SetMaxHealth(100.0f);
            BotPawn->SetHealth(100.0f);

            // Initialize bot abilities
            if (BotPawn->AbilitySystemComponent)
            {
                static auto AbilitySet = UObject::FindObject<UFortAbilitySet>("GAS_AthenaPlayer.GAS_AthenaPlayer");
                auto BotPlayerState = (AFortPlayerStateAthena*)BotPawn->PlayerState;
                if (AbilitySet && BotPlayerState)
                {
                    for (int i = 0; i < AbilitySet->GameplayAbilities.Num(); i++)
                    {
                        GrantAbility(BotPlayerState, AbilitySet->GameplayAbilities[i].Get());
                    }
                }
            }

            ActiveBots.Add(NewBot);
            LOG_("Bot {} spawned at [{}, {}, {}]", NewBot.BotId, Location.X, Location.Y, Location.Z);
        }
    }

    // Remove a specific bot
    inline void RemoveBot(int BotId)
    {
        for (int i = 0; i < ActiveBots.Num(); i++)
        {
            if (ActiveBots[i].BotId == BotId)
            {
                if (ActiveBots[i].BotPawn)
                {
                    ActiveBots[i].BotPawn->K2_DestroyActor();
                }
                ActiveBots.Remove(i);
                LOG_("Bot {} removed", BotId);
                return;
            }
        }
    }

    // Clear all bots
    inline void ClearAllBots()
    {
        for (int i = 0; i < ActiveBots.Num(); i++)
        {
            if (ActiveBots[i].BotPawn)
            {
                ActiveBots[i].BotPawn->K2_DestroyActor();
            }
        }
        ActiveBots.Empty();
        LOG_("All bots cleared");
    }

    // Update bot behaviors
    inline void UpdateBots()
    {
        if (!bBotsInitialized)
            return;

        float CurrentTime = GetStatics()->GetTimeSeconds(GetWorld());

        for (int i = 0; i < ActiveBots.Num(); i++)
        {
            auto& Bot = ActiveBots[i];
            if (!Bot.bIsActive || !Bot.BotPawn)
                continue;

            // Simple bot AI - move randomly every few seconds
            if (CurrentTime - Bot.LastActionTime > 5.0f)
            {
                Bot.LastActionTime = CurrentTime;

                // Generate random movement
                FVector RandomOffset(
                    (rand() % 2000 - 1000),
                    (rand() % 2000 - 1000),
                    0
                );

                Bot.TargetLocation = Bot.BotPawn->K2_GetActorLocation() + RandomOffset;

                // Simple move command
                Bot.BotPawn->AddMovementInput(RandomOffset.GetSafeNormal(), 1.0f);
            }
        }
    }

    // Spawn multiple bots around the map
    inline void SpawnBotWave(int Count)
    {
        if (!GetGameMode())
            return;

        for (int i = 0; i < Count; i++)
        {
            // Spawn at random safe zone location
            if (GetGameMode()->SafeZoneLocations.Num() > 0)
            {
                int RandomIndex = rand() % GetGameMode()->SafeZoneLocations.Num();
                FVector SpawnLoc = GetGameMode()->SafeZoneLocations[RandomIndex];
                SpawnLoc.X += (rand() % 10000 - 5000);
                SpawnLoc.Y += (rand() % 10000 - 5000);
                SpawnLoc.Z += 1000;

                SpawnBotAtLocation(SpawnLoc);
            }
        }
    }
}
