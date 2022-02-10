#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "GameMechanics.h"
#include "GameMechanicsUtil.h"
#include "JsonSerializeUtil.h"
#include "TetradFinder.h"

#include <algorithm>
#include <cmath>


namespace Implbits {

class MoveResult : public IMoveResult
{
public:
    MoveResult()
    {}
    MoveResult( const BoardDirection& dir, const int dist, const std::vector< Position >& positions ) :
        m_dir( dir ),
        m_dist( dist ),
        m_positions( positions ),
        m_result()
    {}
    virtual ~MoveResult()
    {}
    virtual void Flip()
    {
        m_dir = FlipBoardDirection( m_dir );
        std::for_each( std::begin( m_positions ), std::end( m_positions ), []( Position& pos )
            {
                pos = FlipPosition( pos );
            } );
        m_result = FlipTetradResult( m_result );
    }
    virtual BoardDirection GetDirection() const
    {
        return m_dir;
    }
    virtual int GetDistance() const
    {
        return m_dist;
    }
    virtual std::vector< Position > GetPositions() const
    {
        return m_positions;
    }
    virtual bool HasTetradForTeam( const PlayerTeam team ) const
    {
        if ( team == TEAM_HOME )
            return m_result.home != TETRAD_END;
        else
            return m_result.away != TETRAD_END;
    }
    virtual TetradShape GetTetradForTeam( const PlayerTeam team ) const
    {
        if ( team == TEAM_HOME )
            return m_result.home;
        else
            return m_result.away;
    }
    virtual ShapeSet GetTetradShapeSetForTeam( const PlayerTeam team ) const
    {
        if ( team == TEAM_HOME )
            return m_result.homeSet;
        else
            return m_result.awaySet;
    }
    virtual json_spirit::mObject JsonEncode() const
    {
        json_spirit::mObject obj;
        obj.insert( json_spirit::mObject::value_type( "direction", m_dir ) );
        obj.insert( json_spirit::mObject::value_type( "distance", m_dist ) );
        json_spirit::mArray arr;
        std::for_each( std::begin( m_positions ), std::end( m_positions ), [&arr]( const Position& pos )
            {
                arr.push_back( JsonEncodePosition( pos ) );
            } );
        obj.insert( json_spirit::mObject::value_type( "positions", arr ) );
        obj.insert( json_spirit::mObject::value_type( "tetradresult", JsonEncodeTetradResult( m_result ) ) );
        return obj;
    }
    virtual void JsonDecode( json_spirit::mObject& json )
    {
        m_dir = static_cast< BoardDirection >( json["direction"].get_int() );
        m_dist = json["distance"].get_int();
        json_spirit::mArray arr = json["positions"].get_array();
        std::transform( std::begin( arr ), std::end( arr ), std::back_inserter( m_positions ),
            [&]( json_spirit::mValue& o ) -> Position
                {
                    return JsonDecodePosition( o.get_obj() );
                } );
        m_result = JsonDecodeTetradResult( json["tetradresult"].get_obj() );
    }
    void SetTetradResult( const TetradResult& result )
    {
        m_result = result;
    }
private:
    BoardDirection m_dir;
    int m_dist;
    std::vector< Position > m_positions;
    TetradResult m_result;
};
class Move : public IMove
{
public:
    Move()
    {}
    Move( const PositionState who, const Position& from, const Position& to, const IMoveResult::Ptr& result ) :
        m_who( who ),
        m_from( from ),
        m_to( to ),
        m_result( result )
    {}
    virtual void Flip()
    {
        m_who = FlipPositionState( m_who );
        m_from = FlipPosition( m_from );
        m_to = FlipPosition( m_to );
        m_result->Flip();
    }
    virtual PositionState GetWho() const
    {
        return m_who;
    }
    virtual Position GetFrom() const
    {
        return m_from;
    }
    virtual Position GetTo() const
    {
        return m_to;
    }
    virtual IMoveResult::Ptr GetResult() const
    {
        return m_result;
    }
    virtual json_spirit::mObject JsonEncode() const
    {
        json_spirit::mObject obj;
        obj.insert( json_spirit::mObject::value_type( "who", m_who ) );
        obj.insert( json_spirit::mObject::value_type( "from", JsonEncodePosition( m_from ) ) );
        obj.insert( json_spirit::mObject::value_type( "to", JsonEncodePosition( m_to ) ) );
        obj.insert( json_spirit::mObject::value_type( "result", m_result->JsonEncode() ) );
        return obj;
    }
    virtual void JsonDecode( json_spirit::mObject& json )
    {
        m_who = static_cast< PositionState >( json["who"].get_int() );
        m_from = JsonDecodePosition( json["from"].get_obj() );
        m_to = JsonDecodePosition( json["to"].get_obj() );
        m_result = std::make_shared< MoveResult >();
        m_result->JsonDecode( json["result"].get_obj() );
    }
private:
    PositionState m_who;
    Position m_from;
    Position m_to;
    IMoveResult::Ptr m_result;
};

IMove::Ptr JsonDecodeMove( json_spirit::mObject& json )
{
    IMove::Ptr mv = std::make_shared< Move >();
    mv->JsonDecode( json );
    return mv;
}

static std::vector< Position > GetValidPositionsFromTargets( const std::vector< Target >& targets )
{
    std::vector< Position > ret;
    std::for_each( std::begin( targets ), std::end( targets ), [&ret]( const Target& t )
        {
            if ( t.valid )
                ret.push_back( t.pos );
        } );
    return ret;
}

GameMechanics::Ptr GameMechanics::Instance()
{
    static auto inst = GameMechanics::Ptr( new GameMechanics() );
    return inst;
}
GameMechanics::GameMechanics()
{}
GameMechanics::~GameMechanics()
{}
IMove::Ptr GameMechanics::GetMove( const GameBoard::ConstPtr& board, const PositionState who, const Position& from, const Position& to ) const
{
    IMove::Ptr ret;
    IMoveResult::Ptr result = GetMoveResult( board, from, to );
    if ( result )
    {
        ret = std::make_shared< Move >( who, from, to, result );
    }
    return ret;
}
std::vector< IMove::Ptr > GameMechanics::GetValidMoves( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove, const PositionState who ) const
{
    std::vector< IMove::Ptr > moves;
    for ( GameBoardIterator from = std::begin( *board ); from != std::end( *board ); from++ )
    {
        auto tos = GetTargets( board, *from, lastmove );
        PositionState who = board->GetPositionState( *from );
        for ( auto to = std::begin( tos ); to != std::end( tos ); to++ )
        {
            if ( to->valid )
                moves.push_back( std::make_shared< Move >( who, *from, to->pos, GetMoveResult( board, *from, to->pos ) ) );
        }
    }
    // This is where you turn off filtering moves
    FilterMovesForPlayer( moves, who );
    return moves;
}
std::vector< Target > GameMechanics::GetTargets( const GameBoard::ConstPtr& board, const Position& from, const IMove::Ptr& lastmove ) const
{
    std::vector< Target > ret;
    std::vector< Position > tmp = GetTargetsPositions( board, from , lastmove );
    std::transform( std::begin( tmp ), std::end( tmp ), std::back_inserter( ret ), [&lastmove, &from, &board, this]( Position& pos ) -> Target
        {
            Target t = { pos, true };
            if ( lastmove && IsUndoingLastMove( lastmove->GetResult(), GetMoveResult( board, from, pos ) ) )
                t.valid = false;
            return t;
        } );
    return ret;
}
void GameMechanics::ApplyMove( GameBoard::Ptr& board, const IMove::Ptr& move ) const
{
    auto result = move->GetResult();
    if ( result )
    {
        ApplyMoveResult( board, result );
    }
}
std::vector< Position > GameMechanics::GetTargetsPositions( const GameBoard::ConstPtr& board, const Position& from, const IMove::Ptr& lastmove ) const
{
    std::vector< Position > ret;
    PositionState who = board->GetPositionState( from );
    if ( who == STATE_HOME || who == STATE_AWAY )
    {
        GetDirectionalMoves( board, from, DIR_LEFT, ret );
        GetDirectionalMoves( board, from, DIR_RIGHT, ret );
        GetDirectionalMoves( board, from, DIR_UP, ret );
        GetDirectionalMoves( board, from, DIR_DOWN, ret );
    }
    return ret;
}
void GameMechanics::GetDirectionalMoves( const GameBoard::ConstPtr& board, const Position& pos, const BoardDirection& dir, std::vector< Position >& ret ) const
{
    int cnt = GetSignificantEmptySpacesCount( board, pos, dir );
    Position tmp = pos;
    for ( int i = 0; i < cnt; i++ )
    {
        tmp = IterateInDirection( tmp, dir );
        ret.push_back( tmp );
    }
}
int GameMechanics::GetSignificantEmptySpacesCount( const GameBoard::ConstPtr& board, const Position& pos, const BoardDirection& dir ) const
{
    int ret = 0;
    for ( Position p = IterateInDirection( pos, dir ); board->GetPositionState( p ) != STATE_INVALID; p = IterateInDirection( p, dir ) )
    {
        PositionState s = board->GetPositionState( p );
        if ( s == STATE_HOME || s == STATE_AWAY )
        {
            if ( ret > 0 )
            break;
        }
        else if ( s == STATE_EMPTY )
        {
            ret++;
        }
    }
    return ret;
}
bool GameMechanics::IsUndoingLastMove( const IMoveResult::Ptr& lastmove, const IMoveResult::Ptr& thismove ) const
{
    if ( ! lastmove )
        return false;
    auto lastdir = lastmove->GetDirection();
    auto thisdir = thismove->GetDirection();
    if ( lastdir == DIR_INVALID || thisdir == DIR_INVALID )
        return false;
    // This condition to determine if the directions are opposite is completely dependent on not changing the values in the BoardDirection enumeration
    if ( ( abs( lastdir - thisdir ) != 1 ) || ( lastdir * thisdir ) % 10 != 2 )
        return false;
    int lastdist = lastmove->GetDistance();
    if ( lastdist != thismove->GetDistance() )
        return false;
    auto lastpositions = lastmove->GetPositions();
    auto thispositions = thismove->GetPositions();
    if ( lastpositions.size() != thispositions.size() )
        return false;
    // Actually move all the positions from the first move result into resulting locations so that
    // we can see if they are the same positions being moved back by the second move result
    std::for_each( std::begin( lastpositions ), std::end( lastpositions ), [&]( Position& p )
        {
            for ( int i = 0; i < lastdist; i++ )
                p = IterateInDirection( p, lastdir );
        } );
    for ( auto lastpit = std::begin( lastpositions ); lastpit != std::end( lastpositions ); lastpit++ )
    {
        if ( ! std::any_of( std::begin( thispositions ), std::end( thispositions ), [&]( Position& thisp ) -> bool
            {
                Position lastp = *lastpit;
                return lastp == thisp;
            } ) )
            return false;
    }
    return true;
}
IMoveResult::Ptr GameMechanics::GetMoveResult( const GameBoard::ConstPtr& board, const Position& from, const Position& to ) const
{
    auto cpy = board->Copy();
    PositionState f = cpy->GetPositionState( from );
    PositionState t = cpy->GetPositionState( to );
    if ( from != to && ( f == STATE_HOME || f == STATE_AWAY ) )
    {
        auto targets = this->GetTargets( cpy, from, IMove::Ptr() );
        auto valids = GetValidPositionsFromTargets( targets );
        std::sort( std::begin( valids ), std::end( valids ) );
        if ( std::find( std::begin( valids ), std::end( valids ), to ) != std::end( valids ) )
        {
            BoardDirection dir = GetBoardDirection( from, to );
            int dist = GetLinearDistance( cpy, from, to, dir );
            std::vector< Position > mv;
            Position p = from;
            while ( cpy->GetPositionState( p ) != STATE_INVALID
                    && cpy->GetPositionState( p ) != STATE_EMPTY )
            {
                mv.push_back( p );
                p = IterateInDirection( p, dir );
            }
            auto moveresult = std::make_shared< MoveResult >( dir, dist, mv );
            this->ApplyMoveResult( cpy, moveresult );
            moveresult->SetTetradResult( TetradFinder::Instance()->FindTetrads( cpy ) );
            return moveresult;
        }
    }
    return IMoveResult::Ptr();
}
int GameMechanics::GetLinearDistance( const GameBoard::ConstPtr& board, const Position& from, const Position& to, const BoardDirection& dir ) const
{
    int ret = 0;
    for ( Position p = from; board->GetPositionState( p ) != STATE_INVALID; p = IterateInDirection( p, dir ) )
    {
        if ( p == to )
            return ret;
        ret++;
    }
    throw LinearDistanceException( from, to, dir );
}
void GameMechanics::FilterMovesForPlayer( std::vector< IMove::Ptr >& moves, const PositionState who ) const
{
    for ( auto move = std::begin( moves ); move != std::end( moves ); )
    {
        if ( ( *move )->GetWho() == who )
            move++;
        else
            move = moves.erase( move );
    }
}
void GameMechanics::ApplyMoveResult( GameBoard::Ptr& board, const IMoveResult::Ptr& result ) const
{
    auto mv = result->GetPositions();
    for ( std::vector< Position >::reverse_iterator it = mv.rbegin(); it != mv.rend(); it++ )
    {
        Position swap = *it;
        for ( int i = 0; i < result->GetDistance(); i++ )
            swap = IterateInDirection( swap, result->GetDirection() );
        board->SetPositionState( swap, board->GetPositionState( *it ) );
        board->SetPositionState( *it, STATE_EMPTY );
    }
}

} // Implbits
