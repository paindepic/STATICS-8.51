# Quick Start Guide - Statics Gameserver V1.0.0

## 🚀 Quick Start (5 Minutes)

### Step 1: Build (2 minutes)
1. Open `Statics-Gameserver-V1.0.0.sln` in Visual Studio 2022
2. Select **Release | x64**
3. Press **Ctrl+Shift+B** to build
4. Find DLL at: `x64/Release/Statics-Gameserver.dll`

### Step 2: Run (1 minute)
1. Launch Fortnite 8.51 (Project Reboot launcher recommended)
2. Wait until you're in the lobby
3. Inject `Statics-Gameserver.dll` using your preferred injector
4. Console window appears: "Statics Gameserver V1.0.0"

### Step 3: Connect (1 minute)
1. Server auto-starts on port **7777**
2. Use Project Reboot launcher or similar
3. Connect to `127.0.0.1:7777`
4. You're in!

### Step 4: Spawn Bots (1 minute)
Bots must be spawned programmatically. Quick way to add:

Edit `GameMode.h` at line ~157 (after server initialization), add:
```cpp
// Spawn 20 bots for testing
LOG_("Spawning bots...");
for (int i = 0; i < 20; i++)
{
    Bots::SpawnPlayerBot();
}
LOG_("20 bots spawned successfully!");
```

Then rebuild and inject again.

## 📦 What You Get

### Intelligent Bots That Can:
- ✅ Drop from bus optimally
- ✅ Loot chests and floor loot
- ✅ Engage in combat
- ✅ Build walls, ramps, floors, roofs
- ✅ Edit structures
- ✅ Strafe during fights
- ✅ Heal when damaged
- ✅ Move to safe zones
- ✅ Run when traveling
- ✅ Do 90s, boxfights, buildfights

### Full Server Features:
- ✅ Multiplayer support (port 7777)
- ✅ Floor loot spawning
- ✅ Chest loot
- ✅ Ability system
- ✅ Farming/harvesting
- ✅ Vehicles
- ✅ Teams & squads
- ✅ Storm system
- ✅ Backend HTTP support

## 🎮 Bot Behavior Preview

### What Bots Do Automatically:
1. **Warmup Phase**: Emote, fight players, or build
2. **Bus Phase**: Calculate drop zone, jump at right time
3. **Skydiving/Gliding**: Navigate to target location
4. **Landing**: Switch to looting mode
5. **Looting**: 
   - Find nearest chests/pickups
   - Prioritize weapons → heals → shields
   - Smart inventory management
6. **Combat**:
   - Build walls for protection
   - Place ramps to push
   - Strafe left/right
   - Switch from pickaxe to guns
7. **Healing**: Use bandages < 75 HP, medkits any HP
8. **Safe Zone**: Auto-navigate when storm approaches

## 🛠️ Common Adjustments

### Change Bot Count
In `GameMode.h`, line ~157:
```cpp
for (int i = 0; i < YOUR_NUMBER_HERE; i++)
{
    Bots::SpawnPlayerBot();
}
```

### Change Playlist
In `framework.h`, line ~20:
```cpp
std::string PlaylistName = "Playlist_DefaultSquad.Playlist_DefaultSquad";
// Options: DefaultSolo, DefaultDuo, DefaultSquad, Playground
```

### Change Server Port
In `framework.h`, line ~312:
```cpp
url.Port = 7777;  // Your port here
```

### Adjust Storm Timing
In `framework.h`, line ~22:
```cpp
bool bLategame = true;   // false for normal timing
```

## ⚡ Troubleshooting

### "Cannot open include file" errors
- Check that `sdk/` folder exists
- Verify all SDK headers are present
- SDK should be from Volcano 8.51

### "Unresolved external symbol" errors
- Ensure `inclueds/` folder has libcurl libraries
- Check that minhook library is present
- Verify linker settings in project properties

### Server starts but no bots
- Check console for "Spawned bots" message
- Verify PlayerStarts found (check logs)
- Ensure building items loaded (Floor, Roof, Stair, Wall)
- Manually add bot spawning code (see Step 4 above)

### Bots stand still
- Building items may not have loaded
- Check that `Floor`, `Roof`, `Stair`, `Wall`, `EditTool` are found
- Review console logs for errors

### Client crashes when joining
- Verify version match (8.51 client + 8.51 server)
- Check that all hooks installed successfully
- Review console for hook failures

## 📚 More Information

- **Full Documentation**: See `README.md`
- **Installation Guide**: See `INSTALLATION.md`
- **Project Details**: See `PROJECT_SUMMARY.md`

## 💡 Tips for Best Experience

1. **Start Small**: Test with 5-10 bots first
2. **Watch Console**: Logs show what bots are doing
3. **Check Files**: Make sure building items load
4. **Test Features**: Try different playlists
5. **Monitor Performance**: Keep eye on FPS/tickrate

## 🎯 Success Indicators

You know it's working when:
- ✅ Console says "Server is ready for connections"
- ✅ Console says "Bot system is operational"  
- ✅ You can connect to 127.0.0.1:7777
- ✅ Bots appear in player list
- ✅ Bots drop from bus
- ✅ Bots loot items
- ✅ Bots fight you when they see you

## 🚨 Important Notes

- **Version**: Must be Fortnite 8.51 (Season 8)
- **SDK**: Uses Volcano's 8.51 SDK
- **Offsets**: All verified for 8.51
- **Compatibility**: Not compatible with other versions

## 🎊 You're Ready!

Everything is set up and ready to go. Just:
1. Build the project
2. Inject the DLL
3. Connect to the server
4. Enjoy playing with intelligent bots!

**Have fun building and fighting! 🎮**

---

*For advanced customization, see the other documentation files.*
*For issues or questions, check the console logs first.*
