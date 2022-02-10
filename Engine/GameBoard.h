#pragma once
#include "Exceptions.h"
#include "Definitions.h"

#include <iterator>
#include <memory>


namespace Implbits {

class GameBoardIterator;

class GameBoard : public std::enable_shared_from_this< GameBoard >
{
    friend class GameBoardIterator;
public:
    typedef std::shared_ptr< GameBoard > Ptr;
    typedef std::shared_ptr< const GameBoard > ConstPtr;
    typedef GameBoardIterator const_iterator;
    static GameBoard::Ptr Create();
    static GameBoard::Ptr Create( const unsigned int encoding );
    GameBoard::Ptr Copy() const;
    void SetPositionState( const Position& pos, PositionState ps );
    PositionState GetPositionState( const Position& pos ) const;
    unsigned int Encode() const;
    GameBoardIterator begin() const;
    GameBoardIterator end() const;
    static int SizeX();
    static int SizeY();
    static Position Origin();
private:
    GameBoard();
    int GetHomeBitmapIndex( int x, int y );
    int GetPlayerBitmapIndex( int x, int y ) const;
    int GetEmptyBitmapIndex( int x, int y ) const;
    static PositionState CalculatePositionState( int x, int y, const unsigned int encoding );
    bool IsOutOfBounds( const Position& pos ) const;
    PositionState m_board[4][4];
};

class GameBoardIterator : public std::iterator< std::forward_iterator_tag, Position, Position, Position*, Position& >
{
public:
    GameBoardIterator( const GameBoard::ConstPtr& gb );
    GameBoardIterator( const GameBoard::ConstPtr& gb, const Position& p );
    GameBoardIterator( const GameBoardIterator& gbit );
    GameBoardIterator& operator++();
    GameBoardIterator operator++( int );
    bool operator==( const GameBoardIterator& rhs ) const;
    bool operator!=( const GameBoardIterator& rhs ) const;
    Position& operator*();
private:
    GameBoard::ConstPtr m_gb;
    Position m_p;
};

} // Implbits
