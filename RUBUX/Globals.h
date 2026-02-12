#pragma once

#include <string>

namespace Globals
{
    inline std::string PlaylistName = "Playlist_DefaultSolo.Playlist_DefaultSolo";
    inline bool bLategame = true;

    inline bool bBotsEnabled = true;
    inline int MaxBotsToSpawn = 100;
    inline int MinPlayersForEarlyStart = 95;

    inline int NextTeamIndex = 0;
    inline int CurrentPlayersOnTeam = 0;
    inline int MaxPlayersPerTeam = 1;

    inline int NextBotTeamIndex = 0;
    inline int CurrentBotsOnTeam = 0;
}
