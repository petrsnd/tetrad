#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "GameBoard.h"


namespace Implbits {

GameBoard::Ptr GameBoard::Create()
{
    return GameBoard::Ptr( new GameBoard() );
}
GameBoard::Ptr GameBoard::Create( const unsigned int encoding )
{
    auto b = Create();
    for ( int x = 0; x < SizeX(); x++ )
        for ( int y = 0; y < SizeY(); y++ )
            b->m_board[x][y] = CalculatePositionState( x, y, encoding );
    return b;
}
GameBoard::Ptr GameBoard::Copy() const
{
    auto cpy = Create();
    for ( int x = 0; x < SizeX(); x++ )
        for ( int y = 0; y < SizeY(); y++ )
            cpy->m_board[x][y] = m_board[x][y];
    return cpy;
}
GameBoard::GameBoard()
{
    for ( int x = 0; x < SizeX(); x++ )
        for ( int y = 0; y < SizeY(); y++ )
            m_board[x][y] = STATE_EMPTY;
}
void GameBoard::SetPositionState( const Position& pos, PositionState ps )
{
    if ( IsOutOfBounds( pos ) )
        throw GameBoardOutOfBoundsException();
    m_board[pos.x][pos.y] = ps;
}
PositionState GameBoard::GetPositionState( const Position& pos ) const 
{
    if ( IsOutOfBounds( pos ) )
        return STATE_INVALID;
    return m_board[pos.x][pos.y]; 
}
GameBoardIterator GameBoard::begin() const
{
    return GameBoardIterator( shared_from_this() );
}
GameBoardIterator GameBoard::end() const
{
    return GameBoardIterator( shared_from_this(), Position( GameBoard::Origin().x, GameBoard::SizeY() ) );
}
int GameBoard::SizeX() 
{ 
    return 4; 
}
int GameBoard::SizeY() 
{ 
    return 4; 
}
Position GameBoard::Origin()
{
    static Position origin( 0, 0 );
    return origin;
}
bool GameBoard::IsOutOfBounds( const Position& pos ) const
{
    if ( pos.x < 0 || pos.x >= SizeX() || pos.y < 0 || pos.y >= SizeY() )
        return true;
    return false;
}
unsigned int GameBoard::Encode() const
{
    unsigned int state = 0;
    for ( int y = 0; y < SizeY(); y++ )
    {
        for ( int x = 0; x < SizeX(); x++ )
        {
            PositionState ps = m_board[x][y];
            switch ( ps )
            {
            case STATE_HOME:
                state |= ( 1 << GetPlayerBitmapIndex( x, y ) );
                break;
            case STATE_AWAY:
                state &= ~( 1 << GetPlayerBitmapIndex( x, y ) );
                break;
            case STATE_EMPTY:
                state |= ( 1 << GetEmptyBitmapIndex( x, y ) );
                break;
            }
        }
    }
    return state;
}
int GameBoard::GetPlayerBitmapIndex( int x, int y ) const
{
    return ( y * SizeX() ) + x  + 16; 
}
int GameBoard::GetEmptyBitmapIndex( int x, int y ) const
{
    return ( y * SizeX() ) + x; 
}
PositionState GameBoard::CalculatePositionState( int x, int y, const unsigned int encoding )
{
    PositionState ret = STATE_HOME;
    int index = ( y * SizeY() ) + x;
    unsigned int mask = ( 1 << index );
    if ( ( encoding & ( mask << 16 ) ) == 0 )
        ret = STATE_AWAY;
    else if ( ( encoding & mask ) != 0 )
        ret = STATE_EMPTY;
    return ret;
}

GameBoardIterator::GameBoardIterator( const GameBoard::ConstPtr& gb ) :
    m_gb( gb ),
    m_p( GameBoard::Origin() )
{}
GameBoardIterator::GameBoardIterator( const GameBoard::ConstPtr& gb, const Position& p ) :
    m_gb( gb ),
    m_p( p )
{}
GameBoardIterator::GameBoardIterator( const GameBoardIterator& gbit ) :
    m_gb( gbit.m_gb ),
    m_p( gbit.m_p )
{}
GameBoardIterator& GameBoardIterator::operator++()
{
    if ( m_gb->IsOutOfBounds( m_p ) )
    {
        m_p.x = GameBoard::Origin().x;
        m_p.y = m_gb->SizeY();
    }
    else
    {
        m_p.x++;
        if ( m_gb->IsOutOfBounds( m_p ) )
        {
            m_p.y++;
            m_p.x = GameBoard::Origin().x;
        }
    }
    return *this;
}
GameBoardIterator GameBoardIterator::operator++( int )
{
    GameBoardIterator tmp( *this ); 
    operator++(); 
    return tmp;
}
bool GameBoardIterator::operator==( const GameBoardIterator& rhs ) const
{
    return m_p == rhs.m_p;
}
bool GameBoardIterator::operator!=( const GameBoardIterator& rhs ) const
{
    return m_p != rhs.m_p;
}
Position& GameBoardIterator::operator*()
{
    return m_p;
}

} // Implbits
