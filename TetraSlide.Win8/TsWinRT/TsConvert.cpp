#include "pch.h"
#include "TsConvert.h"

namespace TsWinRT
{

Implbits::PlayerType ConvertToEnginePlayerType( TsPlayerType t )
{
    switch ( t )
    {
    case TsPlayerType::TsPlayerLocal:
        return Implbits::PLAYER_LOCAL;
    case TsPlayerType::TsPlayerComputer:
        return Implbits::PLAYER_COMPUTER;
    case TsPlayerType::TsPlayerRemote:
        return Implbits::PLAYER_REMOTE;
    }
    throw "TODO: Unexpected winrt player type";
}
TsPlayerType ConvertToTsPlayerType( const Implbits::PlayerType t )
{
    switch ( t )
    {
    case Implbits::PLAYER_LOCAL:
        return TsPlayerType::TsPlayerLocal;
    case Implbits::PLAYER_COMPUTER:
        return TsPlayerType::TsPlayerComputer;
    case Implbits::PLAYER_REMOTE:
        return TsPlayerType::TsPlayerRemote;
    }
    throw "TODO: Unexpected engine player type";
}
Implbits::PlayerTeam ConvertToEnginePlayerTeam( TsPlayerTeam t )
{
    switch ( t )
    {
    case TsPlayerTeam::TsTeamHome:
        return Implbits::TEAM_HOME;
    case TsPlayerTeam::TsTeamAway:
        return Implbits::TEAM_AWAY;
    }
    throw "TODO: Unexpected winrt player team";
}
TsPlayerTeam ConvertToTsPlayerTeam( const Implbits::PlayerTeam t )
{
    switch ( t )
    {
    case Implbits::TEAM_HOME:
        return TsPlayerTeam::TsTeamHome;
    case Implbits::TEAM_AWAY:
        return TsPlayerTeam::TsTeamAway;
    }
    throw "TODO: Unexpected engine player team";
}
Implbits::BoardDirection ConvertToEngineBoardDirection( TsBoardDirection d )
{
    switch ( d )
    {
    case TsBoardDirection::TsDirLeft:
        return Implbits::DIR_LEFT;
    case TsBoardDirection::TsDirRight:
        return Implbits::DIR_RIGHT;
    case TsBoardDirection::TsDirUp:
        return Implbits::DIR_UP;
    case TsBoardDirection::TsDirDown:
        return Implbits::DIR_DOWN;
    }
    throw "TODO: Invalid winrt board direction";
}
TsBoardDirection ConvertToTsBoardDirection( const Implbits::BoardDirection d )
{
    switch ( d )
    {
    case Implbits::DIR_LEFT:
        return TsBoardDirection::TsDirLeft;
    case Implbits::DIR_RIGHT:
        return TsBoardDirection::TsDirRight;
    case Implbits::DIR_UP:
        return TsBoardDirection::TsDirUp;
    case Implbits::DIR_DOWN:
        return TsBoardDirection::TsDirDown;
    }
    throw "TODO: Invalid engine board direction";
}
Implbits::TetradShape ConvertToEngineTetradShape( TsTetradShape s )
{
    switch ( s )
    {
    case TsTetradShape::TsTetradSquare:
        return Implbits::TETRAD_SQUARE;
    case TsTetradShape::TsTetradLine:
        return Implbits::TETRAD_LINE;
    case TsTetradShape::TsTetradBackwardsL:
        return Implbits::TETRAD_BACKWARDSL;
    case TsTetradShape::TsTetradLShape:
        return Implbits::TETRAD_LSHAPE;
    case TsTetradShape::TsTetradDownStep:
        return Implbits::TETRAD_DOWNSTEP;
    case TsTetradShape::TSTetradUpStep:
        return Implbits::TETRAD_UPSTEP;
    case TsTetradShape::TsTetradPodium:
        return Implbits::TETRAD_PODIUM;
    }
    throw "TODO: Invalid winrt tetrad shape";
}
TsTetradShape ConvertToTsTetradShape( const Implbits::TetradShape s )
{
    switch ( s )
    {
    case Implbits::TETRAD_SQUARE:
        return TsTetradShape::TsTetradSquare;
    case Implbits::TETRAD_LINE:
        return TsTetradShape::TsTetradLine;
    case Implbits::TETRAD_BACKWARDSL:
        return TsTetradShape::TsTetradBackwardsL;
    case Implbits::TETRAD_LSHAPE:
        return TsTetradShape::TsTetradLShape;
    case Implbits::TETRAD_DOWNSTEP:
        return TsTetradShape::TsTetradDownStep;
    case Implbits::TETRAD_UPSTEP:
        return TsTetradShape::TSTetradUpStep;
    case Implbits::TETRAD_PODIUM:
        return TsTetradShape::TsTetradPodium;
    }
    throw "TODO: Invalid engine tetrad shape";
}

} // TsWinRT
