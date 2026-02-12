# Statics Gameserver V1.0.0
## Advanced Fortnite 8.51 Private Server with Intelligent Bot System

### Overview
Statics Gameserver V1.0.0 is a hybrid gameserver combining the robust infrastructure of Volcano 8.51 with the advanced bot AI system from Spectra. This creates a fully-featured Fortnite Chapter 1 Season 8 private server with highly intelligent bots capable of:

- ✅ **Advanced Combat AI**: Bots can build, edit, and engage in buildfights
- ✅ **Smart Looting**: Intelligent item prioritization and inventory management
- ✅ **Movement Systems**: Proper skydiving, gliding, running, and strafing
- ✅ **Healing Logic**: Context-aware healing decisions
- ✅ **Zone Awareness**: Bots navigate to safe zones intelligently
- ✅ **90s, Boxfights, and Editing**: Advanced building techniques
- ✅ **Team Support**: Proper team and squad system integration
- ✅ **Full Warmup Behaviors**: Emoting, pickaxing, and player interaction during warmup

### Key Features

#### 🤖 Intelligent Bot System
The bot system is based on Spectra's PlayerBots architecture, featuring:
- **Multiple Bot States**: Warmup, Bus, Skydiving, Gliding, Landed, Looting, Combat, Healing, MovingToSafeZone
- **Decision Making**: Bots evaluate their situation and make intelligent choices
- **Inventory Management**: Smart weapon swapping, item prioritization, and drop management
- **Combat AI**: Line-of-sight tracking, strafing, building for cover, and weapon usage
- **Building Skills**: Can place walls, floors, stairs, roofs, and perform edits
- **Healing System**: Contextual healing with proper item selection

#### 🎮 Full Volcano 8.51 Features
- **Network System**: Full multiplayer support with netcode
- **Looting System**: Floor loot, chest loot, and container spawning
- **Abilities**: Complete ability system integration
- **Farming**: Resource gathering from structures and props
- **Vehicles**: Vehicle spawning and interaction
- **Teams**: Proper team and squad management
- **Backend**: HTTP backend support with libcurl
- **Pawn System**: Full player and bot pawn management

### Technical Architecture

#### File Structure
```
Statics-Gameserver-V1.0.0/
├── Statics-Gameserver/
│   ├── Core System Files
│   │   ├── framework.h          # Core framework with utility functions
│   │   ├── dllmain.cpp          # Main entry point
│   │   ├── GameMode.h           # Game mode and initialization
│   │   ├── ue.h                 # Unreal Engine types
│   │   └── Globals.h            # Global variables
│   │
│   ├── Bot System Files
│   │   ├── PlayerBots.h         # Main bot AI logic (95KB of intelligence!)
│   │   ├── Bots.h               # Bot spawning and management
│   │   ├── ServerBotManager.h  # Server-side bot manager
│   │   ├── AIController.h       # AI controller interface
│   │   └── BotNames.h           # Bot name database
│   │
│   ├── Gameplay Systems
│   │   ├── PC.h                 # Player controller hooks
│   │   ├── Pawn.h               # Pawn hooks and behavior
│   │   ├── Abilites.h           # Ability system
│   │   ├── Farming.h            # Resource gathering
│   │   ├── Inventory.h          # Inventory management
│   │   ├── Looting.h            # Loot spawning and pickup
│   │   ├── Teams.h              # Team management
│   │   ├── Vehicles.h           # Vehicle system
│   │   ├── FortInventory.h      # Fortnite inventory specifics
│   │   ├── FortPickup.h         # Pickup handling
│   │   └── BuildingContainer.h  # Container interaction
│   │
│   ├── SDK/
│   │   └── sdk/                 # Dumped Unreal Engine SDK for 8.51
│   │
│   ├── minhook/                 # MinHook library for hooking
│   │
│   └── inclueds/                # Additional includes (libcurl, etc.)
│
├── Statics-Gameserver.sln       # Visual Studio solution
└── README.md                     # This file
```

#### Bot AI Decision Tree
```
Bot Tick Loop (30 Hz)
├── AI Evaluator
│   ├── Update nearest player/loot
│   ├── Check health/shields for healing
│   ├── Determine looting priorities
│   └── Check safe zone status
│
├── State Machine
│   ├── Warmup → Choose warmup behavior (emote, fight, build, etc.)
│   ├── Bus → Monitor drop zone distance, jump when optimal
│   ├── Skydiving → Navigate toward target location
│   ├── Gliding → Adjust trajectory, spot loot
│   ├── Landed → Transition to looting
│   ├── Looting → Find and collect weapons, mats, heals
│   ├── Combat → Engage enemies, build, strafe, shoot
│   ├── Healing → Use appropriate healing items
│   └── MovingToSafeZone → Navigate to next safe zone
│
└── Inventory Manager
    ├── Item swapping logic
    ├── Weapon prioritization
    ├── Ammo/resource stacking
    └── Drop management
```

