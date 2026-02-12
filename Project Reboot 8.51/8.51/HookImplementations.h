#pragma once

// Implementation of hooks that are referenced in dllmain.cpp

// TickFlush hook (0x2D39300)
TickFlushFn TickFlushOG = nullptr;
bool TickFlushHook(void* a1) {
    // Call original if exists
    if (TickFlushOG) {
        return TickFlushOG(a1);
    }
    return true;
}

// DispatchRequest hook (0xCF2E80)
DispatchReqFn DispatchReqOG = nullptr;
void* DispatchReqHook(void* a1, void* a2) {
    // Call original if exists
    if (DispatchReqOG) {
        return DispatchReqOG(a1, a2);
    }
    return nullptr;
}

// KickPlayer hook (0x2C03D20) - prevent kicking players
void KickPlayer(void* a1) {
    Log("KickPlayer called - blocked!");
    // Don't actually kick the player
}

// ChangeGameSessionId hook (0x12E7410)
void ChangeGameSessionId(void* a1) {
    Log("ChangeGameSessionId called - blocked!");
    // Don't change session ID
}

// ValFailure1 hook (0x15392D0) - validation bypass
void ValFailure1(void* a1) {
    Log("ValFailure1 called - bypassed!");
    // Bypass validation failure
}

// NoReserve hook (0xBCFB60) - disable player reservation
void NoReserve(void* a1) {
    Log("NoReserve called - bypassed!");
    // Don't reserve slots
}

// UWorldGetNetMode hook (0x30100A0) - force server mode
int UWorldGetNetMode(void* a1) {
    return 3; // NM_DedicatedServer
}

// AActorGetNetMode hook (0x29A40F0) - force server mode
int AActorGetNetMode(void* a1) {
    return 3; // NM_DedicatedServer
}

// NoMcp hook (0x13876D0) - disable MCP (matchmaking/online services)
void NoMcp(void* a1) {
    Log("NoMcp called - MCP disabled!");
    // Don't connect to MCP services
}

// PickTeam hook (0xFA9B20)
void PickTeamHook(void* a1) {
    // Team picking is handled by our custom logic
    Log("PickTeamHook called");
}

// SetMegaStormStuff hook (0xFB9830)
SetMegaStormStuffFn SetMegaStormStuffidkREALOG = nullptr;
void SetMegaStormStuffHOOK(void* a1) {
    // Call original if exists
    if (SetMegaStormStuffidkREALOG) {
        SetMegaStormStuffidkREALOG(a1);
    }
}

// CanActivateAbility hook (0x830630) - allow all abilities
void CanActivateAbility(void* a1) {
    // Always allow ability activation
}

// CollectGarbage hook (0x1E054E0) - disable garbage collection
void CollectGarbage(void* a1) {
    // Don't collect garbage during gameplay
    Log("CollectGarbage called - disabled during gameplay!");
}

// GetMaxTickRate virtual hook - set to 120Hz
int GetMaxTickRate(void* a1) {
    return 120; // 120 ticks per second
}
