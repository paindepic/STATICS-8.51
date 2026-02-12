#pragma once

// CORRECTION: Abilites.h -> Abilities.h
#include "Abilities.h"
#include "Inventory.h"

// Forward declarations pour les fonctions de looting (définies dans Looting.h)
struct LootRow;
LootRow* GetRandomItem(EFortItemType ItemType);
void GetRandomMaterialCount(int* Wood, int* Stone, int* Metal);

// Déclaration externe pour l'initialisation
extern void InitPCOffsets();

// Pointeur de fonction statique initialisé dynamiquement
static void* (*sub_7FF6B99CFE30)(void*, void*) = nullptr;

inline void InitPCOffsets() {
    sub_7FF6B99CFE30 = decltype(sub_7FF6B99CFE30)(Hooking::GetOffsetBRUH(0x175FE30));
}

void ServerAcknowledgePossessionHook(AFortPlayerController* PC, APawn* P)
{
    if (!PC || !P) return;
    
    PC->AcknowledgedPawn = P;

    auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
    auto FortPawn = PC->MyFortPawn;
    if (PlayerState && FortPawn)
    {
        auto& CosmeticLoadout = PC->CosmeticLoadoutPC;
        if (CosmeticLoadout.Character)
        {
            if (CosmeticLoadout.Character->HeroDefinition)
            {
                FortPawn->CosmeticLoadout = CosmeticLoadout;
                FortPawn->OnRep_CosmeticLoadout();

                PlayerState->HeroType = CosmeticLoadout.Character->HeroDefinition;
                PlayerState->OnRep_HeroType();

                if (sub_7FF6B99CFE30) {
                    sub_7FF6B99CFE30(PlayerState, FortPawn);
                }
            }
        }
    }
}

void (*ServerReadyToStartMatchOG)(AController*) = nullptr;

void ServerReadyToStartMatchHook(AFortPlayerController* PC)
{
    if (!PC) {
        if (ServerReadyToStartMatchOG) ServerReadyToStartMatchOG(PC);
        return;
    }

    auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
    if (PlayerState)
    {
        GrantAbilitySet(PlayerState);
    }

    auto GameMode = GetGameMode();
    if (GameMode) {
        for (int i = 0; i < GameMode->StartingItems.Num(); i++)
        {
            Inventory::AddItem(PC, GameMode->StartingItems[i].Item, GameMode->StartingItems[i].Count);
        }
    }

    static auto YAYA = UObject::FindObject<UAthenaPickaxeItemDefinition>("DefaultPickaxe.DefaultPickaxe");
    auto& test = PC->CosmeticLoadoutPC;
    if (test.Pickaxe)
    {
        LOG_("valid pickaxe!!!");
        LOG_("aa: {}", test.Pickaxe->GetName().ToString().c_str());
    }

    Inventory::AddItem(PC, test.Pickaxe ? test.Pickaxe->WeaponDefinition : YAYA->WeaponDefinition, 1);

    LOG_("TeamIndex: {}", PlayerState ? PlayerState->TeamIndex : -1);
    
    if (PlayerState) {
        PlayerState->SquadId = PlayerState->TeamIndex - 2;
        PlayerState->OnRep_PlayerTeam();
        PlayerState->OnRep_PlayerTeamPrivate();
        PlayerState->OnRep_TeamIndex(0);
        PlayerState->OnRep_SquadId();

        FGameMemberInfo test{ -1,-1,-1 };
        test.TeamIndex = PlayerState->TeamIndex;
        test.SquadId = PlayerState->SquadId;
        test.MemberUniqueId = PlayerState->UniqueId;

        auto GameState = GetGameState();
        if (GameState) {
            GameState->GameMemberInfoArray.Members.Add(test);
            GameState->GameMemberInfoArray.MarkArrayDirty();
        }
    }

    if (ServerReadyToStartMatchOG) ServerReadyToStartMatchOG(PC);
}

void ServerExecuteInventoryItem(AFortPlayerController* PC, FGuid& ItemGuid)
{
    if (!PC) return;
    
    if (auto Pawn = (AFortPlayerPawn*)PC->Pawn)
    {
        if (auto ItemEntry = Inventory::FindItemEntry(PC, ItemGuid))
        {
            Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)ItemEntry->ItemDefinition, ItemEntry->ItemGuid);
        }
    }
}

