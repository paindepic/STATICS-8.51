# Project Reboot 8.51 - STATICS 8.51

A complete gameserver project combining the best features from **Volcano 8.51** and **Spectra 10.40** with enhanced Player AIs.

## Features

### From Volcano 8.51:
- **Farming System** - Complete farming mechanics
- **Building/Editing** - Full building functionality
- **Proper Teams** - Team management system
- **Different LTMs Support** - Multiple game mode support
- **Respawning** - Player respawn system
- **Proper Hooking** - Stable hooking infrastructure
- **Vehicles** - Vehicle support
- **Lategame** - Endgame mechanics
- **Emoting** - Emote system
- **Looting** - Loot system (improved with Spectra AI)
- **AutoPickup** - Automatic item pickup
- **Change Teams in Playground** - Team switching
- **MCP** - Matchmaking and party system

### From Spectra 10.40 (Enhanced AI):
- **Advanced Player Bots** - Sophisticated AI with multiple behaviors
- **Bot Warmup Choices** - Bots can emote, approach players, fight, build, etc.
- **Bot State Machine** - Comprehensive AI states (Warmup, Bus, Skydiving, Looting, Combat, Healing)
- **Bot Looting System** - Intelligent loot selection and inventory management
- **Bot Combat System** - Advanced combat with strafing, weapon selection, and targeting
- **Bot Healing System** - Smart healing decisions based on health/shield status
- **Bot Building System** - AI building capabilities
- **Bot Navigation** - Pathfinding and movement logic
- **Bot Decision Making** - Dynamic decision-making based on game state

### New in Project Reboot 8.51:
- **Combined Architecture** - Best of both projects in one codebase
- **Enhanced Stability** - Fixed compilation errors and memory issues
- **Improved AI Integration** - Spectra's AI seamlessly integrated with Volcano's gameplay systems
- **Unified Hooking System** - Consistent hooking approach across all modules
- **STATICS 8.51 Edition** - Optimized for stability and performance

## Technical Details

- **Language**: C++ (MSVC)
- **Build System**: Visual Studio 2022 (v143 toolset)
- **Platform**: Windows DLL
- **Hooking Library**: MinHook
- **Target Engine**: Unreal Engine (Fortnite 8.51)
- **Architecture**: x64

## Building

1. Open `Project Reboot 8.51.sln` in Visual Studio 2022
2. Select `Release x64` configuration
3. Build the solution
4. The output DLL will be in `8.51/Release/`

## Configuration

Edit the global settings in `framework.h`:

```cpp
namespace Globals {
    inline bool bCreativeEnabled = false;
    inline bool bSTWEnabled = false;
    inline bool bDebugMode = false;
    inline bool bBotsEnabled = true;
    inline int MaxBots = 50;
    inline bool bTeamsEnabled = true;
    inline bool bFarmingEnabled = true;
    inline bool bBuildingEnabled = true;
    inline bool bVehiclesEnabled = true;
    inline bool bLootingEnabled = true;
    inline bool bAbilitiesEnabled = true;
    inline bool bBackendEnabled = false;
    inline std::string BackendURL = "http://localhost:3000";
}
```

## Bot AI Features

The enhanced AI system includes:

- **Multiple Warmup Behaviors**: Emoting, approaching players, fighting, building
- **Advanced Looting**: Priority-based item selection, inventory management
- **Combat Tactics**: Strafing, cover usage, weapon switching
- **Healing Logic**: Smart healing item usage based on health/shield status
- **Navigation**: Pathfinding to objectives, safe zone movement
- **State Machine**: Context-aware behavior switching
- **Team Awareness**: Friendly fire prevention, team coordination

## Credits

- **Volcano 8.51**: Original base project
- **Spectra 10.40**: Advanced AI systems
- **Project Reboot Team**: Integration and enhancements

## License

This project is for educational purposes only. Use at your own risk.

## Support

For issues or questions, please refer to the original project documentation or open an issue.

---

**STATICS 8.51 - The ultimate Fortnite gameserver solution!**