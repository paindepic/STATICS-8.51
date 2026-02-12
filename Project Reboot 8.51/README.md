# Project Reboot 8.51 - STATICS Edition

A custom Fortnite 8.51 game server implementation combining the Volcano 8.51 base with improved Spectra 10.40 bot AI.

## Overview

This project is a DLL injection-based private server for Fortnite Chapter 1 Season 8 (Version 8.51). It allows you to run a local game server with enhanced features including:

- 🤖 Advanced AI bot system (from Spectra 10.40)
- 🏗️ Full building and editing mechanics
- 👥 Team-based gameplay
- 🌳 Resource harvesting/farming
- 📦 Looting system with auto-pickup
- 🚗 Vehicle support
- ⚡ Abilities system
- 🎯 Quest manager
- 💀 Respawning system
- 🕺 Emote system
- 🌐 Optional backend integration

## System Requirements

- Windows 10/11 (x64)
- Visual Studio 2022 (v143 toolset)
- C++20 compiler support
- Fortnite Chapter 1 Season 8 (Version 8.51) client files

## Building

1. Clone or download this repository
2. Open `Project Reboot 8.51.sln` in Visual Studio 2022
3. Select your configuration:
   - **Release** (recommended for production use)
   - **Debug** (for development/debugging)
4. Ensure platform is set to **x64**
5. Build the solution (Ctrl+Shift+B)
6. The output DLL will be in `x64/Release/` or `x64/Debug/`

## Installation

1. Build the project following the steps above
2. Inject `8.51.dll` into `FortniteClient-Win64-Shipping.exe` using your preferred DLL injector
3. The game will automatically initialize the server components

## Configuration

Edit the `Globals` namespace in `Globals.h` to configure server settings:

```cpp
namespace Globals {
    bool bBotsEnabled = true;        // Enable/disable bots
    int MaxBotsToSpawn = 100;        // Maximum number of bots
    int MinPlayersForEarlyStart = 95; // Min players to start early
    bool bCreativeEnabled = false;   // Enable creative mode
    bool bSTWEnabled = false;        // Enable Save the World
    bool bEventEnabled = false;      // Enable event mode
}
```

## Network Settings

The server listens on **port 7777** by default. You can modify this in `HelperFunctions.h`:

```cpp
NewURL.Port = 7777;
```

## Features

### Bot System
- State-based AI behaviors (warmup, looting, combat, healing, building)
- Advanced navigation and pathfinding
- Team coordination
- Weapon selection and switching
- Building strategies

### Building System
- Place walls, floors, stairs, roofs
- Edit structures
- Building materials (wood, stone, metal)
- Free build mode (optional)

### Inventory System
- Full item management
- Weapon switching
- Ammo tracking
- Resource collection
- Inventory limits and stacking

### Combat System
- Weapon firing mechanics
- Hit registration
- Damage calculation
- Shield/health management
- Respawning on death

## Hardcoded Offsets (Fortnite 8.51)

**Important:** All offsets are specifically for Fortnite Version 8.51. Using this with other versions will not work.

Key offsets:
- GObjects: `0x7A5B0A0`
- GIsClient/GIsServer: `0x637925B` / `0x637925C`
- NetDriver functions: Various (see `HelperFunctions.h`)

## Troubleshooting

### "Failed to initialize GObjects"
- Verify you're using the correct Fortnite version (8.51)
- Check that the GObjects offset (`0x7A5B0A0`) is correct for your client

### "Network listener failed"
- Ensure port 7777 is not in use
- Check Windows Firewall settings
- Run as Administrator if needed

### Bots not spawning
- Check `Globals::bBotsEnabled = true`
- Verify bot manager initialized successfully (check console output)
- Ensure `MaxBotsToSpawn` is set appropriately

### Build errors
- Ensure you're using Visual Studio 2022
- Check that C++20 language standard is enabled
- Verify all SDK files are present in `SDK/` folder

## Project Structure

```
Project Reboot 8.51/
├── 8.51/                      # Main source directory
│   ├── dllmain.cpp           # DLL entry point and initialization
│   ├── framework.h            # Main header with includes
│   ├── Globals.h             # Global configuration
│   ├── HelperFunctions.h     # Utility functions
│   ├── HookImplementations.h # Hook function implementations
│   ├── MissingFunctions.h    # Forward declarations
│   │
│   ├── GameMode.h            # Game mode hooks and logic
│   ├── Controller.h          # Player controller hooks
│   ├── Pawn.h                # Pawn-related hooks
│   ├── PC.h                  # Player controller specifics
│   │
│   ├── Inventory.h           # Inventory management
│   ├── FortInventory.h       # Fort-specific inventory
│   ├── Looting.h             # Looting system
│   │
│   ├── BuildingActor.h       # Building placement
│   ├── BuildingContainer.h   # Loot containers
│   ├── Farming.h             # Resource harvesting
│   │
│   ├── FortWeapon.h          # Weapon mechanics
│   ├── FortPickup.h          # Item pickups
│   │
│   ├── Bots.h                # Bot logic
│   ├── PlayerBots.h          # Advanced player bot AI
│   ├── ServerBotManager.h    # Bot manager
│   ├── AIController.h        # AI controller hooks
│   ├── BotNames.h            # Bot name generation
│   │
│   ├── Teams.h               # Team system
│   ├── Vehicles.h            # Vehicle support
│   ├── Abilities.h           # Ability system
│   ├── AbilitySystemComponent.h
│   │
│   ├── QuestManager.h        # Quest system
│   ├── backend.h             # Backend integration
│   ├── gaymode.h             # Game mode extensions
│   │
│   ├── Net.h                 # Networking
│   ├── NetDriver.h           # Network driver hooks
│   ├── Misc.h                # Miscellaneous hooks
│   ├── PE.h                  # Process/executable hooks
│   │
│   ├── SDK/                  # Unreal Engine 4 SDK
│   │   ├── UE4.h
│   │   └── Fortnite.h
│   │
│   └── minhook/              # MinHook library
│       └── minhook.h
│
├── Project Reboot 8.51.sln   # Visual Studio solution
├── 8.51/8.51.vcxproj         # Project configuration
├── README.md                 # This file
└── FIXES_SUMMARY.md          # Summary of fixes applied

```

## Credits

- **Volcano 8.51** - Base server implementation
- **Spectra 10.40** - Advanced bot AI system
- **MinHook** - Function hooking library
- **STATICS** - Integration and improvements

## Legal Disclaimer

This project is for educational and research purposes only. Fortnite is a registered trademark of Epic Games, Inc. This project is not affiliated with, endorsed by, or sponsored by Epic Games, Inc.

Use of this software may violate the Fortnite End User License Agreement (EULA) and Terms of Service. Use at your own risk.

## License

This project is provided as-is with no warranty. See individual source files for specific license information.

## Contributing

This is a fixed/improved version of existing projects. If you find bugs or have improvements:

1. Test thoroughly with Fortnite 8.51
2. Document any offset changes
3. Provide clear descriptions of changes
4. Ensure code follows existing style

## Support

For issues, questions, or discussions, please refer to the FIXES_SUMMARY.md file which documents all the fixes applied to this version.