static bool (*CantBuild)(UWorld*, UObject*, FVector, FRotator, char, void*, char*) = nullptr;

void ServerCreateBuildingActorHook(AFortPlayerControllerAthena* PC, FCreateBuildingActorData& CreateBuildingData)
{
    if (!PC) return;
    
    if (!CantBuild) {
        CantBuild = decltype(CantBuild)(Hooking::GetOffsetBRUH(0x1330D70));
    }

    auto BroadcastClientInfo = *(AFortBroadcastRemoteClientInfo**)(__int64(PC) + 0x28D8);
    if (!BroadcastClientInfo) return;
    
    auto Class = BroadcastClientInfo->RemoteBuildableClass.Get();
    if (!Class) return;

    TArray<AActor*> BuildingActorsToDestroy;
    char Result;
    
    auto World = GetWorld();
    if (!World) return;
    
    if (!CantBuild(World, Class, CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, CreateBuildingData.bMirrored, &BuildingActorsToDestroy, &Result))
    {
        for (int i = 0; i < BuildingActorsToDestroy.Num(); i++)
        {
            if (BuildingActorsToDestroy[i]) {
                BuildingActorsToDestroy[i]->K2_DestroyActor();
            }
        }
        BuildingActorsToDestroy.Free();

        FTransform Transform;
        Transform.Translation = CreateBuildingData.BuildLoc;
        Transform.Rotation = CreateBuildingData.BuildRot;
        Transform.Scale3D = FVector{ 1,1,1 };
        
        auto NewBuilding = (ABuildingSMActor*)UGameplayStatics::GetDefaultObj()->BeginDeferredActorSpawnFromClass(
            World, Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, nullptr);
            
        if (NewBuilding)
        {
            NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, PC, true);
            NewBuilding->bPlayerPlaced = true;
            
            auto PS = (AFortPlayerStateAthena*)PC->PlayerState;
            if (PS) {
                NewBuilding->Team = EFortTeam(PS->TeamIndex);
                NewBuilding->TeamIndex = PS->TeamIndex;
                NewBuilding->OnRep_Team();
            }

            if(!PC->bBuildFree) {
                auto FortKismet = (UFortKismetLibrary*)UFortKismetLibrary::StaticClass()->DefaultObject;
                if (FortKismet) {
                    auto ResourceDef = FortKismet->K2_GetResourceItemDefinition(NewBuilding->ResourceType);
                    if (ResourceDef) {
                        Inventory::RemoveItem(PC, ResourceDef, 10);
                    }
                }
            }
        }
    }
}

void ServerBeginEditingBuildingActorHook(AFortPlayerController* PC, ABuildingSMActor* BuildingActorToEdit)
{
    if (!PC || !BuildingActorToEdit) return;
    
    auto Pawn = (AFortPlayerPawnAthena*)PC->Pawn;
    if (Pawn)
    {
        static auto EditToolDef = StaticFindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");
        if (EditToolDef && Pawn->CurrentWeapon && Pawn->CurrentWeapon->WeaponData != EditToolDef)
        {
            if (auto EditToolEntry = Inventory::FindItemEntry(PC, EditToolDef))
                PC->ServerExecuteInventoryItem(EditToolEntry->ItemGuid);
        }

        auto EditTool = (AFortWeap_EditingTool*)Pawn->CurrentWeapon;
        if (EditTool) {
            EditTool->EditActor = BuildingActorToEdit;
            EditTool->OnRep_EditActor();
        }
        
        auto PS = (AFortPlayerStateAthena*)PC->PlayerState;
        if (PS) {
            BuildingActorToEdit->EditingPlayer = PS;
            BuildingActorToEdit->OnRep_EditingPlayer();
        }
    }
}

static ABuildingSMActor* (*ReplaceBuildingActorOG)(ABuildingSMActor*, char, UClass*, int, uint8, bool, AController*) = nullptr;

