#pragma once
#include "Definitions.h"
#include "Exceptions.h"

#include <memory>
#include <set>


namespace Implbits {

BoardDirection GetBoardDirection( const Position& from, const Position& to );
Position IterateInDirection( const Position& pos, const BoardDirection dir );
Position IterateInDirection( const Position& pos, const BoardDirection dir, const int dist );
Position FlipPosition( const Position& pos );
PositionState FlipPositionState( const PositionState state );
BoardDirection FlipBoardDirection( const BoardDirection dir );
TetradShape FlipTetradShape( const TetradShape s );
std::set< TetradShape > FlipSetOfTetradShape( const std::set< TetradShape >& sots );
TetradResult FlipTetradResult( const TetradResult& result );
PlayerTeam FlipTeam( const PlayerTeam team );

} // Implbits
