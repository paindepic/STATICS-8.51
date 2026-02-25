# Installation Guide - Statics Gameserver V1.0.0

## Prerequisites

### Required Software
1. **Visual Studio 2022** (or later)
   - Workload: Desktop development with C++
   - Windows 10 SDK (10.0.19041.0 or later)
   - MSVC v143 toolset

2. **Fortnite 8.51** (Season 8 Chapter 1)
   - Clean installation recommended
   - Make sure it's the exact 8.51 build

3. **DLL Injector**
   - Process Hacker 2
   - Extreme Injector
   - Or any other trusted injector

### Optional but Recommended
- **Project Reboot Launcher** (for 8.51)
- **Visual Studio Code** (for quick file editing)
- **Git** (for version control)

## Building from Source

### Step 1: Clone or Download
```bash
git clone <repository-url>
cd Statics-Gameserver-V1.0.0
```

Or download and extract the ZIP file.

### Step 2: Open in Visual Studio
1. Open `Statics-Gameserver-V1.0.0.sln` in Visual Studio
2. Wait for VS to load all dependencies

### Step 3: Configure Build Settings
1. Set configuration to **Release**
2. Set platform to **x64**
3. Verify the following in project properties:
   - **C++ Language Standard**: ISO C++ Latest (/std:c++latest)
   - **Character Set**: Unicode
   - **Platform Toolset**: Visual Studio 2022 (v143)

### Step 4: Build
1. Build → Build Solution (Ctrl+Shift+B)
2. Wait for compilation to complete
3. Check for errors (there should be none)
4. Output DLL location: `x64/Release/Statics-Gameserver.dll`

### Common Build Issues

#### Error: C1083 - Cannot open include file
**Problem**: Missing SDK files
**Solution**: Ensure the `sdk/` folder is present with all SDK headers

#### Error: C2039 - Member does not exist
**Problem**: Version mismatch or incorrect SDK
**Solution**: Verify you're building for 8.51, not another version

#### Error: LNK2019 - Unresolved external symbol
**Problem**: Missing libcurl or other libraries
**Solution**: Check that `inclueds/` folder contains all required .lib files

## Installation

### Step 1: Prepare Fortnite
1. Launch Fortnite 8.51
2. Get to the lobby (login screen is fine)
3. **Do not click "Play" yet**

### Step 2: Inject the DLL
1. Open your DLL injector
2. Select the Fortnite process (FortniteClient-Win64-Shipping.exe)
3. Inject `Statics-Gameserver.dll`
4. Wait for console window to appear

### Step 3: Verify Installation
You should see a console window titled:
```
Statics Gameserver V1.0.0 - Fortnite 8.51
```

With output similar to:
```
=====================================
  STATICS GAMESERVER V1.0.0
  Fortnite Chapter 1 - Season 8
  Advanced Bot System Enabled
=====================================
Loading Athena Terrain...
Core hooks installed
PC hooks initialized
Abilities initialized
Farming initialized
GameMode hooks initialized
Pawn hooks initialized
Vehicle hooks initialized
Backend initialized
Server tickrate set to 30 Hz
=====================================
  All hooks installed successfully!
  Server is ready for connections
  Bot system is operational
=====================================
```

### Step 4: Connect to Server
1. Server listens on **port 7777**
2. Use Project Reboot or your preferred launcher
3. Set server IP to `127.0.0.1:7777` (for local)
4. Launch the game

## Configuration

### Changing Playlist
Edit `framework.h` line ~20:
```cpp
std::string PlaylistName = "Playlist_DefaultSolo.Playlist_DefaultSolo";
```

Available playlists:
- `Playlist_DefaultSolo.Playlist_DefaultSolo` (Solo)
- `Playlist_DefaultDuo.Playlist_DefaultDuo` (Duos)
- `Playlist_DefaultSquad.Playlist_DefaultSquad` (Squads)
- `Playlist_Playground.Playlist_Playground` (Creative)

### Adjusting Bot Count
Bots must be spawned programmatically. You can add bot spawning logic in `GameMode.h` after the server is ready.

Example (add after line 157 in GameMode.h):
```cpp
// Spawn 10 bots
for (int i = 0; i < 10; i++)
{
    Bots::SpawnPlayerBot();
}
LOG_("Spawned 10 bots!");
```

### Storm Timing
Edit `framework.h` line ~22:
```cpp
bool bLategame = true;  // Set to false for normal storm timing
```

### Server Port
Edit `framework.h` line ~312 in the `Listen()` function:
```cpp
url.Port = 7777;  // Change to your desired port
```