void ServerEditBuildingActorHook(AFortPlayerController* PC, ABuildingSMActor* BuildingActorToEdit, UClass* NewBuildingClass, uint8 RotationIterations, bool bMirrored)
{
    if (!PC || !BuildingActorToEdit || !NewBuildingClass) return;

    if (!ReplaceBuildingActorOG) {
        ReplaceBuildingActorOG = decltype(ReplaceBuildingActorOG)(Hooking::GetOffsetBRUH(0x11252B0));
    }

    if (auto NewBuilding = ReplaceBuildingActorOG(BuildingActorToEdit, 1, NewBuildingClass, BuildingActorToEdit->CurrentBuildingLevel, RotationIterations, bMirrored, PC))
    {
        NewBuilding->bPlayerPlaced = true;

        BuildingActorToEdit->EditingPlayer = nullptr;
        BuildingActorToEdit->OnRep_EditingPlayer();
    }
}

void ServerEndEditingBuildingActorHook(AFortPlayerController* PC, ABuildingSMActor* BuildingActorToStopEditing)
{
    if (!PC || !PC->Pawn || !BuildingActorToStopEditing) return;

    BuildingActorToStopEditing->EditingPlayer = nullptr;
    BuildingActorToStopEditing->OnRep_EditingPlayer();

    AFortWeap_EditingTool* EditTool = (AFortWeap_EditingTool*)((AFortPlayerPawn*)PC->Pawn)->CurrentWeapon;
    if (EditTool)
    {
        EditTool->bEditConfirmed = true;
        EditTool->EditActor = nullptr;
        EditTool->OnRep_EditActor();
    }
}

void ServerClientIsReadyToRespawn(AFortPlayerControllerAthena* PC)
{
    if (!PC) return;
    
    auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
    if (!PlayerState) return;
    
    auto& RespawnData = PlayerState->RespawnData;
    if (RespawnData.bRespawnDataAvailable && RespawnData.bServerIsReady)
    {
        RespawnData.bClientIsReady = true;

        FTransform Transform{};
        Transform.Translation = RespawnData.RespawnLocation;
        Transform.Scale3D = FVector{ 1,1,1 };
        
        auto GameMode = GetGameMode();
        if (GameMode) {
            auto Pawn = (AFortPlayerPawnAthena*)GameMode->SpawnDefaultPawnAtTransform(PC, Transform);
            if (Pawn) {
                PC->Possess(Pawn);
                Pawn->SetMaxHealth(100);
                Pawn->SetHealth(100);
                Pawn->SetMaxShield(100);
                Pawn->SetShield(100);
                PC->RespawnPlayerAfterDeath(true);
            }
        }
    }
}

void (*GetPlayerViewPointOG)(AFortPlayerController* a1, FVector a2, FRotator a3) = nullptr;

void GetPlayerViewPointHook(AFortPlayerController* a1, FVector& a2, FRotator& a3)
{
    if (!a1) {
        if (GetPlayerViewPointOG) GetPlayerViewPointOG(a1, a2, a3);
        return;
    }
    
    if (auto Pawn = a1->Pawn)
    {
        a2 = Pawn->K2_GetActorLocation();
        a3 = a1->GetControlRotation();
        return;
    }

    if (GetPlayerViewPointOG) GetPlayerViewPointOG(a1, a2, a3);
}

DWORD ThreadTEST(LPVOID)
{
    Sleep(5500);
    
    static void (*sub_7FF6B922C400)(__int64, char) = nullptr;
    if (!sub_7FF6B922C400) {
        sub_7FF6B922C400 = decltype(sub_7FF6B922C400)(Hooking::GetOffsetBRUH(0xFBC400));
    }
    
    auto GameMode = GetGameMode();
    if (GameMode && sub_7FF6B922C400) {
        sub_7FF6B922C400(__int64(GameMode), 1);
    }

    auto GameState = GetGameState();
    if (GameState) {
        GameState->SafeZonesStartTime = 0.01f;
        GameState->bAircraftIsLocked = false;
    }
    return 1;
}

void (*EnterAircraft)(AFortPlayerControllerAthena* a1, unsigned __int64 a2) = nullptr;

