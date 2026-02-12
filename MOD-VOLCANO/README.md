# MOD-VOLCANO for Project Reboot 3.0

## Overview
MOD-VOLCANO is a modification for Fortnite 8.51 (Chapter 1 Season 8) private servers, combining the VOLCANO 8.51 base with SPECTRA bots (AIS and Player Bot systems).

## Features
- **Base**: VOLCANO 8.51 SDK (Fortnite 8.51)
- **Bot System**: SPECTRA AIS Bots & Player Bots adapted for 8.51
- **AI Bots**: Native behavior tree-based AI bots
- **Player Bots**: Custom bot implementation with advanced behaviors
- **Building Support**: Full building system integration
- **Looting System**: Intelligent loot collection
- **Combat System**: Bot combat with weapons and abilities

## Requirements
- **Visual Studio 2022** (v143 toolset)
- **Windows SDK 10.0**
- **C++17 Standard**
- **x64 Platform**

## Project Structure
```
MOD-VOLCANO/
├── MOD-VOLCANO.sln              # Visual Studio solution file
├── MOD-VOLCANO.vcxproj          # Project file (VS2022)
├── MOD-VOLCANO.vcxproj.filters  # Project filters
├── README.md                     # This file
├── .gitignore                   # Git ignore file
├── src/
│   ├── SDK/                     # Fortnite 8.51 SDK
│   │   ├── SDK.hpp              # Main SDK header
│   │   ├── ue.h                 # UE4 utilities
│   │   └── SDK/                 # SDK classes and functions
│   ├── Core/                    # Core system files
│   │   ├── framework.h          # Main framework
│   │   ├── globals.h            # Global variables
│   │   └── dllmain.cpp          # DLL entry point
│   ├── Bots/                    # Bot system
│   │   ├── BotNames.h           # Bot name generator
│   │   ├── ServerBotManager.h   # Bot management
│   │   ├── FortInventory.h       # Inventory handling
│   │   ├── BuildingContainer.h  # Building system
│   │   └── FortPickup.h          # Pickup system
│   ├── Bots/AIS/                # AI Bot system (native)
│   │   ├── AIController.h       # AI Controller
│   │   └── Bots.h               # AI Bot implementation
│   └── Bots/PlayerBot/          # Player Bot system (custom)
│       └── PlayerBots.h         # Player Bot implementation
└── deps/                        # Dependencies
    ├── minhook/                 # MinHook library
    └── inclueds/                # Includes and libraries
        └── curl/                # cURL library
```

## Building Instructions

### Step 1: Open Project
1. Open `MOD-VOLCANO.sln` in Visual Studio 2022
2. Select configuration: **Release | x64** or **Debug | x64**

### Step 2: Build
1. Right-click on the project in Solution Explorer
2. Select "Build"
3. The output DLL will be: `x64/Release/MOD-VOLCANO.dll` or `x64/Debug/MOD-VOLCANO.dll`

### Step 3: Configuration
Edit `src/Core/globals.h` to configure:
- `PlaylistName`: Set your desired playlist
- `MaxBots`: Maximum number of bots (default: 99)
- `bEnableBots`: Enable/disable bots (default: true)

## Bot System Details

### AI Bots (Native)
- Uses Fortnite's native behavior tree system
- Controlled by AIController
- Automatic pathfinding and navigation
- Full weapon and ability support

### Player Bots (Custom)
- Custom implementation with advanced AI
- States: Warmup, PreBus, Bus, Skydiving, Gliding, Landed, Looting, Combat, Healing
- Features:
  - Intelligent looting with filtering
  - Building and editing
  - Combat with strafing
  - Healing management
  - Safe zone awareness

## Known Compatibility Notes

### SDK Version Differences
- This project uses **Fortnite 8.51 SDK** from VOLCANO
- Original SPECTRA bots were for **Fortnite 10.40**
- Bot code has been adapted for 8.51 compatibility
- Some features may need adjustment based on 8.51 SDK structure

### Include Paths
All bot files use relative includes:
```cpp
#include "framework.h"           // Main framework
#include "SDK/SDK.hpp"           // Fortnite SDK
#include "globals.h"             // Global configuration
```

## Troubleshooting

### Build Errors
- Ensure all dependencies are in `deps/` folder
- Check that include paths are correct in project settings
- Verify VS2022 and Windows SDK are properly installed

### Runtime Issues
- Check `FortniteLogs.log` for detailed logging
- Verify bot configuration in `globals.h`
- Ensure all asset paths are correct for 8.51

## Credits
- **Base Project**: VOLCANO 8.51
- **Bot System**: SPECTRA (adapted for 8.51)
- **Integration**: MOD-VOLCANO Team

## License
This project is for educational purposes only. Please respect Epic Games' terms of service.

## Version
- **MOD-VOLCANO V2** - Initial Release
- Target: Fortnite 8.51 (Chapter 1 Season 8)
- Platform: Windows x64
- Compiler: MSVC v143 (VS2022)
