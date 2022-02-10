#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "Game.h"
#include "GameMechanics.h"
#include "GameMechanicsUtil.h"
#include "JsonSerializeUtil.h"

#include "Base64/Base64.h"

#include <algorithm>

#include <cassert>


namespace Implbits {

static PositionState PlayerTeamToPositionState( const PlayerTeam team )
{
    if ( team == TEAM_HOME )
        return STATE_HOME;
    else
        return STATE_AWAY;
}
static PlayerBase::Ptr GetPlayerFromSpec( const IPlayer::Ptr& p )
{
    if ( p )
    {
        auto ret = PlayerFactory::Instance()->Create( p->GetType(), p->GetTeam(), p->GetIdentity(), p->GetName(), p->GetColor() );
        if ( ret )
            return ret;
    }
    throw UnexpectedException( "Unknown player type" );
}


Tile::Ptr Tile::Create()
{
    return Tile::Ptr( new Tile() );
}
Tile::Ptr Tile::Create( int id, const Position& pos, const PlayerTeam owner, const Game::Ptr& game )
{
    return Tile::Ptr( new Tile( id, pos, owner, game ) );
}
Tile::Tile() :
    m_id( -1 )
{}
Tile::Tile( const int id, const Position& pos, const PlayerTeam owner, const Game::Ptr& game ) :
    m_id( id ),
    m_pos( pos ),
    m_owner( owner ),
    m_game( game )
{}
Tile::~Tile()
{}
void Tile::Flip()
{
    m_pos = FlipPosition( m_pos );
    m_owner = FlipTeam( m_owner );
}
void Tile::SetGame( const std::shared_ptr< Game>& game )
{
    m_game = game;
}
json_spirit::mObject Tile::JsonEncode() const
{
    json_spirit::mObject obj;
    obj.insert( json_spirit::mObject::value_type( "id", m_id ) );
    obj.insert( json_spirit::mObject::value_type( "owner", m_owner ) );
    obj.insert( json_spirit::mObject::value_type( "pos", JsonEncodePosition( m_pos ) ) );
    return obj;
}
void Tile::JsonDecode( json_spirit::mObject& json )
{
    m_id = json["id"].get_int();
    m_owner = static_cast< PlayerTeam >( json["owner"].get_int() );
    m_pos = JsonDecodePosition( json["pos"].get_obj() );
}
int Tile::GetId() const
{
    return m_id;
}
Position Tile::GetPos() const
{
    return m_pos;
}
void Tile::SetPos( const Position& pos )
{
    m_pos = pos;
}
PlayerTeam Tile::GetOwner() const
{
    return m_owner;
}
std::vector< Target > Tile::GetTargets() const
{
    return GameMechanics::Instance()->GetTargets( m_game->GetGameBoard(), m_pos, m_game->GetLastMove() );
}


static PlayerBase::Ptr JsonDecodePlayer( json_spirit::mObject& json, const Game::Ptr& game )
{
    PlayerBase::Ptr player = std::make_shared< PlayerSpec >();
    player->JsonDecode( json );
    player = PlayerFactory::Instance()->RebuildPlayer( player, game );
    return player;
}
static void FlipPlayers( PlayerBase::Ptr& a, PlayerBase::Ptr& b, const Game::Ptr& game )
{
    if ( a->GetType() != PLAYER_REMOTE && b->GetType() != PLAYER_REMOTE )
    {
        std::swap( a, b );
        a->SetTeam( FlipTeam( a->GetTeam() ) );
        b->SetTeam( FlipTeam( b->GetTeam() ) );
    }
    else
    {
        PlayerBase::Ptr c = b;
        b = PlayerFactory::Instance()->RebuildPlayer( a->GetType() == PLAYER_REMOTE ? PLAYER_LOCAL : PLAYER_REMOTE, FlipTeam( a->GetTeam() ),
                                                      a->GetIdentity(), a->GetName(), a->GetColor(), FlipSetOfTetradShape( a->GetTetrads() ), game );
        a = PlayerFactory::Instance()->RebuildPlayer( c->GetType() == PLAYER_REMOTE ? PLAYER_LOCAL : PLAYER_REMOTE, FlipTeam( c->GetTeam() ),
                                                      c->GetIdentity(), c->GetName(), c->GetColor(), FlipSetOfTetradShape( c->GetTetrads() ), game );
    }
}
Game::Ptr Game::Create()
{
    return Game::Ptr( new Game() );
}
Game::Ptr Game::CreateNewGame( const GameIdInitiatorFunc& initId, const IPlayer::Ptr& home, const IPlayer::Ptr& away )
{
    auto ret = Game::Create();

    ret->m_id = initId();

    ret->InitTiles();

    ret->m_home = GetPlayerFromSpec( home );
    ret->m_home->SetGame( ret );
    ret->m_away = GetPlayerFromSpec( away );
    ret->m_away->SetGame( ret );

    ret->m_friendlyName = ret->m_home->GetIdentity() + " vs " + ret->m_away->GetIdentity();

    // game creator goes first
    ret->m_turn = ret->m_home->GetTeam();

    return ret;
}
Game::Ptr Game::LoadGame( const std::string& b64 )
{
    std::string encoding = b64::Base64DecodeAsString( b64 );
    json_spirit::mValue value;
    json_spirit::read( encoding, value );
    auto game = Implbits::Game::Create();
    game->JsonDecode( value.get_obj() );
    return game;
}
Game::Game() :
    m_viewNotifier(),
    m_quit( false )
{}
Game::~Game()
{}
void Game::InitTiles()
{
    m_tiles.push_back( Tile::Create( 0, Position( 2, 0 ), TEAM_AWAY, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 1, Position( 3, 0 ), TEAM_AWAY, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 2, Position( 3, 1 ), TEAM_AWAY, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 3, Position( 3, 2 ), TEAM_AWAY, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 4, Position( 3, 3 ), TEAM_AWAY, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 5, Position( 2, 3 ), TEAM_AWAY, shared_from_this() ) );

    m_tiles.push_back( Tile::Create( 6, Position( 1, 0 ), TEAM_HOME, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 7, Position( 0, 0 ), TEAM_HOME, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 8, Position( 0, 1 ), TEAM_HOME, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 9, Position( 0, 2 ), TEAM_HOME, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 10, Position( 0, 3 ), TEAM_HOME, shared_from_this() ) );
    m_tiles.push_back( Tile::Create( 11, Position( 1, 3 ), TEAM_HOME, shared_from_this() ) );
}
IMove::Ptr Game::DoLocalTurn()
{
    CallDoLocalTurn();
    return m_lastmove;
}
IMove::Ptr Game::DoRemoteTurn()
{
    CallPutGame();
    CallGetGame();
    return m_lastmove;
}
PlayerTeam Game::GetTurn() const
{
    return m_turn;
}
void Game::Resume( const IView::Ptr& view )
{
    m_view = view;
    m_view->GameResumed( this->shared_from_this(), m_turn );
    while ( ! m_quit && ! m_home->HasAllTetrads() && ! m_away->HasAllTetrads() )
    {
        RunTurn();
        if ( ! m_quit )
            ChangeTurn();
    }
    if ( ! m_quit )
    {
        if ( m_home->GetType() == PLAYER_REMOTE
             || m_away->GetType() == PLAYER_REMOTE )
        {
            auto player = GetPlayerForTurn();
            if ( player->GetTeam() == TEAM_HOME )
                CallDeleteGame();
            else
                CallPutGame();
        }
        m_view->StateChanged();
        m_view->GameOver( m_home->HasAllTetrads() ? m_home : m_away );
    }
}
std::string Game::GetSerializer() const
{
    return m_serializer;
}
void Game::Flip()
{
    std::for_each( std::begin( m_tiles ), std::end( m_tiles ), []( Tile::Ptr& tile )
        {
            tile->Flip();
        } );
    FlipPlayers( m_home, m_away, shared_from_this() );
    m_turn = FlipTeam( m_turn );
    if ( m_lastmove )
        m_lastmove->Flip();
}
GameBoard::Ptr Game::GetGameBoard() const
{
	// TODO: put a breakpoint here to see the lockup thing.
    GameBoard::Ptr board = GameBoard::Create();
    std::for_each( std::begin( m_tiles ), std::end( m_tiles ), [&board]( const Tile::Ptr& t )
        {
            board->SetPositionState( t->GetPos(), PlayerTeamToPositionState( t->GetOwner() ) );
        } );
    return board;
}
IMove::Ptr Game::GetLastMove() const
{
    return m_lastmove;
}
PlayerBase::Ptr Game::GetPlayerForTeam( const PlayerTeam team ) const
{
    if ( team == TEAM_HOME )
        return m_home;
    else
        return m_away;
}
json_spirit::mObject Game::JsonEncode() const
{
    json_spirit::mObject obj;
    obj.insert( json_spirit::mObject::value_type( "serializer", m_home->GetIdentity() ) ); // the key to flip / not flip
    obj.insert( json_spirit::mObject::value_type( "id", m_id ) );
    obj.insert( json_spirit::mObject::value_type( "friendlyName", m_friendlyName ) );
    json_spirit::mArray arr;
    std::for_each( std::begin( m_tiles ), std::end( m_tiles ), [&arr]( const Tile::Ptr& t )
        {
            arr.push_back( t->JsonEncode() );
        } );
    obj.insert( json_spirit::mObject::value_type( "tiles", arr ) );
    obj.insert( json_spirit::mObject::value_type( "home", m_home->JsonEncode() ) );
    obj.insert( json_spirit::mObject::value_type( "away", m_away->JsonEncode() ) );
    if ( m_lastmove )
        obj.insert( json_spirit::mObject::value_type( "lastmove", m_lastmove->JsonEncode() ) );
    obj.insert( json_spirit::mObject::value_type( "turn", m_turn ) );
    return obj;
}
void Game::JsonDecode( json_spirit::mObject& json )
{
    m_serializer = json["serializer"].get_str();
    m_id = json["id"].get_str();
    m_friendlyName = json["friendlyName"].get_str();
    json_spirit::mArray arr = json["tiles"].get_array();
    std::transform( std::begin( arr ), std::end( arr ), std::back_inserter( m_tiles ),
        [this]( json_spirit::mValue& t ) -> Tile::Ptr
            {
                Tile::Ptr tile = Tile::Create();
                tile->JsonDecode( t.get_obj() );
                tile->SetGame( this->shared_from_this() );
                return tile;
            } );
    m_home = JsonDecodePlayer( json["home"].get_obj(), shared_from_this() );
    m_away = JsonDecodePlayer( json["away"].get_obj(), shared_from_this() );
    if ( json.find( "lastmove" ) != json.end() )
        m_lastmove = JsonDecodeMove( json["lastmove"].get_obj() );
    m_turn = static_cast< PlayerTeam >( json["turn"].get_int() );
}
std::string Game::GetId() const
{
    return m_id;
}
std::string Game::GetFriendlyName() const
{
    return m_friendlyName;
}
std::vector< ITile::Ptr > Game::GetTiles() const
{
    std::vector< ITile::Ptr > ret;
    std::copy( std::begin( m_tiles ), std::end( m_tiles ), std::back_inserter( ret ) );
    return ret;
}
IPlayer::Ptr Game::GetHomePlayer() const
{
    return m_home;
}
IPlayer::Ptr Game::GetAwayPlayer() const 
{
    return m_away;
}
void Game::LocalTurnComplete( const ITile::Ptr& tile, const Position& target )
{
    m_lastmove = GameMechanics::Instance()->GetMove( GetGameBoard(), PlayerTeamToPositionState( m_turn ), tile->GetPos(), target );
    m_viewNotifier.SetViewFinished();
}
void Game::AnimationComplete()
{
    m_viewNotifier.SetViewFinished();
}

void Game::GetRemoteGameComplete( const std::string& payload )
{
    auto game = LoadGame( payload );
    if ( m_serializer != game->m_serializer )
    {
        game->Flip();
        m_lastmove = game->GetLastMove();
        auto away = game->GetAwayPlayer();
        m_away->SetColor( away->GetColor() );
        m_away->SetName( away->GetName() );
    }
    m_viewNotifier.SetViewFinished();
}

void Game::PutRemoteGameComplete()
{
    m_viewNotifier.SetViewFinished();
}

void Game::DeleteRemoteGameComplete()
{
    m_viewNotifier.SetViewFinished();
}

void Game::Quit()
{
    m_quit = true;
    m_viewNotifier.SetViewFinished();
}

void Game::RunTurn()
{
    m_view->StateChanged();
    PlayerBase::Ptr player = GetPlayerForTurn();
    m_lastmove = player->GetMove( GetGameBoard(), m_lastmove );
    if ( m_quit )
        return;
    if ( player->GetType() != PLAYER_LOCAL )
        CallAnimate();
    UpdateTiles( m_lastmove->GetResult() );
    player->IncrementNumberOfMoves();
    m_view->StateChanged();
    UpdateTetrads( m_lastmove->GetResult() );
}
PlayerBase::Ptr Game::GetPlayerForTurn() const
{
    return GetPlayerForTeam( m_turn );
}
void Game::CallDoLocalTurn()
{
    m_view->DoLocalTurn( this->shared_from_this() );
    m_viewNotifier.WaitForView();
}
void Game::CallPutGame()
{
    m_serializer = this->GetHomePlayer()->GetIdentity();
    std::string encoding = json_spirit::write( this->JsonEncode() );
    std::string payload = b64::Base64EncodeFromString( encoding );
    m_view->PutRemoteGame( this->shared_from_this(), m_id, payload );
    m_viewNotifier.WaitForView();
}
void Game::CallGetGame()
{
    // TODO: This loop is for polling, later to be
    //       replaced by notification of some sort.
    auto wholast = m_lastmove->GetWho();
    do
    {
        m_view->GetRemoteGame( this->shared_from_this(), m_id );
        m_viewNotifier.WaitForView();
    } while ( wholast == m_lastmove->GetWho() && ! m_quit );
}
void Game::CallDeleteGame()
{
    m_view->DeleteRemoteGame( this->shared_from_this(), m_id );
    m_viewNotifier.WaitForView();
}
void Game::CallAnimate()
{
    IMoveResult::Ptr result = m_lastmove->GetResult();
    std::vector< ITile::Ptr > tiles = GetTilesFromPositions( result->GetPositions() );
    m_view->AnimateTurn( this->shared_from_this(), tiles, result->GetDirection(), result->GetDistance() );
    m_viewNotifier.WaitForView();
}
void Game::UpdateTiles( const IMoveResult::Ptr& result )
{
    std::vector< Position > positions = result->GetPositions();
    BoardDirection dir = result->GetDirection();
    int dist = result->GetDistance();
    std::for_each( std::begin( m_tiles ), std::end( m_tiles ), [&positions, &dir, &dist]( Tile::Ptr& t )
        {
            Position pos = t->GetPos();
            for ( std::vector< Position >::const_iterator it = std::begin( positions ); it != std::end( positions ); it++ )
            {
                if ( ( *it ) == pos )
                {
                    Position newpos = IterateInDirection( pos, dir, dist );
                    t->SetPos( newpos );
                }
            }
        } );
}
void Game::UpdateTetrads( const IMoveResult::Ptr& result )
{
    HandleTetradFound( result, TEAM_HOME );
    HandleTetradFound( result, TEAM_AWAY );
}
void Game::HandleTetradFound( const IMoveResult::Ptr& result, const PlayerTeam team )
{
    if ( result->HasTetradForTeam( team ) )
    {
        auto player = GetPlayerForTeam( team );
        auto tetrad = result->GetTetradForTeam( team );
        if ( ! player->HasTetrad( tetrad ) )
        {
            player->AddTetrad( tetrad );
            m_view->TetradFound( this->shared_from_this(), player, tetrad, GetTilesFromPositions( result->GetTetradShapeSetForTeam( team ) ) );
            m_viewNotifier.WaitForView();
        }
    }
}
std::vector< ITile::Ptr > Game::GetTilesFromPositions( const ShapeSet& shapeSet ) const
{
    std::vector< Position > v;
    std::copy( std::begin( shapeSet ), std::end( shapeSet ), std::back_inserter( v ) );
    auto ret = GetTilesFromPositions( v );
    assert( ret.size() == 4 );
    return ret;
}
std::vector< ITile::Ptr > Game::GetTilesFromPositions( const std::vector< Position >& positions ) const
{
    std::vector< ITile::Ptr > ret;
    std::for_each( std::begin( m_tiles ), std::end( m_tiles ), [&ret, &positions]( const Tile::Ptr& t )
        {
            if ( std::any_of( std::begin( positions ), std::end( positions ), [&t]( const Position& pos ) -> bool
                    {
                        return t->GetPos() == pos;
                    } ) )
            {
                ret.push_back( t );
            }
        } );
    assert( ret.size() > 0 );
    return ret;
}
void Game::ChangeTurn()
{
    m_turn = FlipTeam( m_turn );
}

} // Implbits