void EnterAircraftHook(AFortPlayerControllerAthena* a1, unsigned __int64 a2)
{
    if (!a1) {
        if (EnterAircraft) EnterAircraft(a1, a2);
        return;
    }
    
    LOG_("__int64(a1): {}", __int64(a1));
    LOG_("a22: {}", a2);

    static bool aa1WOWRACIST = false;
    if (!aa1WOWRACIST && Globals::bLategame)
    {
        aa1WOWRACIST = true;
        
        auto GameState = GetGameState();
        auto GameMode = GetGameMode();
        
        if (GameState && GameMode) {
            auto Aircraft = GameState->GetAircraft(0);
            LOG_("TESTESTESTE: num safe zones locations {}", GameMode->SafeZoneLocations.Num());

            if (Aircraft) {
                Aircraft->FlightInfo.FlightSpeed = 0.01f;
                FVector Loc = GameMode->SafeZoneLocations[4];
                Loc.Z = 19000;
                Aircraft->FlightInfo.FlightStartLocation = (FVector_NetQuantize100)Loc;

                Aircraft->FlightInfo.TimeTillFlightEnd = 10;
                Aircraft->FlightInfo.TimeTillDropEnd = 10;
                Aircraft->FlightInfo.TimeTillDropStart = 5;
                
                auto World = GetWorld();
                if (World) {
                    auto Statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;
                    if (Statics) {
                        Aircraft->DropStartTime = Statics->GetTimeSeconds(World) + 5;
                        Aircraft->DropEndTime = Statics->GetTimeSeconds(World) + 7;
                    }
                }
                
                GameState->bAircraftIsLocked = true;
            }

            CreateThread(0, 0, ThreadTEST, 0, 0, 0);
        }
    }

    if (a1->WorldInventory)
    {
        std::vector<FGuid> ToDropGoodSir{};
        auto InstancesPtr = &a1->WorldInventory->Inventory.ItemInstances;
        for (int i = 0; i < InstancesPtr->Num(); i++)
        {
            if (InstancesPtr->operator[](i))
            {
                auto ItemDef = (UFortWorldItemDefinition*)InstancesPtr->operator[](i)->ItemEntry.ItemDefinition;
                if (ItemDef && ItemDef->bCanBeDropped)
                {
                    LOG_("REAL 1");
                    ToDropGoodSir.push_back(InstancesPtr->operator[](i)->ItemEntry.ItemGuid);
                }
            }
        }

        for (auto& Guid : ToDropGoodSir)
        {
            Inventory::RemoveItem(a1, Guid);
        }

        Inventory::Update(a1);

        if (Globals::bLategame)
        {
            auto first = GetRandomItem(EFortItemType::WeaponRanged);
            auto second = GetRandomItem(EFortItemType::WeaponRanged);
            auto third = GetRandomItem(EFortItemType::WeaponRanged);
            auto fourth = GetRandomItem(EFortItemType::Consumable);
            auto fifth = GetRandomItem(EFortItemType::Consumable);

            static auto MatReal = UObject::FindObject<UFortItemDefinition>("WoodItemData.WoodItemData");
            static auto MatReal2 = UObject::FindObject<UFortItemDefinition>("MetalItemData.MetalItemData");
            static auto MatReal3 = UObject::FindObject<UFortItemDefinition>("StoneItemData.StoneItemData");

            int WoodCount = 0, MetalCount = 0, StoneCount = 0;
            GetRandomMaterialCount(&WoodCount, &StoneCount, &MetalCount);

            if (first) Inventory::AddItem(a1, first->ItemDefinition, first->DropCount, first->LoadedAmmo);
            if (second) Inventory::AddItem(a1, second->ItemDefinition, second->DropCount, second->LoadedAmmo);
            if (third) Inventory::AddItem(a1, third->ItemDefinition, third->DropCount, third->LoadedAmmo);
            if (fourth) Inventory::AddItem(a1, fourth->ItemDefinition, fourth->DropCount, fourth->LoadedAmmo);
            if (fifth) Inventory::AddItem(a1, fifth->ItemDefinition, fifth->DropCount, fifth->LoadedAmmo);

            if (first) {
                auto AmmoDef = ((UFortWeaponItemDefinition*)first->ItemDefinition)->GetAmmoWorldItemDefinition_BP();
                if (AmmoDef) Inventory::AddItem(a1, AmmoDef, 200);
            }
            if (second) {
                auto AmmoDef = ((UFortWeaponItemDefinition*)second->ItemDefinition)->GetAmmoWorldItemDefinition_BP();
                if (AmmoDef) Inventory::AddItem(a1, AmmoDef, 200);
            }
            if (third) {
                auto AmmoDef = ((UFortWeaponItemDefinition*)third->ItemDefinition)->GetAmmoWorldItemDefinition_BP();
                if (AmmoDef) Inventory::AddItem(a1, AmmoDef, 200);
            }

            if (MatReal) Inventory::AddItem(a1, MatReal, WoodCount);
            if (MatReal3) Inventory::AddItem(a1, MatReal3, StoneCount);
            if (MatReal2) Inventory::AddItem(a1, MatReal2, MetalCount);
        }
    }

    if (EnterAircraft) EnterAircraft(a1, a2);
}

