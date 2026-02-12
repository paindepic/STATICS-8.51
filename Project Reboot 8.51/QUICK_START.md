# Project Reboot 8.51 - Quick Start Guide

## What Was Fixed

This version fixes several critical compilation errors in the original Project Reboot 8.51:

1. ✅ **`.vcxproj` duplicate file error** - Removed framework.h from ClCompile section
2. ✅ **Missing helper functions** - Added GetGameMode(), GetGameState(), SpawnActor(), etc.
3. ✅ **Missing hook implementations** - Implemented all referenced hook functions
4. ✅ **Include order issues** - Reorganized framework.h for proper compilation
5. ✅ **Missing Inventory.h** - Integrated into build system
6. ✅ **Declaration conflicts** - Resolved duplicate declarations

See `FIXES_SUMMARY.md` for detailed information about all fixes.

## Quick Build Steps

### Prerequisites
- Windows 10/11 x64
- Visual Studio 2022 with C++ workload
- Fortnite 8.51 client

### Build
1. Open `Project Reboot 8.51.sln`
2. Select **Release | x64**
3. Press **Ctrl+Shift+B** to build
4. Output: `x64/Release/8.51.dll`

### Run
1. Start Fortnite 8.51
2. Inject `8.51.dll` using any DLL injector
3. A console window will appear showing initialization
4. Wait for "All systems operational" message

## Default Configuration

```cpp
Port: 7777
Bots: Enabled (max 100)
Teams: Enabled
Building: Enabled
Farming: Enabled  
Vehicles: Enabled
Looting: Enabled
Abilities: Enabled
Backend: Disabled
```

## Testing Checklist

After building, verify:

- [ ] Solution opens without errors
- [ ] Build completes successfully (0 errors)
- [ ] Console appears after injection
- [ ] "Project Reboot 8.51 initialized successfully!" message appears
- [ ] Can join game and spawn
- [ ] Building/editing works
- [ ] Weapons fire correctly
- [ ] Bots spawn (if enabled)

## Common Issues

### Build Errors
**Problem:** Compilation fails with missing types  
**Solution:** Ensure SDK files are present in `8.51/SDK/` folder

**Problem:** C++20 errors  
**Solution:** Check project properties → C/C++ → Language → Language Standard = C++20

### Runtime Errors  
**Problem:** "Failed to initialize GObjects"  
**Solution:** Wrong Fortnite version - must be exactly 8.51

**Problem:** DLL injection fails  
**Solution:** Run as Administrator, disable antivirus temporarily

**Problem:** Bots don't spawn  
**Solution:** Check console for bot manager initialization message

## Console Output

Successful initialization should show:

```
[Project Reboot 8.51] Project Reboot 8.51 - STATICS 8.51 Edition
[Project Reboot 8.51] Combining Volcano 8.51 with Spectra 10.40 AI improvements
[Project Reboot 8.51] Loading World!
[Project Reboot 8.51] MinHook Initialized!
[Project Reboot 8.51] Hooked PE!
[Project Reboot 8.51] Hooked Misc!
[Project Reboot 8.51] Hooked GameMode!
[Project Reboot 8.51] Hooked Controller!
[Project Reboot 8.51] Hooked AbilitySystemComponent!
[Project Reboot 8.51] Hooked Pawn!
[Project Reboot 8.51] Hooked FortWeapon!
[Project Reboot 8.51] Hooked FortPickup!
[Project Reboot 8.51] Hooked BuildingActor!
[Project Reboot 8.51] Hooked BuildingContainer!
[Project Reboot 8.51] Hooked Net!
[Project Reboot 8.51] Hooked NetDriver!
[Project Reboot 8.51] Hooked AIController!
[Project Reboot 8.51] All hooks enabled!
[Project Reboot 8.51] Bot manager initialized!
[Project Reboot 8.51] Project Reboot 8.51 initialized successfully!
[Project Reboot 8.51] STATICS 8.51 edition - All systems operational
```

## Key Features to Test

### Building System
1. Press Q (wall), E (floor), etc.
2. Place structure
3. Edit with G (default)
4. Confirm edit
5. Structures should remain after edit

### Bot System
1. Wait for bots to spawn
2. Bots should:
   - Land from bus
   - Loot items
   - Build when shot at
   - Fight back
   - Use abilities

### Inventory System
1. Pick up weapons
2. Switch between slots (1-5)
3. Check ammo consumption
4. Drop items (X key typically)
5. Inventory should sync correctly

### Combat System
1. Fire weapons
2. Hit enemies
3. Check damage numbers
4. Shield/health should decrease
5. Respawn on death (if enabled)

## Performance Tips

- Reduce `MaxBotsToSpawn` if performance issues
- Use Release build for better performance
- Close unnecessary applications
- Ensure adequate RAM (8GB+ recommended)

## Advanced Configuration

Edit `Globals.h` before building to customize:

```cpp
// Bot settings
inline static int MaxBotsToSpawn = 50;  // Lower for better perf
inline static int MinPlayersForEarlyStart = 25;

// Team settings
inline static int MaxPlayersPerTeam = 1;  // 1 = Solos, 2 = Duos, 4 = Squads

// Bot team assignments
inline static int NextBotTeamIndex = 0;
inline static int CurrentBotsOnTeam = 0;
```

## Offsets Reference

All offsets are for Fortnite 8.51:

| Component | Offset | Purpose |
|-----------|--------|---------|
| GObjects | 0x7A5B0A0 | Object array |
| GIsClient | 0x637925B | Client flag |
| GIsServer | 0x637925C | Server flag |
| CreateNetDriver | 0x13CB180 | Network driver creation |
| InitListen | 0x7EDB70 | Network listener init |
| SetWorld | 0x7EC710 | World assignment |

See individual `.h` files for function-specific offsets.

## Development

To add new features:

1. Create new `.h` file in `8.51/` folder
2. Add to `8.51.vcxproj` in `<ClInclude>` section
3. Include in `framework.h` (order matters!)
4. Implement hooks following existing patterns
5. Call hook init in `dllmain.cpp` Main() function

## Support & Docs

- `README.md` - Full project documentation
- `FIXES_SUMMARY.md` - Detailed fix information
- `QUICK_START.md` - This file
- Individual `.h` files - Feature-specific documentation

## Next Steps

1. Build and test the project
2. Customize configuration in `Globals.h`
3. Test all major features
4. Report any issues with:
   - Fortnite version used
   - Build configuration
   - Console output
   - Steps to reproduce

## Version Info

- **Project:** Project Reboot 8.51 - STATICS Edition
- **Fortnite Version:** 8.51 (Chapter 1 Season 8)
- **Base:** Volcano 8.51
- **AI:** Spectra 10.40 Bot System
- **Build Toolset:** Visual Studio 2022 v143
- **Language Standard:** C++20
- **Architecture:** x64

Good luck and have fun! 🎮
