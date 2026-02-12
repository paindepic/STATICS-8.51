#pragma once
#include <string>

namespace Globals {
    inline bool bIsProdServer = false;

    inline bool bCreativeEnabled = false;
    inline bool bSTWEnabled = false;
    inline bool bEventEnabled = false;
    inline bool bLategame = false;  // Added missing variable

    inline bool bBotsEnabled = true; // I think you can have bots in this version, not sure yet.

    inline int MaxBotsToSpawn = 100;
    inline int MinPlayersForEarlyStart = 95;

    //REAL PLAYERS
    inline static int NextTeamIndex = 0;
    inline static int CurrentPlayersOnTeam = 0;
    inline static int MaxPlayersPerTeam = 1;

    //BOTS PLAYERS
    inline static int NextBotTeamIndex = 0;
    inline static int CurrentBotsOnTeam = 0;

    // Playlist name for gaymode.h
    inline std::wstring PlaylistName = L"/Game/Athena/Playlists/Playlist_DefaultDuo.Playlist_DefaultDuo";
}
