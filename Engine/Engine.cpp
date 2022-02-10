#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "Interfaces.h"
#include "Game.h"
#include "Logger.h"
#include "MutexObject.h"

#include "Base64/Base64.h"

#include <map>


namespace Implbits {

class Engine : public IEngine
{
public:
    friend IEngine;
    typedef std::map< std::string, Game::Ptr > GameMap;
    virtual ~Engine();

    // IEngine
    virtual std::string Version() const;
    virtual void Init( const IView::Ptr& view );
    virtual IGameDescriptor::Ptr CreateGame( const IPlayer::Ptr& home, const IPlayer::Ptr& away );
    virtual std::string SaveGame( const IGameDescriptor::Ptr& game );
    virtual IGameDescriptor::Ptr LoadGame( const std::string& gameData );
    std::vector< IGameDescriptor::Ptr > GetRemoteGameDescriptors( const std::string& localId );
    virtual void ResumeGame( const IGameDescriptor::Ptr& game );
private:
    static const std::string s_version;
    Engine();
    Game::Ptr GetGameFromDescriptor( const IGameDescriptor::Ptr& descriptor ) const;

    GameMap m_games;
    IView::Ptr m_view;
    Game::Ptr m_gameToSuspend;
    MutexObject m_resumeMutex;
};

const std::string Engine::s_version( "TetraSlide Engine v 1.0" );
Engine::Engine()
{}
Engine::~Engine()
{}
std::string Engine::Version() const
{
    return s_version;
}
void Engine::Init( const IView::Ptr& view )
{
    Logger::Init( view );
    m_view = view;
    LOG << "Engine initialized!";
}
IGameDescriptor::Ptr Engine::CreateGame( const IPlayer::Ptr& home, const IPlayer::Ptr& away )
{
    Game::Ptr newgame;
    if ( away->GetType() == PLAYER_REMOTE )
        newgame = Game::CreateNewGame( [this, &home, &away]() -> std::string
            {
                return m_view->NewRemoteGame( home->GetIdentity(), away->GetIdentity() );
            }, home, away );
    else
        newgame = Game::CreateNewGame( DefaultGameIdInitiator, home , away );
    m_games[newgame->GetId()] = newgame;
    return newgame;
}
std::string Engine::SaveGame( const IGameDescriptor::Ptr& game )
{
    m_gameToSuspend = GetGameFromDescriptor( game );
    m_gameToSuspend->Quit();
    std::string encoding = json_spirit::write( m_gameToSuspend->JsonEncode() );
    return b64::Base64EncodeFromString( encoding );
}
IGameDescriptor::Ptr Engine::LoadGame( const std::string& gameData )
{
    auto desc = Game::LoadGame( gameData );
    m_games[desc->GetId()] = desc;
    return desc;
}
std::vector< IGameDescriptor::Ptr > Engine::GetRemoteGameDescriptors( const std::string& localId )
{
    std::vector< IGameDescriptor::Ptr > ret;
    auto gamePayloads = m_view->GetRemoteGames();
    std::transform( std::begin( gamePayloads ), std::end( gamePayloads ), std::back_inserter( ret ), [this, &localId]( const std::string& b64 ) -> IGameDescriptor::Ptr
        {
            try
            {
                auto desc = Game::LoadGame( b64 );
                if ( desc->GetSerializer() != localId )
                    desc->Flip();
                m_games[desc->GetId()] = desc;
                return desc;
            }
            catch ( ... )
            {
                return nullptr;
            }
        } );
    ret.erase( std::remove_if( std::begin( ret ), std::end( ret ), []( const IGameDescriptor::Ptr& game ) -> bool
        {
            return ! game;
        } ), std::end( ret ) );
    return ret;
}
Game::Ptr Engine::GetGameFromDescriptor( const IGameDescriptor::Ptr& descriptor ) const
{
    GameMap::const_iterator it = m_games.find( descriptor->GetId() );
    if ( it != std::end( m_games ) )
        return it->second;
    throw UnexpectedException( "Game not found - '" + descriptor->GetId() + "'" );
}
void Engine::ResumeGame( const IGameDescriptor::Ptr& game )
{
    m_resumeMutex.Lock();
    GetGameFromDescriptor( game )->Resume( m_view );
    if ( m_gameToSuspend )
    {
        m_view->GameSuspended( m_gameToSuspend );
        m_gameToSuspend = nullptr;
    }
    m_resumeMutex.Unlock();
}


IEngine::Ptr IEngine::Instance()
{
    static auto inst = IEngine::Ptr( new Engine() );
    return inst;
}

} // Implbits
