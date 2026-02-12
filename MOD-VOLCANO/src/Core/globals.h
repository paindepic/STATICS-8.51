#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <intrin.h>
#include <sstream>
#include <array>
#include <tlhelp32.h>
#include <future>
#include <set>
#include <variant>
#include <unordered_set>
#include <iomanip>

#include "ue.h"

namespace Globals
{
    std::string PlaylistName = "Playlist_DefaultSolo.Playlist_DefaultSolo";
    bool bLategame = true;
    
    // Bot configuration
    int MaxBots = 99;
    bool bEnableBots = true;
    
    // Building items
    class UFortBuildingItemDefinition* Floor = nullptr;
    class UFortBuildingItemDefinition* Roof = nullptr;
    class UFortBuildingItemDefinition* Stair = nullptr;
    class UFortBuildingItemDefinition* Wall = nullptr;
    class UFortEditToolItemDefinition* EditTool = nullptr;
    
    // Character items
    std::vector<class UAthenaCharacterItemDefinition*> Characters{};
    std::vector<class UAthenaPickaxeItemDefinition*> Pickaxes{};
    std::vector<class UAthenaBackpackItemDefinition*> Backpacks{};
    std::vector<class UAthenaGliderItemDefinition*> Gliders{};
    std::vector<class UAthenaSkyDiveContrailItemDefinition*> Contrails{};
    std::vector<class UAthenaDanceItemDefinition*> Dances{};
    
    // Bot display names
    std::vector<std::string> BotDisplayNames = {
        "Ninja", "DrLupo", "Timthetatman", "SypherPK", "Tfue",
        "NICKMERCS", "CouRageJD", "Hamlinz", "HighDistortion",
        "Valkyrae", "Loserfruit", "Pokimane", "Symfuhny"
    };
}

template<typename T>
T* Cast(UObject* Object, bool bForceCheck = true)
{
    if (Object)
    {
        if (bForceCheck)
        {
            return Object->IsA(T::StaticClass()) ? (T*)Object : nullptr;
        }
        else
        {
            return (T*)Object;
        }
    }
    return nullptr;
}
