#pragma once
#include "framework.h"

#include "ServerBotManager.h"
#include "BotNames.h"

namespace Bots {
    inline void InitializeBotData()
    {
        static bool bInitialized = false;
        if (bInitialized)
        {
            return;
        }

        auto GameMode = GetGameMode();
        auto GameState = GetGameState();
        if (!GameMode || !GameState)
        {
            return;
        }

        if (!GameMode->ServerBotManager)
        {
            auto BotManager = (UFortServerBotManagerAthena*)GetStatics()->SpawnObject(UFortServerBotManagerAthena::StaticClass(), GameMode);
            if (BotManager)
            {
                GameMode->ServerBotManager = BotManager;
                BotManager->CachedGameMode = GameMode;
                BotManager->CachedGameState = GameState;
            }
        }

        GetStatics()->GetAllActorsOfClass(GetWorld(), AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);
        GetStatics()->GetAllActorsOfClass(GetWorld(), ABuildingFoundation::StaticClass(), &BuildingFoundations);

        for (size_t i = 0; i < UObject::GObjects->Num(); i++)
        {
            auto Obj = UObject::GObjects->GetByIndex(i);
            if (Obj && Obj->IsA(UAthenaCharacterItemDefinition::StaticClass()))
            {
                std::string SkinsData = ((UAthenaCharacterItemDefinition*)Obj)->Name.ToString();
                if (SkinsData.contains("Athena_Commando") || SkinsData.contains("CID_Character") || !SkinsData.contains("CID_NPC") || !SkinsData.contains("CID_VIP") || !SkinsData.contains("CID_TBD"))
                {
                    Characters.push_back((UAthenaCharacterItemDefinition*)Obj);
                }
            }
            if (Obj && Obj->IsA(UAthenaBackpackItemDefinition::StaticClass()))
            {
                Backpacks.push_back((UAthenaBackpackItemDefinition*)Obj);
            }
            if (Obj && Obj->IsA(UAthenaPickaxeItemDefinition::StaticClass()))
            {
                Pickaxes.push_back((UAthenaPickaxeItemDefinition*)Obj);
            }
            if (Obj && Obj->IsA(UAthenaDanceItemDefinition::StaticClass()))
            {
                std::string EmoteData = ((UAthenaDanceItemDefinition*)Obj)->Name.ToString();
                if (EmoteData.contains("EID") || !EmoteData.contains("Sync") || !EmoteData.contains("Owned"))
                {
                    Dances.push_back((UAthenaDanceItemDefinition*)Obj);
                }
            }
            if (Obj && Obj->IsA(UAthenaGliderItemDefinition::StaticClass()))
            {
                Gliders.push_back((UAthenaGliderItemDefinition*)Obj);
            }
        }

        bInitialized = true;
        Log("Bots initialized");
    }

