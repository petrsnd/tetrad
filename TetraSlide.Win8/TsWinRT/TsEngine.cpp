#include "pch.h"
#include "TsEngine.h"
#include "TsConvert.h"
#include "WinRTUtil.h"

#include "TsTile.h"

namespace Implbits
{

class EngineView : public IView
{
public:
    static IView::Ptr Create( TsWinRT::ITsView^ view )
    {
        return IView::Ptr( new EngineView( view ) );
    }
private:
    EngineView( TsWinRT::ITsView^ view ) :
        m_view( view )
    {}
public:
    virtual ~EngineView()
    {}
    virtual void GameResumed( const IGameDescriptor::Ptr& game, const PlayerTeam turn )
    {
        m_view->GameResumed( ref new TsWinRT::TsGameDescriptor( game ), TsWinRT::ConvertToTsPlayerTeam( turn ) );
    }
    virtual void GameSuspended( const IGameDescriptor::Ptr& game )
    {
        m_view->GameSuspended( ref new TsWinRT::TsGameDescriptor( game ) );
    }
    virtual void GameOver( const IPlayer::Ptr& winner )
    {
        m_view->GameOver( ref new TsWinRT::TsPlayer( winner ) );
    }
    virtual void StateChanged()
    {
        m_view->StateChanged();
    }
    virtual void TetradFound( const ITurnCallback::Ptr& callback, const IPlayer::Ptr& player, const TetradShape shape, const std::vector< ITile::Ptr >& tiles )
    {
        auto tstiles = ref new Platform::Collections::Vector< TsWinRT::TsTile^ >();
        for ( auto t_it = std::begin( tiles ); t_it != std::end( tiles ); t_it++ )
            tstiles->Append( ref new TsWinRT::TsTile( *t_it ) );
        m_view->TetradFound( ref new TsWinRT::TsTurnCallback( callback ), ref new TsWinRT::TsPlayer( player ), TsWinRT::ConvertToTsTetradShape( shape ), tstiles );
    }
    virtual void DoLocalTurn( const ITurnCallback::Ptr& callback )
    {
        m_view->DoLocalTurn( ref new TsWinRT::TsTurnCallback( callback ) );
    }
    virtual void AnimateTurn( const ITurnCallback::Ptr& callback, const std::vector< ITile::Ptr >& tiles, const BoardDirection dir, const int dist )
    {
        auto tstiles = ref new Platform::Collections::Vector< TsWinRT::TsTile^ >();
        for ( auto t_it = std::begin( tiles ); t_it != std::end( tiles ); t_it++ )
            tstiles->Append( ref new TsWinRT::TsTile( *t_it ) );
        m_view->AnimateTurn( ref new TsWinRT::TsTurnCallback( callback ), tstiles, TsWinRT::ConvertToTsBoardDirection( dir ), dist );
    }
    virtual void Log( const std::string& msg, const std::string& file, int line )
    {
        m_view->Log( TsWinRT::ConvertStdStringToPlatformString( msg ), TsWinRT::ConvertStdStringToPlatformString( file ), line );
    }
    virtual std::vector< std::string > GetRemoteGames()
    {
        std::vector< std::string > ret;
        auto games = m_view->GetRemoteGames();
        for ( auto g_it = games->First(); g_it->HasCurrent; g_it->MoveNext() )
            ret.push_back( TsWinRT::ConvertPlatformStringToStdString( g_it->Current ) );
        return ret;
    }
    virtual std::string NewRemoteGame( const std::string& localId, const std::string& remoteId )
    {
        auto newgame = m_view->NewRemoteGame( TsWinRT::ConvertStdStringToPlatformString( localId ), TsWinRT::ConvertStdStringToPlatformString( remoteId ) );
        return TsWinRT::ConvertPlatformStringToStdString( newgame );
    }
    virtual void GetRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId )
    {
        m_view->GetRemoteGame( ref new TsWinRT::TsTurnCallback( callback ), TsWinRT::ConvertStdStringToPlatformString( gameId ) );
    }
    virtual void PutRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId, const std::string& payload )
    {
        m_view->PutRemoteGame( ref new TsWinRT::TsTurnCallback( callback ), TsWinRT::ConvertStdStringToPlatformString( gameId ), TsWinRT::ConvertStdStringToPlatformString( payload ) );
    }
    virtual void DeleteRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId )
    {
        m_view->DeleteRemoteGame( ref new TsWinRT::TsTurnCallback( callback ), TsWinRT::ConvertStdStringToPlatformString( gameId ) );
    }
private:
    TsWinRT::ITsView^ m_view;
};

} // Implbits

namespace TsWinRT
{

static Implbits::IPlayer::Ptr GetImplbitsPlayerSpec( ITsPlayer^ player )
{
    auto ret = Implbits::IPlayer::CreateSpec();
    ret->SetType( ConvertToEnginePlayerType( player->Type ) );
    ret->SetTeam( ConvertToEnginePlayerTeam( player->Team ) );
    ret->SetColor( player->Color );
    ret->SetIdentity( ConvertPlatformStringToStdString( player->Identity ) );
    ret->SetName( ConvertPlatformStringToStdString( player->Name ) );
    return ret;
}

TsEngine::TsEngine() :
    m_engine( Implbits::IEngine::Instance() )
{}
Platform::String^ TsEngine::Version()
{
    return ConvertStdStringToPlatformString( m_engine->Version() );
}
void TsEngine::Init( ITsView^ view )
{
    m_engine->Init( Implbits::EngineView::Create( view ) );
}
TsGameDescriptor^ TsEngine::CreateGame( ITsPlayer^ home, ITsPlayer^ away )
{
    auto game = m_engine->CreateGame( GetImplbitsPlayerSpec( home ), GetImplbitsPlayerSpec( away ) );
    return ref new TsGameDescriptor( game );
}
Platform::String^ TsEngine::SaveGame( TsGameDescriptor^ game )
{
    return ConvertStdStringToPlatformString( m_engine->SaveGame( game->GetPtr() ) );
}
TsGameDescriptor^ TsEngine::LoadGame( Platform::String^ gameData )
{
    auto game = m_engine->LoadGame( ConvertPlatformStringToStdString( gameData ) );
    return ref new TsGameDescriptor( game );
}
Windows::Foundation::Collections::IVector< TsGameDescriptor^ >^ TsEngine::GetRemoteGameDescriptors( Platform::String^ localId )
{
    auto ret = ref new Platform::Collections::Vector< TsGameDescriptor^ >();
    auto games = m_engine->GetRemoteGameDescriptors( ConvertPlatformStringToStdString( localId ) );
    for ( auto g_it = std::begin( games ); g_it != std::end( games ); g_it++ )
        ret->Append( ref new TsGameDescriptor( *g_it ) );
    return ret;
}
void TsEngine::ResumeGame( TsGameDescriptor^ game )
{
    m_engine->ResumeGame( game->GetPtr() );
}

} // TsWinRT
