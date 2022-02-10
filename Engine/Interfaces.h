#pragma once
#include "Exceptions.h"
#include "Definitions.h"

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <vector>


namespace Implbits {

class IPlayer
{
public:
    typedef std::shared_ptr< IPlayer > Ptr;
    static IPlayer::Ptr CreateSpec();
    virtual ~IPlayer() {}
    virtual PlayerType GetType() const = 0;
    virtual void SetType( const PlayerType type ) = 0;
    virtual std::string GetIdentity() const = 0;
    virtual void SetIdentity( const std::string& id ) = 0;
    virtual std::string GetName() const = 0;
    virtual void SetName( const std::string& name ) = 0;
    virtual PlayerTeam GetTeam() const = 0;
    virtual void SetTeam( const PlayerTeam team ) = 0;
    virtual int GetColor() const = 0;
    virtual void SetColor( const int color ) = 0;
    virtual bool HasTetrad( const TetradShape shape ) const = 0;
    virtual int GetNumberOfMoves() const = 0;
};

class ITile
{
public:
    typedef std::shared_ptr< ITile > Ptr;
    virtual ~ITile() {}
    virtual int GetId() const = 0;
    virtual Position GetPos() const = 0;
    virtual void SetPos( const Position& pos ) = 0;
    virtual PlayerTeam GetOwner() const = 0;
    virtual std::vector< Target > GetTargets() const = 0;
};

class IGameDescriptor
{
public:
    typedef std::shared_ptr< IGameDescriptor > Ptr;
    virtual ~IGameDescriptor() {}
    virtual std::string GetId() const = 0;
    virtual std::string GetFriendlyName() const = 0;
    virtual std::vector< ITile::Ptr > GetTiles() const = 0;
    virtual IPlayer::Ptr GetHomePlayer() const = 0;
    virtual IPlayer::Ptr GetAwayPlayer() const = 0;
    virtual PlayerTeam GetTurn() const = 0;
};

class ITurnCallback
{
public:
    typedef std::shared_ptr< ITurnCallback > Ptr;
    virtual ~ITurnCallback() {}
    virtual void LocalTurnComplete( const ITile::Ptr& tile, const Position& target ) = 0;
    virtual void AnimationComplete() = 0;
    virtual void GetRemoteGameComplete( const std::string& payload ) = 0;
    virtual void PutRemoteGameComplete() = 0;
    virtual void DeleteRemoteGameComplete() = 0;
};

class IView
{
public:
    typedef std::shared_ptr< IView > Ptr;
    virtual ~IView() {}
    virtual void GameResumed( const IGameDescriptor::Ptr& game, const PlayerTeam turn ) = 0;
    virtual void GameSuspended( const IGameDescriptor::Ptr& game ) = 0;
    virtual void GameOver( const IPlayer::Ptr& winner ) = 0;
    virtual void StateChanged() = 0;
    virtual void TetradFound( const ITurnCallback::Ptr& callback, const IPlayer::Ptr& player, const TetradShape shape, const std::vector< ITile::Ptr >& tiles ) = 0;
    virtual void DoLocalTurn( const ITurnCallback::Ptr& callback ) = 0;
    virtual void AnimateTurn( const ITurnCallback::Ptr& callback, const std::vector< ITile::Ptr >& tiles, const BoardDirection dir, const int dist ) = 0;
    virtual void Log( const std::string& msg, const std::string& file, int line ) = 0;
    virtual std::vector< std::string > GetRemoteGames() = 0;
    virtual std::string NewRemoteGame( const std::string& localId, const std::string& remoteId ) = 0;
    virtual void GetRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId ) = 0;
    virtual void PutRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId, const std::string& payload ) = 0;
    virtual void DeleteRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId ) = 0;
};

class IEngine
{
public:
    typedef std::shared_ptr< IEngine > Ptr;
    static IEngine::Ptr Instance();
    virtual ~IEngine() {}
    virtual std::string Version() const = 0;
    virtual void Init( const IView::Ptr& view ) = 0;
    virtual IGameDescriptor::Ptr CreateGame( const IPlayer::Ptr& home, const IPlayer::Ptr& away ) = 0;
    virtual std::string SaveGame( const IGameDescriptor::Ptr& game ) = 0;
    virtual IGameDescriptor::Ptr LoadGame( const std::string& gameData ) = 0;
    virtual std::vector< IGameDescriptor::Ptr > GetRemoteGameDescriptors( const std::string& localId ) = 0;
    virtual void ResumeGame( const IGameDescriptor::Ptr& game ) = 0;
};

} // Implbits
