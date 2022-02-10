#pragma once

namespace TsWinRT
{

public enum class TsPlayerType
{
    TsPlayerLocal,
    TsPlayerComputer,
    TsPlayerRemote
};

public enum class TsPlayerTeam
{
    TsTeamHome,
    TsTeamAway
};

public enum class TsBoardDirection
{
    TsDirLeft,
    TsDirRight,
    TsDirUp,
    TsDirDown,
    TsDirInvalid
};

public enum class TsTetradShape
{
    TsTetradSquare,
    TsTetradLine,
    TsTetradBackwardsL,
    TsTetradLShape,
    TsTetradDownStep,
    TSTetradUpStep,
    TsTetradPodium
};

} // TsWinRT