void (*ServerSetTeam)(void*, uint8) = nullptr;

void ServerSetTeamHook(AFortPlayerControllerAthena* PC, uint8 NewTeam)
{
    if (!PC) return;
    
    if (ServerSetTeam) ServerSetTeam(PC, NewTeam);
    
    LOG_("ServerSetTeam CALLED, {}", NewTeam);
    
    if (auto PlayerState = Cast<AFortPlayerStateAthena>(PC->PlayerState))
    {
        PlayerState->OnRep_TeamIndex(PlayerState->TeamIndex);
        PlayerState->SquadId = PlayerState->TeamIndex - 2;
        PlayerState->OnRep_PlayerTeam();
        PlayerState->OnRep_PlayerTeamPrivate();
        PlayerState->OnRep_SquadId();

        LOG_("new squadid: {}", PlayerState->SquadId);

        auto GameState = GetGameState();
        if (GameState) {
            for (int i = 0; i < GameState->GameMemberInfoArray.Members.Num(); i++)
            {
                auto CurrentUniqueId = GameState->GameMemberInfoArray.Members[i].MemberUniqueId;
                if (PlayerState->AreUniqueIDsIdentical(CurrentUniqueId, PlayerState->UniqueId))
                {
                    LOG_("FOUND UNIQUEID");
                    GameState->GameMemberInfoArray.Members.Remove(i);
                    GameState->GameMemberInfoArray.MarkArrayDirty();

                    FGameMemberInfo test{ -1,-1,-1 };
                    test.TeamIndex = PlayerState->TeamIndex;
                    test.SquadId = PlayerState->SquadId;
                    test.MemberUniqueId = PlayerState->UniqueId;

                    GameState->GameMemberInfoArray.Members.Add(test);
                    GameState->GameMemberInfoArray.MarkItemDirty(test);
                    break;
                }
            }
        }
    }
}

static void (*RemoveFromAlivePlayerOG)(void*, void*, void*, void*, void*, EDeathCause, char) = nullptr;
void (*ClientOnPawnDiedOG)(AFortPlayerControllerZone* a1, FFortPlayerDeathReport a2) = nullptr;

