#pragma once
#include <array>
#include <iterator>
#include <cassert>


namespace Implbits {

typedef enum {
    PLAYER_LOCAL,
    PLAYER_COMPUTER,
    PLAYER_REMOTE
} PlayerType;

typedef enum {
    TEAM_HOME,
    TEAM_AWAY
} PlayerTeam;

typedef enum {
    STATE_HOME,
    STATE_AWAY,
    STATE_EMPTY,
    STATE_INVALID
} PositionState;

// _BEGIN and _END are for iteration
// Don't change the values or it will break the IsUndo algorithm in GameEngine.cpp
typedef enum {
    DIR_BEGIN = 1,
    DIR_LEFT = DIR_BEGIN,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
    DIR_INVALID,
    DIR_END = DIR_INVALID
} BoardDirection;

BoardDirection& operator++( BoardDirection& d );
BoardDirection operator++( BoardDirection& d, int );
BoardDirection operator*( BoardDirection d );

 // _BEGIN and _END are for iteration
typedef enum {
    TETRAD_BEGIN = 1,
    TETRAD_SQUARE = TETRAD_BEGIN,
    TETRAD_LINE,
    TETRAD_BACKWARDSL,
    TETRAD_LSHAPE,
    TETRAD_DOWNSTEP,
    TETRAD_UPSTEP,
    TETRAD_PODIUM,
    TETRAD_END
} TetradShape;


TetradShape& operator++( TetradShape& s );
TetradShape operator++( TetradShape& s, int );
TetradShape operator*( TetradShape s );

struct Position
{
    inline Position() :
        x( -1 ), y( -1 )
    {}
    inline Position( int x_, int y_ ) :
        x( x_ ), y( y_ )
    {}
    // implicit copy/move constructors are already inline
    inline bool operator==( const Position& other ) const
    {
        return this->x == other.x && this->y == other.y;
    }
    inline bool operator!=( const Position& other ) const
    {
        return ! ( ( *this ) == other );
    }
    inline bool operator<( const Position& other ) const
    {
        // we want to sort in rows
        if ( this->y < other.y )
            return true;
        else if ( this->y > other.y )
            return false;
        return this->x < other.x;
    }
    int x;
    int y;
};

struct Target
{
    Position pos;
    bool valid;
};

typedef std::array< Position, 4 > ShapeSet;

struct TetradResult
{
    TetradResult() :
        home( TETRAD_END ),
        away( TETRAD_END )
    {}
    TetradShape home;
    ShapeSet homeSet;
    TetradShape away;
    ShapeSet awaySet;
};

} // Implbits

namespace std {

template<>
struct iterator_traits< Implbits::BoardDirection >
{
    typedef Implbits::BoardDirection value_type;
    typedef int difference_type;
    typedef Implbits::BoardDirection* pointer;
    typedef Implbits::BoardDirection& reference;
    typedef std::forward_iterator_tag iterator_category;
};

template<>
struct iterator_traits< Implbits::TetradShape >
{
    typedef Implbits::TetradShape value_type;
    typedef int difference_type;
    typedef Implbits::TetradShape* pointer;
    typedef Implbits::TetradShape& reference;
    typedef std::forward_iterator_tag iterator_category;
};

} // std
