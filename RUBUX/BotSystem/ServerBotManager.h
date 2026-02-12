#pragma once
#include "framework.h"
#include "PlayerBots.h"

namespace ServerBotManager
{
	inline AFortPlayerPawnAthena* FinalizeBot(AFortPlayerPawnAthena* Pawn, UFortAthenaAIBotCustomizationData* BotData)
	{
		if (!Pawn)
		{
			return nullptr;
		}

		auto PC = (AFortAthenaAIBotController*)Pawn->Controller;
		if (!PC)
		{
			Log("UFortServerBotManagerAthena::SpawnBot, spawning controller");
			PC = SpawnActor<AFortAthenaAIBotController>({});
			if (PC)
			{
				PC->Possess(Pawn);
			}
		}

		auto PlayerState = PC ? (AFortPlayerStateAthena*)PC->PlayerState : nullptr;
		if (!PC || !PlayerState)
		{
			Log("UFortServerBotManagerAthena::SpawnBot failed to get controller/playerstate");
			return Pawn;
		}

		if (!Characters.empty())
		{
			auto CID = Characters[GetMath()->RandomIntegerInRange(0, Characters.size() - 1)];
			if (CID && CID->HeroDefinition && CID->HeroDefinition->Specializations.IsValid())
			{
				for (size_t i = 0; i < CID->HeroDefinition->Specializations.Num(); i++)
				{
					UFortHeroSpecialization* Spec = StaticLoadObject<UFortHeroSpecialization>(GetString()->Conv_NameToString(CID->HeroDefinition->Specializations[i].ObjectID.AssetPathName).ToString());
					if (Spec)
					{
						for (size_t j = 0; j < Spec->CharacterParts.Num(); j++)
						{
							UCustomCharacterPart* Part = StaticLoadObject<UCustomCharacterPart>(GetString()->Conv_NameToString(Spec->CharacterParts[j].ObjectID.AssetPathName).ToString());
							if (Part)
							{
								PlayerState->CharacterData.Parts[(uintptr_t)Part->CharacterPartType] = Part;
							}
						}
					}
				}
			}
			if (CID)
			{
				Pawn->CosmeticLoadout.Character = CID;
			}
		}

		if (!Backpacks.empty() && GetMath()->RandomBoolWithWeight(0.5f))
		{
			auto Backpack = Backpacks[GetMath()->RandomIntegerInRange(0, Backpacks.size() - 1)];
			for (size_t j = 0; j < Backpack->CharacterParts.Num(); j++)
			{
				UCustomCharacterPart* Part = Backpack->CharacterParts[j];
				if (Part)
				{
					PlayerState->CharacterData.Parts[(uintptr_t)Part->CharacterPartType] = Part;
				}
			}
		}

		if (!Gliders.empty())
		{
			auto Glider = Gliders[GetMath()->RandomIntegerInRange(0, Gliders.size() - 1)];
			Pawn->CosmeticLoadout.Glider = Glider;
		}

		if (!Contrails.empty() && GetMath()->RandomBoolWithWeight(0.95f))
		{
			auto Contrail = Contrails[GetMath()->RandomIntegerInRange(0, Contrails.size() - 1)];
			Pawn->CosmeticLoadout.SkyDiveContrail = Contrail;
		}

		for (size_t i = 0; i < Dances.size(); i++)
		{
			Pawn->CosmeticLoadout.Dances.Add(Dances.at(i));
		}

		Pawn->OnRep_CosmeticLoadout();
		PlayerState->OnRep_CharacterData();

		if (BotData)
		{
			PC->BehaviorTree = BotData->BehaviorTree;
			if (PC->BehaviorTree)
			{
				PC->RunBehaviorTree(PC->BehaviorTree);
			}
		}

		Pawn->SetMaxHealth(100.f);
		Pawn->SetMaxShield(100.f);
		Pawn->SetHealth(100.f);
		Pawn->SetShield(0.f);

		if (!PC->Inventory)
		{
			PC->Inventory = SpawnActor<AFortInventory>({});
			PC->Inventory->Owner = PC;
			PC->Inventory->OnRep_Owner();
		}

		auto Bot = new PlayerBots::PlayerBot(Pawn, PC, PlayerState);

		if (PC->Inventory && BotData && BotData->StartupInventory)
		{
			for (auto item : BotData->StartupInventory->Items)
			{
				if (item)
				{
					Bot->GiveItem(item, 1, 0);
				}
			}
		}
		else
		{
			Log("Bot inventory missing");
		}

		Bot->EquipPickaxe();
		return Bot->Pawn;
	}

	inline AFortPlayerPawnAthena* SpawnBot(AFortGameModeAthena* GameMode, FVector SpawnLoc, FRotator SpawnRot, UFortAthenaAIBotCustomizationData* BotData)
	{
		if (!GameMode || !GameMode->ServerBotManager)
		{
			return nullptr;
		}

		auto Pawn = GameMode->ServerBotManager->SpawnBot(SpawnLoc, SpawnRot, BotData);
		return FinalizeBot(Pawn, BotData);
	}

	inline void HookAll()
	{
		Log("ServerBotManager ready");
	}
}