## Bot System Configuration

### Bot AI Behavior
The bot AI is automatically activated when bots are spawned. No additional configuration is needed.

### Bot Skill Level
Currently, all bots use skill level 1. To modify:
Edit `Bots.h` line ~44:
```cpp
BotCustomizationData->SkillLevel = 1;  // Change to 1-10
```

### Bot Starting State
Bots can be spawned in different states:
```cpp
// Spawn in warmup (default)
Bots::SpawnPlayerBot();

// Spawn already on the bus
Bots::SpawnPlayerBot(nullptr, PlayerBots::EBotState::Bus);

// Spawn in combat mode
Bots::SpawnPlayerBot(nullptr, PlayerBots::EBotState::Combat);
```

### Bot Names
Bot names are loaded from `BotNames.h`. You can add more names by editing the `BotDisplayNames` vector.

## Troubleshooting

### Server doesn't start
**Symptoms**: No console appears after injection
**Solutions**:
1. Check that you injected into the correct process
2. Verify the DLL is not blocked by Windows (Right-click → Properties → Unblock)
3. Run as administrator
4. Disable antivirus temporarily

### Bots don't spawn
**Symptoms**: Server starts but no bots appear
**Solutions**:
1. Check console for error messages
2. Verify `PlayerStarts` are found (check logs)
3. Ensure `BuildingFoundations` are detected
4. Manually trigger bot spawning (see Bot Count section)

### Crashes on connection
**Symptoms**: Client crashes when joining server
**Solutions**:
1. Verify version match (8.51 client + 8.51 server)
2. Check that all SDK files are correct for 8.51
3. Review console logs for hook failures
4. Rebuild in Debug mode for more detailed errors

### Bots behave strangely
**Symptoms**: Bots stand still, don't build, or glitch
**Solutions**:
1. Check that `Floor`, `Roof`, `Stair`, `Wall`, `EditTool` loaded (console logs)
2. Verify `PlayerStarts` and `BuildingFoundations` are populated
3. Ensure bot tick rate is 30 Hz (check MaxTickRate function)
4. Review PlayerBots.h for state machine issues

### Cannot build project
**Symptoms**: Compilation errors in Visual Studio
**Solutions**:
1. Clean solution (Build → Clean Solution)
2. Rebuild (Build → Rebuild Solution)
3. Check that C++ language standard is set to /std:c++latest
4. Verify all files are present (see File Structure in README.md)
5. Check that SDK paths are correct

## Advanced Configuration

### Logging
Logs are written to:
- `StaticsGameserver.log` (main log)
- `StaticsGameserver_detailed.log` (detailed log)

To increase logging verbosity, add more `LOG_()` calls in relevant files.

### Network Settings
For remote connections, configure:
1. Port forwarding on router (port 7777)
2. Firewall rules to allow incoming connections
3. Update `Listen()` function in framework.h if needed

### Custom Bot Behavior
To customize bot AI:
1. Edit `PlayerBots.h` - Main bot logic
2. Modify state machine transitions
3. Adjust timing constants (tick intervals)
4. Add new bot behaviors in respective services (Combat, Loot, Healing, etc.)

### Backend Integration
The backend system from Volcano is preserved. To use:
1. Configure backend URL in `backend.h`
2. Implement endpoint handlers
3. Use `Backend::Setup()` in dllmain.cpp

## Testing

### Local Testing
1. Inject DLL into Fortnite 8.51
2. Wait for "Server is ready" message
3. Connect with client to 127.0.0.1:7777
4. Spawn bots using configured method
5. Observe bot behavior in-game

### Bot AI Testing
Watch for:
- ✅ Bots drop from bus correctly
- ✅ Bots loot chests and pickups
- ✅ Bots engage in combat when seeing players
- ✅ Bots build walls/ramps during combat
- ✅ Bots heal when damaged and safe
- ✅ Bots move to safe zone when storm approaches
- ✅ Bots run when moving long distances

### Performance Testing
Monitor:
- Server FPS (should maintain 30 Hz)
- Memory usage (watch for leaks)
- Network bandwidth
- Bot tick performance

## Support

For issues, questions, or contributions:
1. Check existing issues in repository
2. Review console logs for specific errors
3. Ensure all prerequisites are met
4. Verify file integrity (redownload if needed)

## Next Steps

After successful installation:
1. Experiment with bot count
2. Try different playlists
3. Customize bot behavior
4. Add custom features
5. Share your modifications with the community

Happy building! 🎮
