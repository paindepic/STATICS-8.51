# Volcano 8.51 - Fortnite Private Server (Season 8)

![Version](https://img.shields.io/badge/version-1.0.0.2-blue)
![Fortnite](https://img.shields.io/badge/Fortnite-8.51%20S8-orange)
![Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Windows%2064--bit-lightgrey)

Un serveur de jeu privé pour Fortnite 8.51 (Chapitre 1 Saison 8) avec support des bots avancés et backend personnalisable.

---

## 📋 Vue d'ensemble

Volcano-8.51 est une DLL d'injection qui transforme le client Fortnite en serveur de jeu fonctionnel. Le projet inclut:

- 🎮 **Serveur Listen** complet sur port 7777
- 🤖 **Infrastructure pour bots AI** avancés
- 🔧 **Systèmes de jeu complets** (inventaire, loot, building, farming)
- 🌐 **Backend API** avec support libcurl
- 🎯 **Hooking propre** via MinHook et Virtual Function Tables
- 🔥 **Late game mode** pour démarrage rapide

---

## ✨ Fonctionnalités

### ✅ Implémenté et fonctionnel

#### Gameplay Core
- [x] **Farming** - Récolte de ressources avec weak spots
- [x] **Building/Editing** - Construction et édition de structures
- [x] **Inventaire** - Système complet avec gestion des stacks
- [x] **Looting** - Loot au sol, coffres, conteneurs d'ammo
- [x] **Emoting** - Support complet des emotes
- [x] **Véhicules** - Système de véhicules physiques

#### Multijoueur
- [x] **Teams** - Gestion des équipes et squads
- [x] **Respawning** - Système de respawn configurable
- [x] **Change Teams** - Changement d'équipe en Playground
- [x] **Netcode** - Réplication réseau fonctionnelle

#### Serveur
- [x] **Listen Server** - Serveur sur port 7777
- [x] **Different LTMs** - Support de plusieurs playlists
- [x] **Late game** - Mode partie avancée
- [x] **Storm/Safe Zone** - Gestion du storm
- [x] **Bus** - Avion de début de partie

#### Techniques
- [x] **Proper Hooking** - MinHook + VTable hooking
- [x] **Backend API** - Wrapper curl pour backend externe
- [x] **Logging** - Système de logs console + fichier

### 🔧 En cours / Partiel

- [ ] **AutoPickup** - Auto-ramassage (structure présente)
- [ ] **MCP** - Profil MCP (bugged)
- [ ] **Bots AI** - AI avancée (infrastructure présente)
- [ ] **Vehicle Weapons** - Armes de véhicules

---

## 🚀 Démarrage rapide

### Prérequis

- **Visual Studio 2022** (v143 toolset)
- **Windows 10/11** 64-bit
- **Fortnite 8.51** (Chapitre 1 Saison 8)

### Build

1. **Ouvrir** `Volcano-8.51.sln` dans Visual Studio
2. **Sélectionner** configuration `Release` et platform `x64`
3. **Build** avec `Ctrl+Shift+B`
4. **Output**: `x64/Release/Volcano-8.51.dll`

Pour plus de détails, voir [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md)

### Utilisation

1. Lancer Fortnite 8.51
2. Injecter `Volcano-8.51.dll` avec votre injector préféré (Manual Map recommandé)
3. La console s'ouvre automatiquement avec les logs
4. Le serveur démarre sur port **7777**
5. Les joueurs peuvent se connecter via `open <IP>:7777`

---

## 📁 Structure du projet

```
Volcano-8.51/
├── dllmain.cpp           # Point d'entrée et initialisation
├── framework.h           # Fonctions core et helpers
├── ue.h                  # Types Unreal Engine personnalisés
│
├── gaymode.h             # Game Mode logic et playlist
├── PC.h                  # Player Controller hooks (5000+ lignes)
├── Pawn.h                # Pawn management et pickup
├── Inventory.h           # Système d'inventaire complet
├── Looting.h             # Loot tables et spawn
├── Abilites.h            # Gameplay Abilities System
├── Farming.h             # Resource harvesting
├── Teams.h               # Team et Squad management
├── Vehicles.h            # Vehicle physics
├── backend.h             # Backend API (curl wrapper)
│
├── sdk/                  # Unreal Engine SDK (généré)
│   ├── SDK.hpp           # Header principal
│   └── SDK/              # Structs, classes, functions
│
├── minhook/              # MinHook library
└── inclueds/             # Dépendances externes
    └── curl/             # libcurl + zlib (statique)
```

---

## 🔧 Configuration

### Playlist

Dans `framework.h`:
```cpp
namespace Globals
{
    std::string PlaylistName = "Playlist_DefaultSolo.Playlist_DefaultSolo";
    // Autres: Duos, Squads, Playground, etc.
    
    bool bLategame = true; // Démarrage late game
}
```

### Port du serveur

Dans `framework.h` ligne 312:
```cpp
url.Port = 7777; // Changer si nécessaire
```

### Backend

Dans `backend.h`, configurer vos endpoints:
```cpp
api->Request(EReqType_GET, "http://your-api.com/endpoint", body, &response);
```

---

## 📊 Statistiques de code

- **Fichiers C++**: 12 headers + 1 source
- **Lignes de code**: ~6000+ (sans SDK)
- **SDK généré**: 800+ fichiers
- **Hooks**: 50+ fonctions hookées
- **Systèmes**: 10+ (Inventory, Looting, Building, etc.)

---

## 🐛 Débogage

### Logs

Les logs sont écrits dans:
- **Console** (AllocConsole)
- **Fichier**: `FortniteLogs.log`

Format:
```cpp
LOG_("Message simple");
LOG_("Avec variable: {}", value);
LOG_("Multiple: {}, {}, {}", a, b, c);
```

### Erreurs communes

Voir [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) section "Résolution des problèmes"

### Corrections récentes

**Version 1.0.0.2** - 102+ erreurs de compilation corrigées:
- C2664, C3312, C2530, C2143, C2451, C2378
- Détails complets: [CORRECTIONS_APPLIED.md](CORRECTIONS_APPLIED.md)

---

## 🎯 Roadmap

### Court terme
- [ ] Finaliser AutoPickup
- [ ] Corriger MCP bugs
- [ ] Améliorer stabilité réseau
- [ ] Documenter tous les offsets

### Moyen terme
- [ ] Bot AI avancée
- [ ] Support d'autres versions Fortnite
- [ ] Web dashboard pour backend
- [ ] Matchmaking

### Long terme
- [ ] Support Linux (Wine/Proton)
- [ ] Outils d'administration
- [ ] Replay system
- [ ] Custom game modes

---

## 👥 Contribution

Les contributions sont bienvenues! Pour contribuer:

1. **Fork** le projet
2. **Create** une branche feature (`git checkout -b feature/AmazingFeature`)
3. **Commit** vos changements (`git commit -m 'Add AmazingFeature'`)
4. **Push** vers la branche (`git push origin feature/AmazingFeature`)
5. **Open** une Pull Request

### Standards

- Code en **C++20/23**
- Commentaires en **anglais**
- Indentation: **Tabs**
- Naming: **PascalCase** (fonctions), **camelCase** (variables)
- Tests avant commit

---

## 📜 Crédits

### Développement
- **Auteur principal**: jyzo. (Discord: "@jyzo.")
- **SDK Dumper**: [Dumper-7](https://github.com/Encryqed/Dumper-7) par Encryqed & Fischsalat
- **Corrections v1.0.0.2**: [CTO.new](https://cto.new) AI Assistant

### Bibliothèques
- **MinHook**: TsudaKageyu - Hooking library
- **libcurl**: Daniel Stenberg & contributors - HTTP requests
- **Unreal Engine**: Epic Games

### Communauté
- Merci à tous les contributeurs et testeurs
- Discord community pour le feedback

---

## ⚠️ Avertissement légal

> **Ce projet est à des fins éducatives uniquement.**
> 
> - ❌ Ne PAS utiliser sur serveurs officiels Epic Games
> - ❌ Ne PAS distribuer la DLL compilée publiquement
> - ✅ Utiliser UNIQUEMENT dans un environnement de test privé
> - ✅ Respecter les Terms of Service d'Epic Games
> 
> Les auteurs ne sont pas responsables de l'utilisation abusive de ce code.

---

## 📞 Support & Contact

- **Discord**: @jyzo.
- **Issues**: [GitHub Issues](../../issues)
- **Documentation**: Voir [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) et [CORRECTIONS_APPLIED.md](CORRECTIONS_APPLIED.md)

---

## 📄 Licence

Ce projet est fourni **"as is"** sans garantie d'aucune sorte.

Utilisation à vos propres risques.

---

## ⭐ Star ce repo!

**STAR FOR MORE UPDATES**

Si ce projet vous aide, n'hésitez pas à lui donner une étoile ⭐

---

**Version**: 1.0.0.2  
**Dernière mise à jour**: 12 Février 2026  
**Statut**: ✅ Compilable (102+ erreurs corrigées)  
**Fortnite Version**: 8.51 (4.23.0-6165369+++Fortnite+Release-8.51)  
**Chapitre**: 1 - Saison 8
