#pragma once
#include "Interfaces.h"
#include "Player.h"
#include "JsonSerializable.h"
#include "GuidCreator.h"
#include "EventNotifier.h"


namespace Implbits {

class Game;
class Tile : public ITile, public IJsonSerializable
{
public:
    typedef std::shared_ptr< Tile > Ptr;
    static Tile::Ptr Create();
    static Tile::Ptr Create( const int id, const Position& pos, const PlayerTeam owner, const std::shared_ptr< Game >& game );
    virtual ~Tile();
    void Flip();
    void SetGame( const std::shared_ptr< Game>& game );

    // IJsonSerializable
    virtual json_spirit::mObject JsonEncode() const;
    virtual void JsonDecode( json_spirit::mObject& json );

    // ITile
    virtual int GetId() const;
    virtual Position GetPos() const;
    virtual void SetPos( const Position& pos );
    virtual PlayerTeam GetOwner() const;
    virtual std::vector< Target > GetTargets() const;
private:
    Tile();
    Tile( const int id, const Position& pos, const PlayerTeam owner, const std::shared_ptr< Game >& game );
    int m_id;
    Position m_pos;
    PlayerTeam m_owner;
    std::shared_ptr< Game > m_game;
};

typedef std::function< std::string ( void ) > GameIdInitiatorFunc;
static const GameIdInitiatorFunc DefaultGameIdInitiator = []() -> std::string
    {
        return Implbits::GuidCreator::Instance()->NewGuid();
    };
class Game : public IJsonSerializable, public IGameDescriptor, public ITurnCallback,
    public std::enable_shared_from_this< Game >
{
public:
    typedef std::shared_ptr< Game > Ptr;
    static Game::Ptr Create();
    static Game::Ptr CreateNewGame( const GameIdInitiatorFunc& initId, const IPlayer::Ptr& home, const IPlayer::Ptr& away );
    void InitIdRemote( const IView::Ptr& view );
    static Game::Ptr LoadGame( const std::string& b64 );
    virtual ~Game();

    void Quit();
    void Resume( const IView::Ptr& view );
    IMove::Ptr DoLocalTurn();
    IMove::Ptr DoRemoteTurn();

    std::string GetSerializer() const;
    void Flip();
    GameBoard::Ptr GetGameBoard() const;
    IMove::Ptr GetLastMove() const;
    PlayerBase::Ptr GetPlayerForTeam( const PlayerTeam team ) const;

    // IJsonSerializable
    virtual json_spirit::mObject JsonEncode() const;
    virtual void JsonDecode( json_spirit::mObject& json );

    // IGameDescriptor
    virtual std::string GetId() const;
    virtual std::string GetFriendlyName() const;
    virtual std::vector< ITile::Ptr > GetTiles() const;
    virtual IPlayer::Ptr GetHomePlayer() const;
    virtual IPlayer::Ptr GetAwayPlayer() const;
    virtual PlayerTeam GetTurn() const;

    // ITurnCallback
    virtual void LocalTurnComplete( const ITile::Ptr& tile, const Position& target );
    virtual void AnimationComplete();
    virtual void GetRemoteGameComplete( const std::string& payload );
    virtual void PutRemoteGameComplete();
    virtual void DeleteRemoteGameComplete();
private:
    Game();
    void InitTiles();
    void RunTurn();
    PlayerBase::Ptr GetPlayerForTurn() const;
    void CallDoLocalTurn();
    void CallPutGame();
    void CallGetGame();
    void CallDeleteGame();
    void CallAnimate();
    void UpdateTiles( const IMoveResult::Ptr& result );
    void UpdateTetrads( const IMoveResult::Ptr& result );
    void HandleTetradFound( const IMoveResult::Ptr& result, const PlayerTeam team );
    std::vector< ITile::Ptr > GetTilesFromPositions( const ShapeSet& shapeSet ) const;
    std::vector< ITile::Ptr > GetTilesFromPositions( const std::vector< Position >& positions ) const;
    void ChangeTurn();

    std::string m_serializer;
    std::string m_id;
    std::string m_friendlyName;
    std::vector< Tile::Ptr > m_tiles;
    PlayerBase::Ptr m_home;
    PlayerBase::Ptr m_away;
    IView::Ptr m_view;

    PlayerTeam m_turn;
    IMove::Ptr m_lastmove;

    EventNotifier m_viewNotifier;
    bool m_quit;
};

} // Implbits
