#pragma once

// Forward declarations for functions referenced in dllmain.cpp but defined elsewhere

// From gaymode.h
void HOKSREAL();

// From PC.h
void InitHoksPC();

// From Abilities.h
void InitAbilities();

// From Farming.h
void InitFarming();

// From Pawn.h
void InitPawnHooks();

// From Vehicles.h
void InitVehicleHooks();

// Hook function declarations that are defined inline in dllmain.cpp
// These need to be declared before being used

// TickFlush hook (0x2D39300)
typedef bool (*TickFlushFn)(void*);
extern TickFlushFn TickFlushOG;
bool TickFlushHook(void* a1);

// DispatchRequest hook (0xCF2E80)
typedef void* (*DispatchReqFn)(void*, void*);
extern DispatchReqFn DispatchReqOG;
void* DispatchReqHook(void* a1, void* a2);

// Other hook functions
void KickPlayer(void* a1);
void ChangeGameSessionId(void* a1);
void ValFailure1(void* a1);
void NoReserve(void* a1);
int UWorldGetNetMode(void* a1);
int AActorGetNetMode(void* a1);
void NoMcp(void* a1);

// K2_CommitExecute hook (0x8C1F70) - declared in Inventory.h
// void K2_CommitExecuteHook(UFortGameplayAbility* a1);
// extern void (*K2_CommitExecute)(UFortGameplayAbility*);

// PickTeam hook (0xFA9B20)
void PickTeamHook(void* a1);

// SetMegaStormStuff hook (0xFB9830)
typedef void (*SetMegaStormStuffFn)(void*);
extern SetMegaStormStuffFn SetMegaStormStuffidkREALOG;
void SetMegaStormStuffHOOK(void* a1);

// sub_7FF6B9B17A60 hook (0x18A7A60) - declared in Inventory.h
// extern void (*sub_7FF6B9B17A60_OG)(AFortWeapon* a1, unsigned int a2);
// void sub_7FF6B9B17A60(AFortWeapon* a1, unsigned int a2);

// CanActivateAbility hook (0x830630)
void CanActivateAbility(void* a1);

// CollectGarbage hook (0x1E054E0)
void CollectGarbage(void* a1);

// GetMaxTickRate virtual hook
int GetMaxTickRate(void* a1);
