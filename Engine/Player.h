#pragma once
#include "Interfaces.h"
#include "GameBoard.h"
#include "Move.h"
#include "JsonSerializable.h"

#include <set>


namespace Implbits {

class Game;
class PlayerBase : public IPlayer, public IJsonSerializable
{
public:
    typedef std::shared_ptr< PlayerBase > Ptr;
    PlayerBase();
    PlayerBase( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color );
    virtual ~PlayerBase();
    void SetGame( const std::shared_ptr< Game>& game );

    // IPlayer
    virtual PlayerType GetType() const;
    virtual void SetType( const PlayerType type );
    virtual std::string GetIdentity() const;
    virtual void SetIdentity( const std::string& id );
    virtual std::string GetName() const;
    virtual void SetName( const std::string& name );
    virtual PlayerTeam GetTeam() const;
    virtual void SetTeam( const PlayerTeam team );
    virtual int GetColor() const;
    virtual void SetColor( const int color );
    virtual bool HasTetrad( const TetradShape shape ) const;
    virtual void IncrementNumberOfMoves();
    virtual int GetNumberOfMoves() const;

    virtual bool HasAllTetrads() const;
    virtual void AddTetrad( const TetradShape s );
    virtual std::set< TetradShape > PlayerBase::GetTetrads() const;
    virtual void SetTetrads( const std::set< TetradShape >& shapes );
    virtual IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove ) = 0;

    // IJsonSerializable
    virtual json_spirit::mObject JsonEncode() const;
    virtual void JsonDecode( json_spirit::mObject& json );
protected:
    PlayerType m_type;
    PlayerTeam m_team;
    std::string m_identity;
    std::string m_name;
    int m_color;
    char m_tetrads;
    int m_numberOfMoves;
    std::shared_ptr< Game > m_game;
};

class PlayerSpec : public PlayerBase
{
public:
    virtual IMove::Ptr GetMove( const GameBoard::ConstPtr& board, const IMove::Ptr& lastmove );
};

class PlayerFactory
{
public:
    typedef std::shared_ptr< PlayerFactory > Ptr;
    typedef std::function< PlayerBase::Ptr ( const PlayerTeam&, const std::string&, const std::string&, const int ) > PlayerCreator;
    typedef std::map< PlayerType, PlayerCreator > Registry;
    static PlayerFactory::Ptr Instance();
    bool Register( const PlayerType& t, PlayerCreator f );
    PlayerBase::Ptr Create( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color ) const;
    PlayerBase::Ptr RebuildPlayer( const PlayerType& type, const PlayerTeam& team, const std::string& id, const std::string& name, const int color,
                                   const std::set< TetradShape >& shapes, const std::shared_ptr< Game>& game ) const;
    PlayerBase::Ptr RebuildPlayer( const PlayerBase::Ptr& p, const std::shared_ptr< Game>& game ) const;
private:
    PlayerFactory();
    Registry m_reg;
};

} // Implbits
