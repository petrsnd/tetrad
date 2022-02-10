#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "GameMechanicsUtil.h"
#include "GameBoard.h"

#include <algorithm>


namespace Implbits {

BoardDirection GetBoardDirection( const Position& from, const Position& to )
{
    if ( from.x == to.x )
    {
        if ( from.y > to.y )
            return DIR_UP;
        else if ( from.y < to.y )
            return DIR_DOWN;
    }
    else if ( from.y == to.y )
    {
        if ( from.x > to.x )
            return DIR_LEFT;
        else if ( from.x < to.x )
            return DIR_RIGHT;
    }
    return DIR_INVALID;
}
Position IterateInDirection( const Position& pos, const BoardDirection dir )
{
    switch ( dir )
    {
    case DIR_LEFT:
        return Position( pos.x - 1, pos.y );
    case DIR_RIGHT:
        return Position( pos.x + 1, pos.y );
    case DIR_UP:
        return Position( pos.x, pos.y - 1 );
    case DIR_DOWN:
        return Position( pos.x, pos.y + 1 );
    case DIR_INVALID:
    default:
        throw InvalidPositionIterationException();
    }
}
Position IterateInDirection( const Position& pos, const BoardDirection dir, const int dist )
{
    Position ret = pos;
    for ( int i = 0; i < dist; i++ )
    {
        ret = IterateInDirection( ret, dir );
    }
    return ret;
}
Position FlipPosition( const Position& pos )
{
    Position ret;
    ret.y = pos.y;
    // f(x) = { 0 -> 3
    //          1 -> 2
    //          2 -> 1
    //          3 -> 0 }
    ret.x = GameBoard::SizeX() - 1;
    for ( int x = 0; x < pos.x; x++ )
        ret.x--;
    return ret;
}
PositionState FlipPositionState( const PositionState state )
{
    if ( state == STATE_HOME )
        return STATE_AWAY;
    else if ( state == STATE_AWAY )
        return STATE_HOME;
    else
        throw UnexpectedException( "Cannot flip invalid position state" );
}
BoardDirection FlipBoardDirection( const BoardDirection dir )
{
    switch ( dir )
    {
    case DIR_LEFT:
        return DIR_RIGHT;
    case DIR_RIGHT:
        return DIR_LEFT;
    case DIR_UP:
    case DIR_DOWN:
    default:
        return dir;
    }
}
TetradShape FlipTetradShape( const TetradShape s )
{
    switch ( s )
    {
    case TETRAD_SQUARE:
        return TETRAD_SQUARE;
    case TETRAD_LINE:
        return TETRAD_LINE;
    case TETRAD_PODIUM:
        return TETRAD_PODIUM;
    // flip mirror image pieces
    case TETRAD_BACKWARDSL:
        return TETRAD_LSHAPE;
    case TETRAD_LSHAPE:
        return TETRAD_BACKWARDSL;
    case TETRAD_DOWNSTEP:
        return TETRAD_UPSTEP;
    case TETRAD_UPSTEP:
        return TETRAD_DOWNSTEP;
    case TETRAD_END:
        return TETRAD_END;
    }
    throw UnexpectedException( "Cannot flip unknown Tetrad Shape" );
}
std::set< TetradShape > FlipSetOfTetradShape( const std::set< TetradShape >& sots )
{
    std::set< TetradShape > ret;
    std::transform( std::begin( sots ), std::end( sots ), std::inserter( ret, std::end( ret ) ), []( const TetradShape s ) -> TetradShape
        {
            return FlipTetradShape( s );
        } );
    return ret;
}
TetradResult FlipTetradResult( const TetradResult& result )
{
    TetradResult ret = result;
    std::swap( ret.home, ret.away );
    ret.home = FlipTetradShape( ret.home );
    ret.away = FlipTetradShape( ret.away );
    std::swap( ret.homeSet, ret.awaySet );
    std::for_each( std::begin( ret.homeSet ), std::end( ret.homeSet ), []( Position& pos )
        {
            pos = FlipPosition( pos );
        } );
    std::for_each( std::begin( ret.awaySet ), std::end( ret.awaySet ), []( Position& pos )
        {
            pos = FlipPosition( pos );
        } );
    return ret;
}
PlayerTeam FlipTeam( const PlayerTeam team )
{
    if ( team == TEAM_HOME )
        return TEAM_AWAY;
    else
        return TEAM_HOME;
}

} // Implbits
