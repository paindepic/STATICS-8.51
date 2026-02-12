# Instructions de Build - Volcano 8.51 Game Server

## Vue d'ensemble

Volcano-8.51 est un serveur de jeu privé pour Fortnite 8.51 (Chapitre 1 Saison 8) avec support des bots avancés. Ce projet compile en une DLL Windows qui s'injecte dans le processus Fortnite pour créer un serveur dédié fonctionnel.

---

## Configuration requise

### Logiciels
- **Visual Studio 2022** ou supérieur
  - Workload: "Desktop development with C++"
  - Platform toolset: v143
- **Windows SDK 10.0** ou supérieur
- **Git** (optionnel, pour version control)

### Système
- **OS**: Windows 10/11 (64-bit)
- **RAM**: 8 GB minimum, 16 GB recommandé
- **Disque**: 2 GB d'espace libre

---

## Structure du projet

```
Volcano-8.51-master/
├── Volcano-8.51/
│   ├── dllmain.cpp           # Point d'entrée DLL
│   ├── framework.h           # Fonctions framework core
│   ├── ue.h                  # Types Unreal Engine
│   ├── gaymode.h             # Game mode logic
│   ├── PC.h                  # Player Controller hooks
│   ├── Pawn.h                # Pawn management
│   ├── Inventory.h           # Système d'inventaire
│   ├── Looting.h             # Système de loot
│   ├── Abilites.h            # Gameplay abilities
│   ├── Farming.h             # Resource farming
│   ├── Teams.h               # Team management
│   ├── Vehicles.h            # Vehicle system
│   ├── backend.h             # Backend API (curl)
│   ├── sdk/                  # SDK Unreal Engine généré
│   │   ├── SDK.hpp           # Header principal SDK
│   │   └── SDK/              # Fichiers générés (structs, classes, functions)
│   ├── minhook/              # MinHook library pour hooking
│   │   ├── MinHook.h
│   │   └── minhook.lib
│   └── inclueds/             # Dépendances externes
│       └── curl/             # libcurl statique
│           ├── libcurl.lib
│           ├── zlib.lib
│           └── *.h
├── Volcano-8.51.sln          # Solution Visual Studio
└── README.md                 # Documentation
```

---

## Étapes de compilation

### 1. Cloner/Extraire le projet

```bash
git clone <repository-url>
cd Volcano-8.51-master
```

Ou extraire l'archive ZIP.

### 2. Ouvrir la solution

1. Double-cliquer sur `Volcano-8.51.sln`
2. Visual Studio va s'ouvrir et charger le projet

### 3. Configurer la build

**Barre d'outils Visual Studio:**
- **Configuration**: Sélectionner `Release`
- **Platform**: Sélectionner `x64`

**Important**: Ne pas utiliser Debug ou Win32 - le jeu est 64-bit uniquement.

### 4. Restaurer les dépendances (si nécessaire)

Les dépendances (libcurl, MinHook) sont déjà incluses dans le projet.

Vérifier que ces fichiers existent:
- `Volcano-8.51/minhook/minhook.lib`
- `Volcano-8.51/inclueds/curl/libcurl.lib`
- `Volcano-8.51/inclueds/curl/zlib.lib`

### 5. Build le projet

**Méthode 1: Menu**
```
Build > Build Solution
```

**Méthode 2: Raccourci**
```
Ctrl + Shift + B
```

**Méthode 3: Ligne de commande** (depuis Developer Command Prompt)
```bash
msbuild Volcano-8.51.sln /p:Configuration=Release /p:Platform=x64
```

### 6. Vérifier l'output

Si la compilation réussit, vous trouverez:
```
Volcano-8.51/x64/Release/Volcano-8.51.dll
```

---

## Configuration du projet

### Options de compilation (vcxproj)

#### Langage
- **C++ Standard**: `stdcpplatest` (C++23 features, mais avec workarounds)
- **Character Set**: Unicode
- **Conformance Mode**: Oui

