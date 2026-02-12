#pragma once
#include "Teams.h"
#include "Looting.h"
#include "Bots.h"
#include "PlayerBots.h"

bool (*ReadyToStartMatchOG)(void*);
bool ReadyToStartMatchHook(AFortGameModeAthena* a1)
{
	static bool bPLAYLIST = false;
	if (!bPLAYLIST)
	{
		bPLAYLIST = true;

		auto playlist = UObject::FindObject<UFortPlaylistAthena>(Globals::PlaylistName);
		if (playlist)
		{
			GetGameState()->CurrentPlaylistInfo.BasePlaylist = playlist;
			GetGameState()->CurrentPlaylistInfo.OverridePlaylist = playlist;
			GetGameState()->CurrentPlaylistInfo.PlaylistReplicationKey++;
			GetGameState()->CurrentPlaylistInfo.MarkArrayDirty();

			GetGameState()->CurrentPlaylistId = playlist->PlaylistId;
			a1->CurrentPlaylistId = playlist->PlaylistId;
			a1->CurrentPlaylistName = playlist->PlaylistName;
			GetGameState()->CachedSafeZoneStartUp = playlist->SafeZoneStartUp;
			GetGameState()->OnRep_CurrentPlaylistId();
			GetGameState()->OnRep_CurrentPlaylistInfo();

			TeamIndex = playlist->DefaultFirstTeam;
			LOG_("FirstTeam: {}", TeamIndex);
			NumPlayerPerTeam = *(int*)(__int64(playlist) + 0x50);
			LOG_("MaxPlayerPerTeam: {}", NumPlayerPerTeam);
			Globals::MaxPlayersPerTeam = NumPlayerPerTeam;
			
			LOG_("GetGameMode()->WarmupRequiredPlayerCount {}", GetGameMode()->WarmupRequiredPlayerCount);
			GetGameMode()->WarmupRequiredPlayerCount = 1;
			LOG_("GetGameMode()->WarmupRequiredPlayerCount NEW {}", GetGameMode()->WarmupRequiredPlayerCount);

			LOG_("playlist->MaxPlayers: {}", playlist->MaxPlayers);
			a1->GameSession->MaxPlayers = playlist->MaxPlayers;
			a1->FortGameSession->MaxPlayers = playlist->MaxPlayers;
		}
	}

	if (!GetGameState()->MapInfo)
		return false;

	static bool bListneing = false;
	if (!bListneing)
	{
		bListneing = true;
		GetGameState()->OnRep_CurrentPlaylistInfo();
		
		// Initialize building items for bots
		Floor = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
		Roof = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
		Stair = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
		Wall = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
		EditTool = UObject::FindObject<UFortEditToolItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
		
		// Get player starts for bot spawning
		TArray<AActor*> TempPlayerStarts;
		GetStatics()->GetAllActorsOfClass(GetWorld(), UObject::FindObject<UClass>("Class Engine.PlayerStart"), &TempPlayerStarts);
		for (int i = 0; i < TempPlayerStarts.Num(); i++)
		{
			if (TempPlayerStarts[i])
				PlayerStarts.Add(TempPlayerStarts[i]);
		}
		LOG_("Found {} PlayerStarts", PlayerStarts.Num());
		TempPlayerStarts.Free();
		
		// Get building foundations
		auto BuildingFoundationClass = StaticLoadObject<UBlueprintGeneratedClass>("/Game/Building/ActorBlueprints/PlayerBuildableAbstract.PlayerBuildableAbstract_C");
		TArray<AActor*> TempFoundations;
		GetStatics()->GetAllActorsOfClass(GetWorld(), BuildingFoundationClass, &TempFoundations);
		for (int i = 0; i < TempFoundations.Num(); i++)
		{
			if (TempFoundations[i])
				BuildingFoundations.Add(TempFoundations[i]);
		}
		LOG_("Found {} BuildingFoundations", BuildingFoundations.Num());
		TempFoundations.Free();
		
		// Initialize looting
		InitLooting();

		// Spawn floor loot
		auto BR_FloorLoot_Class = StaticLoadObject<UBlueprintGeneratedClass>("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");
		TArray<AActor*> BR_FloorLootActors;
		GetStatics()->GetAllActorsOfClass(GetWorld(), BR_FloorLoot_Class, &BR_FloorLootActors);
		int SpawnedLoot = 0;
		for (int i = 0; i < BR_FloorLootActors.Num(); i++)
		{
			auto CurrentActor = BR_FloorLootActors[i];
			if (CurrentActor)
			{
				FVector Loc = CurrentActor->K2_GetActorLocation();
				Loc.Z += 30;
				SpawnedLoot++;
				auto loot = GetFloorLoot();
				for (auto& LootItem : loot)
				{
					SpawnPickup(LootItem->ItemDefinition, LootItem->DropCount, LootItem->LoadedAmmo, Loc, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
				}

				CurrentActor->K2_DestroyActor();
			}
		}
		BR_FloorLootActors.Free();

		auto Warmup_FloorLoot_Class = StaticLoadObject<UBlueprintGeneratedClass>("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_Warmup.Tiered_Athena_FloorLoot_Warmup_C");
		TArray<AActor*> Warmup_FloorlootActors;
		GetStatics()->GetAllActorsOfClass(GetWorld(), Warmup_FloorLoot_Class, &Warmup_FloorlootActors);
		for (int i = 0; i < Warmup_FloorlootActors.Num(); i++)
		{
			auto CurrentActor = Warmup_FloorlootActors[i];
			if (CurrentActor)
			{
				FVector Loc = CurrentActor->K2_GetActorLocation();
				Loc.Z += 30;
				SpawnedLoot++;
				auto loot = GetFloorLoot();
				for (auto& LootItem : loot)
				{
					SpawnPickup(LootItem->ItemDefinition, LootItem->DropCount, LootItem->LoadedAmmo, Loc, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
				}
				CurrentActor->K2_DestroyActor();
			}
		}
		Warmup_FloorlootActors.Free();

		LOG_("Spawned Loot: {}", SpawnedLoot);

		// Setup network listening
		Listen();
		a1->bWorldIsReady = true;

		// Setup warmup timing
		float TimeSeconds = GetStatics()->GetTimeSeconds(GetWorld());
		GetGameState()->WarmupCountdownEndTime = TimeSeconds + 120.f;
		GetGameMode()->WarmupCountdownDuration = 120.f;
		GetGameState()->WarmupCountdownStartTime = TimeSeconds;
		GetGameMode()->WarmupEarlyCountdownDuration = 120.f;
		
		LOG_("World is ready! Server initialized successfully!");
	}

	bool Ret = false;
	Ret = ReadyToStartMatchOG(a1);
	if (Ret == true)
	{
		LOG_("ReadyToStartMatch returned true!");
	}
	
	return Ret;
}

APawn* SpawnDefaultPawnForHook(AGameModeBase* a1, AController* NewPlayer, AActor* StartSpot)
{
	if (NewPlayer && StartSpot)
	{
		auto aa = StartSpot->GetTransform();
		return a1->SpawnDefaultPawnAtTransform(NewPlayer, aa);
	}
	return 0;
}

void InitGameModeHooks()
{
	MH_CreateHook((LPVOID)GetOffsetBRUH(0x307C130), ReadyToStartMatchHook, (void**)&ReadyToStartMatchOG);
	MH_EnableHook((LPVOID)GetOffsetBRUH(0x307C130));

	MH_CreateHook((LPVOID)GetOffsetBRUH(0xFB4A00), SpawnDefaultPawnForHook, nullptr);
	MH_EnableHook((LPVOID)GetOffsetBRUH(0xFB4A00));
}
