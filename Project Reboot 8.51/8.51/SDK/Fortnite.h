#pragma once

// Fortnite-specific SDK includes
#include "SDK.hpp"

// Forward declarations for commonly used Fortnite classes
namespace SDK {
    class AFortGameModeAthena;
    class AFortGameStateAthena;
    class AFortPlayerControllerAthena;
    class AFortPlayerStateAthena;
    class AFortPlayerPawnAthena;
    class AFortAthenaAIBotController;
    class UFortServerBotManagerAthena;
    class AFortAthenaMutator_Bots;
}

// Helper to get current game mode
inline SDK::AFortGameModeAthena* GetGameMode();

// Helper to get current game state
inline SDK::AFortGameStateAthena* GetGameState();
