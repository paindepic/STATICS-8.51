#pragma once
#include "framework.h"

// ============================================
// COMPATIBILITY FUNCTIONS
// ============================================

inline void HOKSREAL() {
    // Placeholder for Volcano compatibility
    LOG_("HOKSREAL initialized - compatibility layer active");
}

// ============================================
// NETWORK & ENGINE HOOKS
// ============================================

inline bool TickFlushHook(void* a1) {
    if (TickFlushOG) {
        return ((bool(*)(void*))TickFlushOG)(a1);
    }
    return true;
}

inline void* TickFlushOG = nullptr;

inline void* DispatchReqHook(void* a1, void* a2) {
    if (DispatchReqOG) {
        return ((void*(*)(void*, void*))DispatchReqOG)(a1, a2);
    }
    return nullptr;
}

inline void* DispatchReqOG = nullptr;

inline void KickPlayer(void* a1) {
    LOG_("KickPlayer blocked");
}

inline void ChangeGameSessionId(void* a1) {
    LOG_("ChangeGameSessionId called");
}

inline void ValFailure1(void* a1) {
    LOG_("ValFailure1 validation bypassed");
}

inline void NoReserve(void* a1) {
    LOG_("NoReserve called");
}

inline int UWorldGetNetMode(void* a1) {
    return 3; // NM_DedicatedServer
}

inline int AActorGetNetMode(void* a1) {
    return 3; // NM_DedicatedServer
}

inline void NoMcp(void* a1) {
    LOG_("NoMcp called");
}

inline void PickTeamHook(void* a1) {
    LOG_("PickTeamHook called");
}

inline void SetMegaStormStuffHOOK(void* a1) {
    if (SetMegaStormStuffidkREALOG) {
        ((void(*)(void*))SetMegaStormStuffidkREALOG)(a1);
    }
}

inline void* SetMegaStormStuffidkREALOG = nullptr;

// ============================================
// WEAPON & INVENTORY FUNCTIONS
// ============================================

inline void* sub_7FF6B9B17A60_OG = nullptr;

inline void sub_7FF6B9B17A60(AFortWeapon* a1, unsigned int a2) {
    if (!a1 || !a1->WeaponData) {
        if (sub_7FF6B9B17A60_OG) {
            ((void(*)(AFortWeapon*, unsigned int))sub_7FF6B9B17A60_OG)(a1, a2);
        }
        return;
    }

    auto Pawn = Cast<AFortPawn>(a1->GetOwner());
    if (!Pawn) {
        if (sub_7FF6B9B17A60_OG) {
            ((void(*)(AFortWeapon*, unsigned int))sub_7FF6B9B17A60_OG)(a1, a2);
        }
        return;
    }

    auto PC = Cast<AFortPlayerController>(Pawn->Controller);
    if (!PC || !PC->WorldInventory) {
        if (sub_7FF6B9B17A60_OG) {
            ((void(*)(AFortWeapon*, unsigned int))sub_7FF6B9B17A60_OG)(a1, a2);
        }
        return;
    }

    // Remove ammo from inventory
    auto AmmoDef = a1->WeaponData->GetAmmoWorldItemDefinition_BP();
    if (AmmoDef) {
        for (int j = 0; j < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); j++) {
            if (PC->WorldInventory->Inventory.ReplicatedEntries[j].ItemDefinition == AmmoDef) {
                int NewCount = PC->WorldInventory->Inventory.ReplicatedEntries[j].Count - a2;
                if (NewCount <= 0) {
                    PC->WorldInventory->Inventory.ReplicatedEntries.Remove(j);
                } else {
                    PC->WorldInventory->Inventory.ReplicatedEntries[j].Count = NewCount;
                }
                break;
            }
        }
    }

    // Update loaded ammo in item entry
    for (int i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++) {
        if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == a1->WeaponData) {
            int NewAmmo = PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo - a2;
            PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo = NewAmmo < 0 ? 0 : NewAmmo;
            break;
        }
    }
    
    PC->WorldInventory->HandleInventoryLocalUpdate();

    if (sub_7FF6B9B17A60_OG) {
        ((void(*)(AFortWeapon*, unsigned int))sub_7FF6B9B17A60_OG)(a1, a2);
    }
}

inline void* K2_CommitExecute = nullptr;

inline void K2_CommitExecuteHook(UFortGameplayAbility* a1) {
    if (!a1) {
        if (K2_CommitExecute) {
            ((void(*)(UFortGameplayAbility*))K2_CommitExecute)(a1);
        }
        return;
    }

    if (a1->IsA(UGA_Ranged_GenericDamage_C::StaticClass())) {
        auto Pawn = a1->GetActivatingPawn();
        if (Pawn && Pawn->CurrentWeapon) {
            auto PC = Cast<AFortPlayerController>(Pawn->Controller);
            if (PC && PC->WorldInventory) {
                for (int i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++) {
                    if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Pawn->CurrentWeapon->ItemEntryGuid) {
                        PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo = Pawn->CurrentWeapon->AmmoCount;
                        PC->WorldInventory->HandleInventoryLocalUpdate();
                        break;
                    }
                }
            }
        }
    }

    if (K2_CommitExecute) {
        ((void(*)(UFortGameplayAbility*))K2_CommitExecute)(a1);
    }
}

// ============================================
// OTHER FUNCTIONS
// ============================================

inline int GetMaxTickRate(void* a1) {
    return 120;
}

inline void CollectGarbage(void* a1) {
    LOG_("CollectGarbage bypassed");
}

inline void CanActivateAbility(void* a1) {
    LOG_("CanActivateAbility called");
}