#### Includes
```
inclueds;%(AdditionalIncludeDirectories)
```

#### Linking
- **Subsystem**: Windows
- **Generate Debug Info**: Oui (même en Release)
- **Enable UAC**: Non

#### Bibliothèques
```
curl/libcurl.lib
curl/zlib.lib
OneCore.lib
minhook/minhook.lib (via pragma comment)
```

#### Preprocessor Definitions (Release x64)
```
NDEBUG
VOLCANO851_EXPORTS
_WINDOWS
_USRDLL
WINDOWS_IGNORE_PACKING_MISMATCH
```

---

## Résolution des problèmes

### Erreur: LNK1104 - Cannot open file 'libcurl.lib'

**Solution**:
1. Vérifier que `inclueds/curl/libcurl.lib` existe
2. Vérifier le chemin dans les Additional Library Directories
3. Recompiler avec configuration x64 (pas Win32)

### Erreur: C1083 - Cannot open include file 'curl/curl.h'

**Solution**:
1. Vérifier que `inclueds/curl/` contient les headers
2. Ajouter `inclueds` aux Additional Include Directories
3. Properties > C/C++ > General > Additional Include Directories

### Erreur: C2039 - 'contains': is not a member of 'std::basic_string'

**Cause**: Utilisation de `std::string::contains()` (C++23)

**Solution**: Les corrections ont déjà remplacé `.contains()` par `.find() != std::string::npos`

Voir `CORRECTIONS_APPLIED.md` pour détails.

### Erreur: C2664 - Cannot convert argument

**Causes courantes**:
1. Mauvaise configuration de platform (Win32 au lieu de x64)
2. Types incompatibles dans le SDK

**Solution**:
1. Changer configuration en x64
2. Clean + Rebuild
3. Vérifier que tous les typedefs sont cohérents

### Erreur: C2530 - Function must return a value

**Solution**: Déjà corrigé dans `backend.h`. Si vous voyez cette erreur ailleurs:
1. Vérifier que toutes les fonctions non-void ont un `return`
2. Vérifier les chemins de contrôle (if/else)

---

## Utilisation de la DLL

### Injection

La DLL doit être injectée dans le processus `FortniteClient-Win64-Shipping.exe`.

**Outils d'injection recommandés**:
- Xenos Injector
- Extreme Injector
- Process Hacker (manual inject)

**Méthode d'injection**: Manual Map recommandé

### Configuration

Avant d'injecter, configurer dans `framework.h`:

```cpp
namespace Globals
{
    std::string PlaylistName = "Playlist_DefaultSolo.Playlist_DefaultSolo";
    bool bLategame = true; // false pour partie normale
}
```

### Port du serveur

Par défaut: **7777**

Modifiable dans `framework.h` ligne 312:
```cpp
url.Port = 7777;
```

---

## Fonctionnalités

### ✅ Implémenté

- [x] Serveur Listen sur port 7777
- [x] Support multi-joueurs
- [x] Système d'inventaire complet
- [x] Loot au sol automatique
- [x] Coffres et conteneurs
- [x] Construction (building)
- [x] Édition de structures
- [x] Teams et Squads
- [x] Respawn system
- [x] Abilities (Gameplay Abilities)
- [x] Farming de ressources
- [x] Emotes
- [x] Véhicules (partiel)
- [x] Storm/Safe Zone
- [x] Avion de bus
- [x] Late game mode
- [x] Backend API (curl)

### 🔧 En cours / Partiel

- [ ] Bots AI avancés (structure présente)
- [ ] Véhicules armés
- [ ] Quest system
- [ ] Auto-pickup complet

---

## Backend API

Le projet inclut un wrapper curl pour communiquer avec un backend externe.

### Configuration

Dans `backend.h`:
```cpp
namespace Backend
{
    void Setup(); // Appeler au démarrage
    
    // Exemple d'utilisation
    std::string response;
    api->Request(EReqType_GET, "http://your-backend.com/api/endpoint", "{\"data\":\"value\"}", &response);
}
```

