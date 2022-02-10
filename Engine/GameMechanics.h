#pragma once
#include "Definitions.h"
#include "GameBoard.h"
#include "Move.h"

#include <memory>
#include <vector>


namespace Implbits {

class GameMechanics
{
public:
    typedef std::shared_ptr< GameMechanics > Ptr;
    static GameMechanics::Ptr Instance();
    ~GameMechanics();
    IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const PositionState who, const Position& from, const Position& to ) const;
    std::vector< IMove::Ptr > GetValidMoves( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove, const PositionState who ) const;
    std::vector< Target > GetTargets( const GameBoard::ConstPtr& board, const Position& from, const IMove::Ptr& lastmove ) const;
    void ApplyMove( GameBoard::Ptr& board, const IMove::Ptr& move ) const;
private:
    GameMechanics();
    std::vector< Position > GetTargetsPositions( const GameBoard::ConstPtr& board, const Position& from, const IMove::Ptr& lastmove ) const;
    void GetDirectionalMoves( const GameBoard::ConstPtr& board, const Position& pos, const BoardDirection& dir, std::vector< Position >& ret ) const;
    int GetSignificantEmptySpacesCount( const GameBoard::ConstPtr& board, const Position& pos, const BoardDirection& dir ) const;
    bool IsUndoingLastMove( const IMoveResult::Ptr& moveresult, const IMoveResult::Ptr& lastmove ) const;
    IMoveResult::Ptr GetMoveResult( const GameBoard::ConstPtr& board, const Position& from, const Position& to ) const;
    int GetLinearDistance( const GameBoard::ConstPtr& board, const Position& from, const Position& to, const BoardDirection& dir ) const;
    void FilterMovesForPlayer( std::vector< IMove::Ptr >& moves, const PositionState who ) const;
    void ApplyMoveResult( GameBoard::Ptr& board, const IMoveResult::Ptr& result ) const;
};

} // Implbits