void ClientOnPawnDiedHook(AFortPlayerControllerZone* DeadPlayer, FFortPlayerDeathReport& DeathReport)
{
    if (!DeadPlayer) {
        if (ClientOnPawnDiedOG) ClientOnPawnDiedOG(DeadPlayer, DeathReport);
        return;
    }
    
    auto DeadPawn = (AFortPlayerPawnAthena*)DeadPlayer->Pawn;
    auto DeadPlayerState = (AFortPlayerStateAthena*)DeadPlayer->PlayerState;
    auto KillerPlayerState = (AFortPlayerStateAthena*)DeathReport.KillerPlayerState;
    auto KillerPawn = (AFortPlayerPawnAthena*)DeathReport.KillerPawn;

    if (!DeadPawn || !DeadPlayerState) {
        if (ClientOnPawnDiedOG) ClientOnPawnDiedOG(DeadPlayer, DeathReport);
        return;
    }

    EDeathCause DeathCause = DeadPlayerState->ToDeathCause(DeathReport.Tags, DeadPawn->bIsDBNO);
    DeadPlayerState->DeathInfo.bInitialized = true;
    DeadPlayerState->DeathInfo.bDBNO = DeadPawn->bIsDBNO;
    DeadPlayerState->DeathInfo.DeathCause = DeathCause;
    DeadPlayerState->DeathInfo.FinisherOrDowner = KillerPlayerState ? KillerPlayerState : DeadPlayerState;
    DeadPlayerState->DeathInfo.Distance = DeathCause == EDeathCause::FallDamage ? DeadPawn->LastFallDistance : DeadPawn->GetDistanceTo(KillerPawn);
    DeadPlayerState->DeathInfo.DeathLocation = DeadPawn ? DeadPawn->K2_GetActorLocation() : FVector{};

    DeadPlayerState->PawnDeathLocation = DeadPlayerState->DeathInfo.DeathLocation;
    DeadPlayerState->OnRep_DeathInfo();

    if (KillerPlayerState && KillerPlayerState != DeadPlayerState)
    {
        KillerPlayerState->KillScore++;
        KillerPlayerState->TeamKillScore++;
        KillerPlayerState->ClientReportKill(DeadPlayerState);
        KillerPlayerState->OnRep_Kills();
        KillerPlayerState->OnRep_TeamScore();

        KillerPlayerState->Score++;
        KillerPlayerState->TeamScore++;
        KillerPlayerState->OnRep_Score();
    }

    auto GameState = GetGameState();
    auto GameMode = GetGameMode();
    
    if (GameState && !GameState->IsRespawningAllowed(DeadPlayerState))
    {
        if (!DeadPawn->IsDBNO())
        {
            if (DeadPlayer->WorldInventory)
            {
                for (int i = 0; i < DeadPlayer->WorldInventory->Inventory.ItemInstances.Num(); i++)
                {
                    if (DeadPlayer->WorldInventory->Inventory.ItemInstances[i] && 
                        DeadPlayer->WorldInventory->Inventory.ItemInstances[i]->CanBeDropped())
                    {
                        // SpawnPickup not defined yet - skip for now
                        // SpawnPickup(DeadPlayer->WorldInventory->Inventory.ItemInstances[i]->ItemEntry, DeadPawn->K2_GetActorLocation(), EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::PlayerElimination);
                    }
                }
            }

            UFortItemDefinition* WeaponDef = nullptr;
            auto DamageCauser = DeathReport.DamageCauser;
            if (DamageCauser)
            {
                if (auto WEAPON = Cast<AFortWeapon>(DamageCauser))
                {
                    WeaponDef = WEAPON->WeaponData;
                }
            }

            if (!RemoveFromAlivePlayerOG) {
                RemoveFromAlivePlayerOG = decltype(RemoveFromAlivePlayerOG)(Hooking::GetOffsetBRUH(0xFAE8C0));
            }
            
            if (RemoveFromAlivePlayerOG && GameMode) {
                RemoveFromAlivePlayerOG(GameMode, DeadPlayer, KillerPlayerState == DeadPlayerState ? nullptr : KillerPlayerState, KillerPawn, WeaponDef, DeadPlayerState->DeathInfo.DeathCause, 0);
            }
        }
    }

    if (ClientOnPawnDiedOG) ClientOnPawnDiedOG(DeadPlayer, DeathReport);
}

void ServerPlayEmoteItemHook(AFortPlayerControllerAthena* PC, UFortItemDefinition* EmoteAsset)
{
    if (!PC || PC->IsInAircraft()) return;

    if (auto Pawn = (AFortPlayerPawnAthena*)PC->Pawn)
    {
        if (auto DanceItemDefinition = Cast<UAthenaDanceItemDefinition>(EmoteAsset))
        {
            LOG_("EMOTING !!!");
            auto PS = (AFortPlayerStateAthena*)PC->PlayerState;
            if (PS) {
                auto Granted = GrantAbility(PS, UGAB_Emote_Generic_C::StaticClass(), DanceItemDefinition, true);

                Pawn->bMovingEmote = DanceItemDefinition->bMovingEmote;
                Pawn->bMovingEmoteForwardOnly = DanceItemDefinition->bMoveForwardOnly;
                Pawn->EmoteWalkSpeed = DanceItemDefinition->WalkForwardSpeed;

                if (PS->AbilitySystemComponent && Granted) {
                    PS->AbilitySystemComponent->ServerTryActivateAbility(Granted->Handle, Granted->InputPressed, Granted->ActivationInfo.PredictionKeyWhenActivated);
                }
            }
        }
    }
}

void ServerAttemptInventoryDropHook(AFortPlayerController* PC, FGuid& ItemGuid, int32 Count)
{
    if (!PC || !PC->Pawn) return;
    
    if (auto ItemEntry = Inventory::FindItemEntry(PC, ItemGuid))
    {
        if (Count > ItemEntry->Count) return;

        // SpawnPickup not defined yet
        // auto Spawned = SpawnPickup(*ItemEntry, PC->Pawn->K2_GetActorLocation(), EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset);
        // if (Spawned) Spawned->PawnWhoDroppedPickup = (AFortPawn*)PC->Pawn;
        
        Inventory::RemoveItem(PC, ItemEntry->ItemDefinition, Count);
    }
}