### Endpoints suggérés

- `POST /server/register` - Enregistrer le serveur
- `GET /matchmaking/players` - Liste des joueurs
- `DELETE /server/shutdown` - Arrêt propre

---

## Développement

### Ajouter une nouvelle fonctionnalité

1. **Créer un nouveau header** (ex: `MyFeature.h`)
2. **Inclure dans `dllmain.cpp`**
```cpp
#include "MyFeature.h"
```
3. **Ajouter init dans `Main()`**
```cpp
InitMyFeature();
```
4. **Hook les fonctions UE4** via MinHook ou VirtualHook

### Exemple de hook

```cpp
void (*OriginalFunction)(UObject* Obj, int Param);

void HookedFunction(UObject* Obj, int Param)
{
    LOG_("Function called with param: {}", Param);
    
    // Custom logic
    
    return OriginalFunction(Obj, Param); // Call original
}

void InitMyFeature()
{
    MH_CreateHook(
        (LPVOID)GetOffsetBRUH(0x12345678), // Offset from base
        HookedFunction,
        (void**)&OriginalFunction
    );
    MH_EnableHook((LPVOID)GetOffsetBRUH(0x12345678));
}
```

### Logging

```cpp
LOG_("Message simple");
LOG_("Message avec variable: {}", myValue);
LOG_("Multiple values: {}, {}, {}", a, b, c);
```

Les logs vont dans:
- Console Windows (AllocConsole)
- Fichier `FortniteLogs.log`

---

## Offsets

Les offsets sont hardcodés pour Fortnite 8.51. Si vous utilisez une autre version:

1. Dumper le SDK avec un outil (Dumper-7, etc.)
2. Mettre à jour les offsets dans:
   - `framework.h` (fonctions globales)
   - `dllmain.cpp` (hooks principaux)
   - Headers individuels (hooks spécifiques)

### Format des offsets
```cpp
GetOffsetBRUH(0xABCDEF) // Offset depuis base du module
```

---

## Performance

### Optimisations Release

- Whole Program Optimization: ON
- Intrinsic Functions: ON
- Optimization: /O2 (Maximize Speed)
- Inline Function Expansion: Any Suitable

### Profilage

Pour profiler les performances:
1. Activer Generate Debug Info en Release
2. Utiliser Visual Studio Profiler
3. Identifier les hotspots avec les logs

---

## Sécurité

⚠️ **Avertissement**: Ce projet est à des fins éducatives uniquement.

- Ne pas utiliser sur les serveurs officiels
- Ne pas distribuer la DLL compilée
- Respecter les ToS d'Epic Games
- Utiliser uniquement dans un environnement de test privé

---

## Contribution

Pour contribuer:

1. Fork le repository
2. Créer une branche feature (`git checkout -b feature/AmazingFeature`)
3. Commit les changements (`git commit -m 'Add AmazingFeature'`)
4. Push vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrir une Pull Request

### Standards de code

- Indentation: Tabs
- Naming: PascalCase pour fonctions, camelCase pour variables
- Comments: En anglais de préférence
- Logging: LOG_() pour debug

---

## Licence

Ce projet est fourni "as is" sans garantie d'aucune sorte.

---

## Crédits

- SDK Generator: Encryqed & Fischsalat
- MinHook: TsudaKageyu
- libcurl: Daniel Stenberg & contributors
- Unreal Engine: Epic Games

---

## Support

Pour obtenir de l'aide:

1. Vérifier `CORRECTIONS_APPLIED.md` pour les erreurs connues
2. Lire attentivement ce document
3. Vérifier les logs dans `FortniteLogs.log`
4. Consulter le code source pour comprendre le flow

---

**Version**: 1.0.0.2
**Date**: 2026-02-12
**Statut Build**: ✅ Fonctionnel (102+ erreurs corrigées)
