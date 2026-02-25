#pragma once
#include "framework.h"
#include "ServerBotManager.h"
#include "BotNames.h"
#include "PlayerBots.h"

namespace Bots {
	void SpawnPlayerBot(AActor* OverrideSpawnLoc = nullptr, PlayerBots::EBotState StartingState = PlayerBots::EBotState::Warmup) {
		static UBehaviorTree* PhoebeBehaviorTree = StaticLoadObject<UBehaviorTree>("/Game/Athena/AI/Phoebe/BehaviorTrees/BT_Phoebe.BT_Phoebe");
		static UClass* PhoebePawnClass = StaticLoadObject<UClass>("/Game/Athena/AI/Phoebe/BP_PlayerPawn_Athena_Phoebe.BP_PlayerPawn_Athena_Phoebe_C");

		static int MaxSpawnLocations = PlayerStarts.Num();
		static int BotSpawnLocationIndex = 0;

		auto GameMode = GetGameMode();
		auto GameState = GetGameState();

		if (!GameMode || !GameState)
		{
			LOG_("GameMode or GameState is null, cannot spawn bot");
			return;
		}

		// Create bot customization data
		UFortAthenaAIBotCustomizationData* BotCustomizationData = (UFortAthenaAIBotCustomizationData*)GetStatics()->SpawnObject(UFortAthenaAIBotCustomizationData::StaticClass(), GameMode);
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

		// Add pickaxe
		static UFortWeaponMeleeItemDefinition* PickDef = StaticLoadObject<UFortWeaponMeleeItemDefinition>("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
		if (!Pickaxes.empty()) {
			int randomIdx = GetMath()->RandomIntegerInRange(0, Pickaxes.size() - 1);
			PickDef = Pickaxes[randomIdx]->WeaponDefinition;
		}

		if (PickDef) {
			BotCustomizationData->StartupInventory->Items.Add(PickDef);
		}

		// Add building items
		if (Floor && Roof && Stair && Wall && EditTool) {
			BotCustomizationData->StartupInventory->Items.Add(Floor);
			BotCustomizationData->StartupInventory->Items.Add(Roof);
			BotCustomizationData->StartupInventory->Items.Add(Stair);
			BotCustomizationData->StartupInventory->Items.Add(Wall);
			BotCustomizationData->StartupInventory->Items.Add(EditTool);
			LOG_("Added building items to bot inventory");
		}
		else
		{
			LOG_("WARNING: Building items are null!");
		}

		// Determine spawn location
		if (BotSpawnLocationIndex >= MaxSpawnLocations) {
			BotSpawnLocationIndex = 0;
		}
		
		if (PlayerStarts.Num() == 0)
		{
			LOG_("ERROR: No PlayerStarts available!");
			return;
		}
		
		AActor* BotSpawn = PlayerStarts[BotSpawnLocationIndex];
		BotSpawnLocationIndex++;
		
		if (OverrideSpawnLoc) {
			BotSpawn = OverrideSpawnLoc;
		}

		// Spawn the bot
		if (!GameMode->ServerBotManager)
		{
			LOG_("ERROR: ServerBotManager is null!");
			return;
		}

		AFortPlayerPawnAthena* Pawn = GameMode->ServerBotManager->SpawnBot(BotSpawn->K2_GetActorLocation(), BotSpawn->K2_GetActorRotation(), BotCustomizationData);
		if (!Pawn)
		{
			LOG_("ERROR: Failed to spawn bot pawn");
			return;
		}

		AFortAthenaAIBotController* PC = (AFortAthenaAIBotController*)Pawn->Controller;
		if (!PC)
		{
			LOG_("ERROR: Bot controller is null");
			return;
		}

		AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
		if (!PlayerState)
		{
			LOG_("ERROR: Bot PlayerState is null");
			return;
		}

		// Set bot name
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

		// Set starting state
		if (StartingState != PlayerBots::EBotState::Warmup) {
			for (PlayerBots::PlayerBot* Bot : PlayerBots::PlayerBotArray) {
				if (Bot->Pawn == Pawn) {
					Bot->BotState = StartingState;
				}
			}
		}

		// Assign team
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

		LOG_("Successfully spawned bot with team index: {}", Ret);
	}
}