void (*ServerAttemptInteractOG)(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, __int64) = nullptr;

void ServerAttemptInteractHook(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, __int64 ssss)
{
    if (!Comp) {
        if (ServerAttemptInteractOG) ServerAttemptInteractOG(Comp, ReceivingActor, InteractComponent, InteractType, ssss);
        return;
    }
    
    auto PC = Cast<AFortPlayerController>(Comp->GetOwner());
    if (PC)
    {
        if (auto Container = Cast<ABuildingContainer>(ReceivingActor))
        {
            auto ClassName = Container->Class->GetName();

            Container->bAlreadySearched = true;
            Container->OnRep_bAlreadySearched();
            Container->SearchBounceData.SearchAnimationCount++;
            Container->BounceContainer();

            FVector Loc = Container->K2_GetActorLocation() + (Container->GetActorRightVector() * 44);
            Loc.Z += 24;

            // Loot logic - simplified
            if (ClassName.contains("Chest"))
            {
                LOG_("Chest opened!");
            }
            if (ClassName.contains("Ammo"))
            {
                LOG_("Ammo box opened!");
            }
        }
    }

    if (ServerAttemptInteractOG) ServerAttemptInteractOG(Comp, ReceivingActor, InteractComponent, InteractType, ssss);
}

void (*ExitAircraft)(AFortPlayerControllerAthena* a1) = nullptr;

void ExitAircraftHook(AFortPlayerControllerAthena* a1)
{
    if (!a1) {
        if (ExitAircraft) ExitAircraft(a1);
        return;
    }
    
    if (ExitAircraft) ExitAircraft(a1);

    if (Globals::bLategame)
    {
        LOG_("LATEGAME EXIT AIRCRAFT");
        
        auto GameMode = GetGameMode();
        if (GameMode && a1->Pawn) {
            FVector Loc = GameMode->SafeZoneLocations[4];
            Loc.Z = 19000;
            FRotator Rot = a1->GetControlRotation();
            
            LOG_("VALID PAWN");
            a1->Pawn->K2_TeleportTo(Loc, Rot);
            ((AFortPawn*)a1->Pawn)->SetShield(100);
        }
    }
}

void InitHoksPC()
{
    auto PlayerControllerDef = GetDefObj<AAthena_PlayerController_C>();
    auto InteractionDef = GetDefObj<UFortControllerComponent_Interaction>();
    
    if (PlayerControllerDef) {
        Hooking::VirtualHook(PlayerControllerDef, 0x108, ServerAcknowledgePossessionHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x1FD, ServerExecuteInventoryItem);
        Hooking::VirtualHook(PlayerControllerDef, 0x223, ServerCreateBuildingActorHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x22A, ServerBeginEditingBuildingActorHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x225, ServerEditBuildingActorHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x228, ServerEndEditingBuildingActorHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x1BC, ServerPlayEmoteItemHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x25F, ServerReadyToStartMatchHook, (void**)&ServerReadyToStartMatchOG);
        Hooking::VirtualHook(PlayerControllerDef, 0x458, ServerClientIsReadyToRespawn);
        Hooking::VirtualHook(PlayerControllerDef, 0x210, ServerAttemptInventoryDropHook);
        Hooking::VirtualHook(PlayerControllerDef, 0x420, ServerSetTeamHook, (void**)&ServerSetTeam);
    }
    
    if (InteractionDef) {
        Hooking::VirtualHook(InteractionDef, 0x80, ServerAttemptInteractHook, (void**)&ServerAttemptInteractOG);
    }

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x1003280), ExitAircraftHook, (void**)&ExitAircraft);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x1003280));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x10020E0), EnterAircraftHook, (void**)&EnterAircraft);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x10020E0));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x16E2230), GetPlayerViewPointHook, (void**)&GetPlayerViewPointOG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x16E2230));

    MH_CreateHook((LPVOID)Hooking::GetOffsetBRUH(0x1AEC9A0), ClientOnPawnDiedHook, (void**)&ClientOnPawnDiedOG);
    MH_EnableHook((LPVOID)Hooking::GetOffsetBRUH(0x1AEC9A0));
    
    LOG_("PC Hooks initialized successfully!");
}
