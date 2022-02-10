#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "Player.h"
#include "Game.h"
#include "MoveGenerator.h"
#include "GameMechanicsUtil.h"


namespace Implbits {

PlayerBase::PlayerBase() :
    m_color( 0 ),
    m_tetrads( 0 ),
    m_numberOfMoves( 0 )
{}
PlayerBase::PlayerBase( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color ) :
    m_type( type ),
    m_team( team ),
    m_identity( id ),
    m_name( name ),
    m_color( color ),
    m_tetrads( 0 ),
    m_numberOfMoves( 0 )
{}
PlayerBase::~PlayerBase()
{}
void PlayerBase::SetGame( const std::shared_ptr< Game>& game )
{
    m_game = game;
}
PlayerType PlayerBase::GetType() const
{
    return m_type;
}
void PlayerBase::SetType( const PlayerType type )
{
    m_type = type;
}
std::string PlayerBase::GetIdentity() const
{
    return m_identity;
}
void PlayerBase::SetIdentity( const std::string& id )
{
    m_identity = id;
}
std::string PlayerBase::GetName() const
{
    return m_name;
}
void PlayerBase::SetName( const std::string& name )
{
    m_name = name;
}
PlayerTeam PlayerBase::GetTeam() const
{
    return m_team;
}
void PlayerBase::SetTeam( const PlayerTeam team )
{
    m_team = team;
}
int PlayerBase::GetColor() const
{
    return m_color;
}
void PlayerBase::SetColor( const int color )
{
    m_color = color;
}
bool PlayerBase::HasTetrad( const TetradShape s ) const
{
    return ( m_tetrads & ( 1 << ( s - 1 ) ) ) != 0;
}
void PlayerBase::IncrementNumberOfMoves()
{
    m_numberOfMoves++;
}
int PlayerBase::GetNumberOfMoves() const
{
    return m_numberOfMoves;
}
bool PlayerBase::HasAllTetrads() const
{
    return m_tetrads == 127;
}
void PlayerBase::AddTetrad( const TetradShape s )
{
    m_tetrads |= ( 1 << ( s - 1 ) );
}
std::set< TetradShape > PlayerBase::GetTetrads() const
{
    std::set< TetradShape > tetrads;
    std::for_each( TETRAD_BEGIN, TETRAD_END, [&]( TetradShape s )
        {
            if ( HasTetrad( s ) )
                tetrads.insert( s );
        } );
    return tetrads;
}
void PlayerBase::SetTetrads( const std::set< TetradShape >& shapes )
{
    std::for_each( std::begin( shapes ), std::end( shapes ), [this]( const TetradShape& s )
        {
            AddTetrad( s );
        } );
}
json_spirit::mObject PlayerBase::JsonEncode() const
{
    json_spirit::mObject obj;
    obj.insert( json_spirit::mObject::value_type( "type", m_type ) );
    obj.insert( json_spirit::mObject::value_type( "team", m_team ) );
    obj.insert( json_spirit::mObject::value_type( "identity", m_identity ) );
    obj.insert( json_spirit::mObject::value_type( "name", m_name ) );
    obj.insert( json_spirit::mObject::value_type( "color", m_color ) );
    obj.insert( json_spirit::mObject::value_type( "tetrads", m_tetrads ) );
    obj.insert( json_spirit::mObject::value_type( "numberOfMoves", m_numberOfMoves ) );
    return obj;
}
void PlayerBase::JsonDecode( json_spirit::mObject& json )
{
    m_type = static_cast< PlayerType >( json["type"].get_int() );
    m_team = static_cast< PlayerTeam >( json["team"].get_int() );
    m_identity = json["identity"].get_str();
    m_name = json["name"].get_str();
    m_color = json["color"].get_int();
    m_tetrads = json["tetrads"].get_int();
    m_numberOfMoves = json["numberOfMoves"].get_int();
}


IMove::Ptr PlayerSpec::GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove )
{
    // Does nothing
    return IMove::Ptr();
}
IPlayer::Ptr IPlayer::CreateSpec()
{
    return std::make_shared< PlayerSpec >();
}

class LocalPlayer : public PlayerBase
{
public:
    typedef std::shared_ptr< LocalPlayer > Ptr;
    static LocalPlayer::Ptr Create( const PlayerTeam team, const std::string& identity, const std::string& name, const int color )
    {
        auto inst = LocalPlayer::Ptr( new LocalPlayer( team, identity, name, color ) );
        return inst;
    }
    ~LocalPlayer()
    {}
    virtual IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove )
    {
        return m_game->DoLocalTurn();
    }