### Building the Project

#### Requirements
- **Visual Studio 2022** or later
- **Windows 10/11 SDK**
- **C++20 or later** (stdcpplatest)
- **Fortnite 8.51** (Season 8 Chapter 1)

#### Build Steps
1. Open `Statics-Gameserver-V1.0.0.sln` in Visual Studio
2. Set configuration to **Release x64**
3. Build Solution (Ctrl+Shift+B)
4. Output DLL will be in `x64/Release/Statics-Gameserver.dll`

### Usage

#### Injection
1. Launch Fortnite 8.51
2. Inject `Statics-Gameserver.dll` using your preferred injector
3. Wait for console window "Statics Gameserver V1.0.0"
4. Server will automatically load Athena_Terrain
5. Connect on port 7777

#### Spawning Bots
Bots can be spawned programmatically via the `Bots::SpawnPlayerBot()` function:

```cpp
// Spawn a bot in warmup state at a random spawn location
Bots::SpawnPlayerBot();

// Spawn a bot at a specific location starting in combat state
Bots::SpawnPlayerBot(SpecificSpawnLocation, PlayerBots::EBotState::Combat);
```

#### Configuration
Edit `framework.h` to configure:
- **Playlist**: `Globals::PlaylistName` (default: DefaultSolo)
- **Late Game**: `Globals::bLategame` (storm timing)
- **Team Size**: Automatically detected from playlist

### Bot Capabilities Showcase

#### 🎯 Combat Intelligence
- **Tracking**: Uses line-of-sight checks and distance calculations
- **Building**: Places walls for protection, builds ramps to push
- **Strafing**: Random strafe timing and direction for unpredictability
- **Weapon Management**: Switches from pickaxe to guns in combat
- **Crouching**: Occasionally crouches during fights

#### 🏗️ Building & Editing
- **Quick Building**: Can place walls, floors, stairs, and roofs
- **Edit Tool**: Equipped with edit tool and can perform edits
- **Build Fights**: Capable of engaging in building battles
- **90s**: Can perform 90-degree turns and vertical building

#### 🎒 Loot Management
- **Smart Prioritization**: Prioritizes weapons → heals → shields → ammo
- **Inventory Swapping**: Automatically swaps worse items for better ones
- **Rarity Awareness**: Prefers higher rarity items
- **Type Categorization**: Understands close/medium/long range weapons

#### 💊 Healing System
- **Context-Aware**: Only heals when safe (not under attack)
- **Item Selection**: Chooses appropriate healing items (bandages < 75 HP, medkit at any HP)
- **Shield Management**: Uses small shields when shield < 50, big shields otherwise
- **State Transition**: Returns to combat or looting after healing

#### 🚁 Movement & Navigation
- **Bus Dropping**: Calculates optimal jump time based on distance
- **Skydiving**: Navigates toward target drop zone
- **Running**: Auto-sprint when moving
- **Safe Zone**: Automatically moves to next safe zone when needed
- **Anti-Stuck**: Teleports if stuck for too long

### Differences from Original Projects

#### vs Volcano 8.51
- ✅ Added full bot system with AI
- ✅ Enhanced framework with bot-specific utilities
- ✅ Integrated ServerBotManager
- ✅ Added building item management for bots
- ✅ Improved logging system

#### vs Spectra 10.40
- ✅ Adapted to 8.51 SDK and offsets
- ✅ Integrated Volcano's networking
- ✅ Uses Volcano's GameMode structure
- ✅ Compatible with 8.51 ability system
- ✅ Works with 8.51 backend integration

### Known Issues & Limitations
- ⚠️ Bot building could be more aggressive in some situations
- ⚠️ Editing functionality is basic (can be expanded)
- ⚠️ Vehicle usage by bots is not implemented
- ⚠️ DBNO (knocked) state handling is limited

### Future Improvements
- 🚀 More advanced editing patterns
- 🚀 Building techniques (double edits, tunneling)
- 🚀 Vehicle operation by bots
- 🚀 Better team coordination
- 🚀 Adaptive difficulty scaling
- 🚀 Custom bot loadouts

### Credits
- **Volcano 8.51**: Base gameserver structure and networking
- **Spectra 10.40**: Advanced bot AI system
- **Statics Team**: Integration, adaptation, and enhancement

### Version
**V1.0.0** - Initial Release
- Fully functional hybrid gameserver
- Complete bot AI integration
- All Volcano features preserved
- Enhanced for 8.51 compatibility

### License
This project is for educational purposes only. Fortnite and all related assets are property of Epic Games.

---

**Made with ❤️ for the Fortnite modding community**