    void SpawnPlayerBot(AActor* OverrideSpawnLoc = nullptr, PlayerBots::EBotState StartingState = PlayerBots::EBotState::Warmup) {
        static UBehaviorTree* PhoebeBehaviorTree = StaticLoadObject<UBehaviorTree>("Game/Athena/AI/Phoebe/BehaviorTrees/BT_Phoebe.BT_Phoebe");
        static UClass* PhoebePawnClass = StaticLoadObject<UClass>("/Game/Athena/AI/Phoebe/BP_PlayerPawn_Athena_Phoebe.BP_PlayerPawn_Athena_Phoebe_C");
        static int BotSpawnLocationIndex = 0;

        InitializeBotData();

        auto GameMode = GetGameMode();
        auto GameState = GetGameState();
        if (!GameMode || !GameState)
        {
            return;
        }

        int MaxSpawnLocations = PlayerStarts.Num();
        if (MaxSpawnLocations <= 0)
        {
            Log("No player starts for bots");
            return;
        }

        auto BotCustomizationData = (UFortAthenaAIBotCustomizationData*)GetStatics()->SpawnObject(UFortAthenaAIBotCustomizationData::StaticClass(), GameMode);
        BotCustomizationData->BehaviorTree = PhoebeBehaviorTree;
        BotCustomizationData->bOverrideBehaviorTree = true;
        BotCustomizationData->bOverrideCharacterCustomization = false;
        BotCustomizationData->bOverrideDBNOPlayStyle = false;
        BotCustomizationData->bOverrideSkillLevel = false;
        BotCustomizationData->bOverrideSkillSets = false;
        BotCustomizationData->bOverrideStartupInventory = false;
        BotCustomizationData->CharacterCustomization = {};
        BotCustomizationData->DBNOPlayStyle = EDBNOPlayStyle::Default;
        BotCustomizationData->PawnClass = PhoebePawnClass;
        BotCustomizationData->SkillLevel = 1;
        BotCustomizationData->SkillSets = {};
        BotCustomizationData->StartupInventory = (UFortAthenaAIBotInventoryItems*)GetStatics()->SpawnObject(UFortAthenaAIBotInventoryItems::StaticClass(), GameMode);

        static UFortWeaponMeleeItemDefinition* PickDef = StaticLoadObject<UFortWeaponMeleeItemDefinition>("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
        if (!Pickaxes.empty()) {
            PickDef = Pickaxes[GetMath()->RandomIntegerInRange(0, Pickaxes.size() - 1)]->WeaponDefinition;
        }

        if (PickDef) {
            BotCustomizationData->StartupInventory->Items.Add(PickDef);
        }

        if (Floor && Roof && Stair && Wall && EditTool) {
            BotCustomizationData->StartupInventory->Items.Add(Floor);
            BotCustomizationData->StartupInventory->Items.Add(Roof);
            BotCustomizationData->StartupInventory->Items.Add(Stair);
            BotCustomizationData->StartupInventory->Items.Add(Wall);
            BotCustomizationData->StartupInventory->Items.Add(EditTool);
        }
        else
        {
            Log("BuildingItem Null!");
        }

        if (BotSpawnLocationIndex >= MaxSpawnLocations) {
            BotSpawnLocationIndex = 0;
        }
        AActor* BotSpawn = PlayerStarts[BotSpawnLocationIndex];
        BotSpawnLocationIndex++;
        if (OverrideSpawnLoc) {
            BotSpawn = OverrideSpawnLoc;
        }
        AFortPlayerPawnAthena* Pawn = ServerBotManager::SpawnBot(GameMode, BotSpawn->K2_GetActorLocation(), BotSpawn->K2_GetActorRotation(), BotCustomizationData);
        if (!Pawn)
            return;
        AFortAthenaAIBotController* PC = (AFortAthenaAIBotController*)Pawn->Controller;
        if (!PC)
            return;
        AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
        if (!PlayerState)
            return;

        if (BotDisplayNames.size() != 0) {
            std::srand(static_cast<unsigned int>(std::time(0)));
            int randomIndex = std::rand() % BotDisplayNames.size();
            std::string rdName = BotDisplayNames[randomIndex];
            BotDisplayNames.erase(BotDisplayNames.begin() + randomIndex);

            int size_needed = MultiByteToWideChar(CP_UTF8, 0, rdName.c_str(), (int)rdName.size(), NULL, 0);
            std::wstring wideString(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, rdName.c_str(), (int)rdName.size(), &wideString[0], size_needed);

            FString CVName = FString(wideString.c_str());
            GameMode->ChangeName(PC, CVName, true);
            PlayerState->OnRep_PlayerName();
        }

        if (StartingState != PlayerBots::EBotState::Warmup) {
            for (PlayerBots::PlayerBot* Bot : PlayerBots::PlayerBotArray) {
                if (Bot->Pawn == Pawn) {
                    Bot->BotState = StartingState;
                }
            }
        }

        if (true)
        {
            int Ret = Globals::NextBotTeamIndex;

            ++Globals::CurrentBotsOnTeam;

            if (Globals::CurrentBotsOnTeam == Globals::MaxPlayersPerTeam)
            {
                Globals::NextBotTeamIndex++;
                Globals::CurrentBotsOnTeam = 0;
            }

            PlayerState->TeamIndex = Ret;
            PlayerState->OnRep_TeamIndex(0);

            PlayerState->SquadId = PlayerState->TeamIndex - 3;
            PlayerState->OnRep_SquadId();

            PlayerState->OnRep_PlayerTeam();
            PlayerState->OnRep_PlayerTeamPrivate();

            FGameMemberInfo NewMemberInfo{ -1,-1,-1 };
            NewMemberInfo.MemberUniqueId = PlayerState->GetUniqueID();
            NewMemberInfo.SquadId = PlayerState->SquadId;
            NewMemberInfo.TeamIndex = PlayerState->TeamIndex;

            GameState->GameMemberInfoArray.Members.Add(NewMemberInfo);
            GameState->GameMemberInfoArray.MarkItemDirty(NewMemberInfo);
        }
    }
}