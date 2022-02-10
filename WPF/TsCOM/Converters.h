#pragma once
#include <Engine/Interfaces.h>
#include "TsCOM_i.h"
#include "Position.h"
#include "ComHelper.h"

inline PlayerType ConvertImplbitsPlayerTypeToComPlayerType( const Implbits::PlayerType type )
{
    switch ( type )
    {
    case Implbits::PLAYER_LOCAL:
        return Local;
    case Implbits::PLAYER_COMPUTER:
        return Computer;
    case Implbits::PLAYER_REMOTE:
        return Remote;
    }
    throw "TODO: Invalid implbits player type";
}

inline Implbits::PlayerType ConvertComPlayerTypeToImplbitsPlayerType( const PlayerType type )
{
    switch ( type )
    {
    case Local:
        return Implbits::PLAYER_LOCAL;
    case Computer:
        return Implbits::PLAYER_COMPUTER;
    case Remote:
        return Implbits::PLAYER_REMOTE;
    }
    throw "TODO: Invalid com player type";
}

inline Direction ConvertImplbitsDirectionToComDirection( const Implbits::BoardDirection dir )
{
    switch ( dir )
    {
    case Implbits::DIR_LEFT:
        return Left;
    case Implbits::DIR_RIGHT:
        return Right;
    case Implbits::DIR_UP:
        return Up;
    case Implbits::DIR_DOWN:
        return Down;
    default:
        throw "TODO: Invalid Implbits board direction";
    }
}

inline PlayerTeam ConvertImplbitsTeamToComTeam( const Implbits::PlayerTeam team )
{
    switch ( team )
    {
    case Implbits::TEAM_HOME:
        return Home;
    case Implbits::TEAM_AWAY:
        return Away;
    }
    throw "TODO: Team not specified as either TEAM_HOME or TEAM_AWAY";
}

inline Implbits::PlayerTeam ConvertComTeamToImplbitsTeam( const PlayerTeam team )
{
    switch ( team )
    {
    case Home:
        return Implbits::TEAM_HOME;
    case Away:
        return Implbits::TEAM_AWAY;
    }
    throw "TODO: Team not specified as either Home or Away";
}

inline TetradShape ConvertImplbitsShapeToComShape( const Implbits::TetradShape shape )
{
    switch ( shape )
    {
    case Implbits::TETRAD_SQUARE:
        return Square;
    case Implbits::TETRAD_LINE:
        return Line;
    case Implbits::TETRAD_BACKWARDSL:
        return BackwardsL;
    case Implbits::TETRAD_LSHAPE:
        return LShape;
    case Implbits::TETRAD_DOWNSTEP:
        return Downstep;
    case Implbits::TETRAD_UPSTEP:
        return Upstep;
    case Implbits::TETRAD_PODIUM:
        return Podium;
    default:
        throw "TODO: Unknown Implbits shape";
    }
}

inline Implbits::TetradShape ConvertComShapeToImplbitsShape( const TetradShape shape )
{
    switch ( shape )
    {
    case Square:
        return Implbits::TETRAD_SQUARE;
    case Line:
        return Implbits::TETRAD_LINE;
    case BackwardsL:
        return Implbits::TETRAD_BACKWARDSL;
    case LShape:
        return Implbits::TETRAD_LSHAPE;
    case Downstep:
        return Implbits::TETRAD_DOWNSTEP;
    case Upstep:
        return Implbits::TETRAD_UPSTEP;
    case Podium:
        return Implbits::TETRAD_PODIUM;
    case None:
    default:
        throw "TODO: Unknown com shape";
    }
}

inline CComPtr< IPosition > ConvertImplbitsPositionToComPosition( const Implbits::Position& pos )
{
    auto ret = CoCreateIPtr< IPosition, CPosition >();
    HRESULT hr = ret->put_X( ( SHORT )pos.x );
    if ( SUCCEEDED( hr )  )
        hr = ret->put_Y( ( SHORT )pos.y );
    if ( ! SUCCEEDED( hr ) )
        throw "TODO: HR Exception";
    return ret;
}

inline Implbits::Position ConvertComPositionToImplbitsPosition( const CComPtr< IPosition >& pos )
{
    if ( pos != NULL )
    {
        SHORT x, y;
        HRESULT hr = pos->get_X( &x );
        if ( SUCCEEDED( hr ) )
        {
            hr = pos->get_Y( &y );
            if ( SUCCEEDED( hr ) )
                return Implbits::Position( ( int )x, int( y ) );
        }
        throw "TODO: HR Exception";
    }
    throw "TODO: NULL position pointer";
}

template < typename IFaceT, typename ClassT, typename ImplbitsT >
CComPtr< IFaceT > GetCComInterfacePtr( const ImplbitsT& ptr )
{
    auto classPtr = CoCreatePtr< ClassT >();
    classPtr->Init( ptr );
    CComPtr< IFaceT > ret;
    HRESULT hr = classPtr.QueryInterface( &ret );
    if ( SUCCEEDED( hr ) )
        return ret;
    throw "TODO: HR exception";
}

template < typename IFaceT, typename ClassT, typename ImplbitsT >
IFaceT* GetRawComInterface( const ImplbitsT& ptr )
{
    CComPtr< IFaceT > ret = GetCComInterfacePtr< IFaceT, ClassT, ImplbitsT >( ptr );
    return ret.Detach();
}

template < typename IFaceT, typename ClassT, typename ImplbitsT >
HRESULT GetRawComInterfaceNoThrow( const ImplbitsT& ptr, IFaceT** iface )
{
    try
    {
        *iface = GetRawComInterface< IFaceT, ClassT, ImplbitsT >( ptr );
    }
    catch ( ... )
    {
        // TODO: handle HR Exception
        return E_FAIL;
    }
    return S_OK;
}