private:
    LocalPlayer( const PlayerTeam team, const std::string& identity, const std::string& name, const int color ) :
        PlayerBase( PLAYER_LOCAL, team, identity, name, color )
    {}
};

class ComputerPlayer : public PlayerBase
{
public:
    typedef std::shared_ptr< ComputerPlayer > Ptr;
    static ComputerPlayer::Ptr Create( const PlayerTeam team, const std::string& identity, const std::string& name, const int color )
    {
        auto inst = ComputerPlayer::Ptr( new ComputerPlayer( team, identity, name, color ) );
        return inst;
    }
    ~ComputerPlayer()
    {}
    virtual IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove )
    {
        return m_moveGenerator->GetNextMove( board, lastmove, this->GetTetrads(),
            m_game->GetPlayerForTeam( FlipTeam( this->GetTeam() ) )->GetTetrads() );
    }

private:
    ComputerPlayer( const PlayerTeam team, const std::string& identity, const std::string& name, const int color ) :
        PlayerBase( PLAYER_COMPUTER, team, identity, name, color )
    {
        m_moveGenerator = MoveGeneratorFactory::Instance()->Create( identity, team );
    }
    IMoveGenerator::Ptr m_moveGenerator;
};

class RemotePlayer : public PlayerBase
{
public:
    typedef std::shared_ptr< RemotePlayer > Ptr;
    static RemotePlayer::Ptr Create( const PlayerTeam team, const std::string& identity, const std::string& name, const int color )
    {
        auto inst = RemotePlayer::Ptr( new RemotePlayer( team, identity, name, color ) );
        return inst;
    }
    ~RemotePlayer()
    {}
    virtual IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove )
    {
        return m_game->DoRemoteTurn();
    }

private:
    RemotePlayer( const PlayerTeam team, const std::string& identity, const std::string& name, const int color ) :
        PlayerBase( PLAYER_REMOTE, team, identity, name, color )
    {}
};

PlayerFactory::Ptr PlayerFactory::Instance()
{
    static auto inst = PlayerFactory::Ptr( new PlayerFactory() );
    return inst;
}
bool PlayerFactory::Register( const PlayerType& t, PlayerCreator f )
{
    return m_reg.insert( Registry::value_type( t, f ) ).second;
}
PlayerBase::Ptr PlayerFactory::Create( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color ) const
{
    auto it = m_reg.find( type );
    if ( it != std::end( m_reg ) )
        return it->second( team, id, name, color );
    return PlayerBase::Ptr();
}
PlayerBase::Ptr PlayerFactory::RebuildPlayer( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color,
                                              const std::set< TetradShape >& shapes, const std::shared_ptr< Game>& game ) const
{
    auto ret = Create( type, team, id, name, color );
    ret->SetTetrads( shapes );
    ret->SetGame( game );
    return ret;
}
PlayerBase::Ptr PlayerFactory::RebuildPlayer( const PlayerBase::Ptr& p, const std::shared_ptr< Game>& game ) const
{
    return RebuildPlayer( p->GetType(), p->GetTeam(), p->GetIdentity(), p->GetName(), p->GetColor(), p->GetTetrads(), game );
}
PlayerFactory::PlayerFactory()
{}

// Registration
namespace {
    using namespace Implbits;
    bool s_local = PlayerFactory::Instance()->Register( PLAYER_LOCAL,
                                                        []( const PlayerTeam team, const std::string& id, const std::string& name, const int color ) -> PlayerBase::Ptr
        {
            return LocalPlayer::Create( team, id, name, color );
        } );
    bool s_computer = PlayerFactory::Instance()->Register( PLAYER_COMPUTER,
                                                           []( const PlayerTeam team, const std::string& id, const std::string& name, const int color ) -> PlayerBase::Ptr
        {
            return ComputerPlayer::Create( team, id, name, color );
        } );
    bool s_remote = PlayerFactory::Instance()->Register( PLAYER_REMOTE,
                                                         []( const PlayerTeam team, const std::string& id, const std::string& name, const int color ) -> PlayerBase::Ptr
        {
            return RemotePlayer::Create( team, id, name, color );
        } );
}

} // Implbits
