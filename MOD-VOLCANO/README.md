# MOD-VOLCANO: Enhanced Fortnite 8.51 Server with AI Bots

![MOD-VOLCANO Logo](https://img.shields.io/badge/MOD-VOLCANO-red?style=for-the-badge)
![Version](https://img.shields.io/badge/Version-8.51--MOD--VOLCANO-yellow?style=for-the-badge)
![Build Status](https://img.shields.io/badge/Build-0.1.0--Alpha-green?style=for-the-badge)

## 🎯 Project Goal

**MOD-VOLCANO** est une intégration avancée de VOLCANO 8.51 avec le système de bots SPECTRA, créant un serveur Fortnite 8.51 Chapitre 1 Saison 8 amélioré avec des capacités d'IA complètes.

### ✨ Features

- **🤖 Système de Bots IA Avancé**
  - Bots AIS avec comportement intelligent
  - Player Bots avec logique de jeu complexe
  - Système de loot automatique
  - Combat et stratégie adaptatifs

- **🔧 Fonctionnalités VOLCANO Intégrées**
  - Toutes les fonctionnalités de VOLCANO 8.51 préservées
  - Hooks et modifications du jeu originales
  - Système de farming automatisé
  - Gestion d'inventaire améliorée

- **⚡ Architecture Moderne**
  - Code source complet fourni
  - Solution Visual Studio (.SLN) incluse
  - Compilation optimisée x64
  - Documentation complète

## 📁 Structure du Projet

```
MOD-VOLCANO/
├── MOD-VOLCANO.sln              # Solution Visual Studio principale
├── MOD-VOLCANO/MOD-VOLCANO.vcxproj # Projet principal
├── dllmain.cpp                   # Point d'entrée DLL avec intégration bot
├── framework.h                   # Framework étendu avec bot system
├── Bots/
│   ├── AIS/                      # Système de Bots AIS
│   │   ├── AIController.h        # Contrôleur IA
│   │   ├── ServerBotManager.h    # Gestionnaire de bots serveur
│   │   ├── Bots.h               # Logique des bots
│   │   └── BotNames.h           # Noms des bots
│   └── PlayerBot/
│       └── PlayerBots.h          # Logique des Player Bots
├── SDK/                          # SDK Fortnite 8.51
├── Volcano-8.51/                 # Code source VOLCANO original
└── README.md                     # Documentation
```

## 🛠️ Installation & Compilation

### Prérequis

- Visual Studio 2022 ou 2019
- Windows 10/11 SDK
- Kit de développement Fortnite (optionnel)

### Étapes de Compilation

1. **Cloner le projet**
   ```bash
   git clone <repository-url>
   cd MOD-VOLCANO
   ```

2. **Ouvrir la solution**
   ```bash
   start MOD-VOLCANO.sln
   ```

3. **Configurer le projet**
   - Assurez-vous que la configuration Release x64 est sélectionnée
   - Vérifiez les dépendances dans Properties > Linker

4. **Compiler**
   ```bash
   # Dans Visual Studio : Build > Build Solution
   # Ou via MSBuild :
   msbuild MOD-VOLCANO.sln /p:Configuration=Release /p:Platform=x64
   ```

5. **DLL générée**
   - Location: `x64/Release/MOD-VOLCANO.dll`

## 🎮 Fonctionnalités des Bots

### 🤖 Bots AIS

Les bots AIS (Artificial Intelligence System) incluent :

- **Navigation intelligente** avec pathfinding avancé
- **Réactions adaptatives** aux situations de combat
- **Gestion automatique** de l'inventaire et du loot
- **Comportements d'équipe** coordonnés

### 🎯 Player Bots

Les Player Bots simulent des joueurs humains avec :

- **Logique de loot sophistiquée**
  - Priorisation intelligente des objets
  - Calcul des distances optimales
  - Évasion des conflits si nécessaire

- **Combat adaptatif**
  - Évitement et positionnement tactique
  - Utilisation intelligente des abilities
  - Gestion de la santé et des boucliers

- **Construction et édition**
  - Structures défensives automatiques
  - Modifications de build en temps réel
  - Adaptation aux situations de combat

## 🔧 Configuration

Les paramètres des bots peuvent être ajustés dans `framework.h` :

```cpp
namespace Globals
{
    // Bot System Configuration
    bool bBotSystemEnabled = true;
    int BotCount = 50; // Nombre de bots à générer
    bool bBotLooting = true;
    bool bBotCombat = true;
    bool bBotBuilding = true;
}
```

## 🏗️ Intégration

MOD-VOLCANO combine :

1. **VOLCANO 8.51 Base** - Serveur modifié original
2. **SPECTRA Bot System** - Intégration complète des bots
3. **Adaptation 8.51** - Portage des fonctionnalités 10.40 vers 8.51

### Hooks Intégrés

- `ServerBotManager::SpawnBot` - Gestion spawn des bots
- `AIController::OnActorBump` - Réactions collision
- Tous les hooks VOLCANO originaux préservés

## ⚠️ Important

- **Usage Éducatif** : Ce projet est destiné à des fins éducatives
- **EULA Epic Games** : Respectez les conditions d'utilisation
- **Sécurité** : Utilisez uniquement sur vos propres serveurs
- **Compatibilité** : Conçu pour Fortnite 8.51 Chapitre 1 Saison 8

## 📝 Changelog

### Version 0.1.0-Alpha
- ✅ Intégration VOLCANO 8.51 + SPECTRA
- ✅ Système de bots AIS fonctionnel
- ✅ Player Bots avec IA avancée
- ✅ Compilation réussie x64
- ✅ Solution Visual Studio complète

## 🤝 Contribution

Les contributions sont les bienvenues ! Pour contribuer :

1. Fork le projet
2. Créer une branche feature (`git checkout -b feature/AmazingFeature`)
3. Commit les changements (`git commit -m 'Add some AmazingFeature'`)
4. Push la branche (`git push origin feature/AmazingFeature`)
5. Ouvrir une Pull Request

## 📄 Licence

Ce projet est sous licence MIT. Voir `LICENSE` pour plus de détails.

## 🏆 Remerciements

- **VOLCANO Team** - Pour la base serveur 8.51
- **SPECTRA Team** - Pour le système de bots
- **Communauté Fortnite** - Pour le reverse engineering
- **Project Reboot** - Pour l'inspiration

---

**MOD-VOLCANO** - *When AI meets Creative Servers* 🎮✨