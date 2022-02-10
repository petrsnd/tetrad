// Engine.cpp : Implementation of CEngine

#include "stdafx.h"
#include "Engine.h"

#include <algorithm>

#include "GameDescriptor.h"
#include "TurnCallback.h"
#include "Player.h"
// #include "Tile.h"
#include "Converters.h"
#include <comutil.h>
#include <functional>
#include <stdexcept>


// Implbits::IView implementation
namespace Implbits
{

// So sad that we don't need this wonderful codes.
//typedef std::function< void ( void ) > DispatchFunction;
//static DWORD WINAPI DispatchHelper( void* ptr )
//{
//    DispatchFunction* f = ( DispatchFunction* )ptr;
//    try
//    {
//        ( *f )();
//    }
//    catch ( ... )
//    {}
//    return 0;
//}
//class Dispatcher
//{
//public:
//    typedef std::shared_ptr< Dispatcher > Ptr;
//    static Dispatcher::Ptr Instance()
//    {
//        static auto inst = Dispatcher::Ptr( new Dispatcher() );
//        return inst;
//    }
//    void Dispatch( DispatchFunction f )
//    {
//        m_dispatches.push_back( std::make_shared< DispatchImpl >( f ) );
//    }
//private:
//    Dispatcher()
//    {}
//    class DispatchImpl
//    {
//    public:
//        typedef std::shared_ptr< DispatchImpl > Ptr;
//        explicit DispatchImpl( DispatchFunction f ) :
//            m_hThread( 0 ), m_f( f )
//        {
//            m_hThread = CreateThread( NULL, 0, DispatchHelper, &m_f, 0, NULL );
//        }
//        ~DispatchImpl()
//        {
//            if ( m_hThread )
//            {
//                WaitForSingleObject( m_hThread, INFINITE );
//                CloseHandle( m_hThread );
//            }
//        }
//    private:
//        HANDLE m_hThread;
//        DispatchFunction m_f;
//    };
//    std::vector< DispatchImpl::Ptr > m_dispatches;
//};

class View : public IView
{
public:
    static IView::Ptr Create( ::IGameView* view )
    {
        auto v = IView::Ptr( new View( view ) );
        return v;
    }
private:
    explicit View( ::IGameView* view ) :
        m_view( view )
    {}
public:
    virtual ~View()
    {}
    // IView
    virtual void GameResumed( const IGameDescriptor::Ptr& game, const PlayerTeam turn )
    {
        m_view->GameResumed( GetRawComInterface< ::IGameDescriptor, ::CGameDescriptor, Implbits::IGameDescriptor::Ptr >( game ),
                                 ConvertImplbitsTeamToComTeam( turn ) );
    }
    virtual void GameSuspended( const IGameDescriptor::Ptr& game )
    {
        m_view->GameSuspended( GetRawComInterface< ::IGameDescriptor, ::CGameDescriptor, Implbits::IGameDescriptor::Ptr >( game ) );
    }
    virtual void GameOver( const IPlayer::Ptr& winner )
    {
        m_view->GameOver( GetRawComInterface< ::IPlayer, ::CPlayer, Implbits::IPlayer::Ptr >( winner ) );
    }
    virtual void StateChanged()
    {
        m_view->StateChanged();
    }
    virtual void TetradFound( const ITurnCallback::Ptr& callback, const IPlayer::Ptr& player, const TetradShape shape, const std::vector< ITile::Ptr >& tiles )
    {
        CComSafeArray< UINT > tileIds;
        std::for_each( std::begin( tiles ), std::end( tiles ), [&tileIds]( const ITile::Ptr& t )
            {
                tileIds.Add( ( UINT )t->GetId() );
            } );
        m_view->TetradFound( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ),
                             GetRawComInterface< ::IPlayer, ::CPlayer, Implbits::IPlayer::Ptr >( player ),
                             ConvertImplbitsShapeToComShape( shape ), tileIds );
    }
    virtual void DoLocalTurn( const ITurnCallback::Ptr& callback )
    {
        m_view->DoLocalTurn( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ) );
    }
    virtual void AnimateTurn( const ITurnCallback::Ptr& callback, const std::vector< ITile::Ptr >& tiles, const BoardDirection dir, const int dist )
    {
        CComSafeArray< UINT > tileIds;
        std::for_each( std::begin( tiles ), std::end( tiles ), [&tileIds]( const ITile::Ptr& t )
            {
                tileIds.Add( ( UINT )t->GetId() );
            } );
        UCHAR distance = ( UCHAR )dist;
        Direction direction = ConvertImplbitsDirectionToComDirection( dir );

        m_view->AnimateTurn( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ),
                             tileIds, direction, distance );
                             
    }
    virtual void Log( const std::string& msg, const std::string& file, int line )
    {
        m_view->Log( CComBSTR( msg.c_str() ), CComBSTR( file.c_str()), line );
    }
    virtual std::vector< std::string > GetRemoteGames() 
    {
        std::vector< std::string > retval;
        SAFEARRAY* sa = NULL;
        m_view->GetRemoteGames( &sa );
        if( sa != NULL )
        {
            CComSafeArray< BSTR > games;
            games.Attach( sa );
            for( size_t i = 0; i < games.GetCount(); ++i )
            {
                std::string tmp( _bstr_t ( games.GetAt(i) ) );
                retval.push_back( tmp );
            }
        }
        return retval;
    }
    virtual std::string NewRemoteGame( const std::string& localId, const std::string& remoteId )
    {
        CComBSTR gameId;
        HRESULT hr = m_view->NewRemoteGame( CComBSTR( localId.c_str() ), CComBSTR( remoteId.c_str() ), &gameId );
        if( hr == S_OK )
            return ( LPSTR )CW2A( gameId );
        else
            throw std::runtime_error("Unexpected failure.");
    }
    virtual void GetRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId )
    {
        m_view->GetRemoteGame( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ), CComBSTR( gameId.c_str() ) );
    }
    virtual void PutRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId, const std::string& payload )
    {
        m_view->PutRemoteGame( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ),
            CComBSTR( gameId.c_str() ), CComBSTR( payload.c_str() ) );
    }
    virtual void DeleteRemoteGame( const ITurnCallback::Ptr& callback, const std::string& gameId )
    {
        m_view->DeleteRemoteGame( GetRawComInterface< ::ITurnCallback, ::CTurnCallback, Implbits::ITurnCallback::Ptr >( callback ), CComBSTR( gameId.c_str() ) );
    }
private:
    CComPtr< ::IGameView > m_view;
};

} // Implbits



// CEngine

STDMETHODIMP CEngine::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IEngine
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CEngine::Init(IGameView* view)
{
    m_engine->Init( Implbits::View::Create( view ) );
	return S_OK;
}


static Implbits::IPlayer::Ptr GetImplbitsPlayerSpec( IPlayer* player )
{
    auto ret = Implbits::IPlayer::CreateSpec();
    PlayerType type;
    PlayerTeam team;
    ULONG color;
    CComBSTR identity;
    CComBSTR name;

    player->get_Type( &type );
    player->get_Team( &team );
    player->get_RgbColor( &color );
    player->get_Identity( &identity );
    player->get_Name( &name );

    ret->SetType( ConvertComPlayerTypeToImplbitsPlayerType( type ) );
    ret->SetTeam( ConvertComTeamToImplbitsTeam( team ) );
    ret->SetColor( ( int )color );
    if ( identity )
        ret->SetIdentity( ( LPSTR )CW2A( identity ) );
    if ( name )
        ret->SetName( ( LPSTR )CW2A( name ) );
    return ret;
}


STDMETHODIMP CEngine::CreateGame(IPlayer* player1, IPlayer* player2, IGameDescriptor** game)
{
    auto p1 = GetImplbitsPlayerSpec( player1 );
    auto p2 = GetImplbitsPlayerSpec( player2 );
    auto iGame = m_engine->CreateGame( p1, p2 );
    return GetRawComInterfaceNoThrow< ::IGameDescriptor, ::CGameDescriptor, Implbits::IGameDescriptor::Ptr >( iGame, game );
}


STDMETHODIMP CEngine::SuspendGame(IGameDescriptor* game)
{
	m_engine->SaveGame( dynamic_cast< CGameDescriptor* >( game )->GetPtr() );
	return S_OK;
}


STDMETHODIMP CEngine::ResumeGame(IGameDescriptor* game)
{
    m_engine->ResumeGame( dynamic_cast< CGameDescriptor* >( game )->GetPtr() );
    return S_OK;
}

STDMETHODIMP CEngine::GetGameDescriptors(BSTR localId, SAFEARRAY** games)
{
    CComSafeArray< VARIANT > arr;
    auto vGames = m_engine->GetRemoteGameDescriptors( ( LPSTR )CW2A( localId ) );
    try
    {
        std::for_each( std::begin( vGames ), std::end( vGames ), [&arr]( Implbits::IGameDescriptor::Ptr& t )
            {
                CComPtr< IGameDescriptor > ret = GetCComInterfacePtr< IGameDescriptor, CGameDescriptor, Implbits::IGameDescriptor::Ptr >( t );
                AppendObject( arr, ret );
            } );
        if ( arr )
            *games = arr.Detach();
    }
    catch ( ... )
    {
        // TODO: handle HR Exception
        return E_FAIL;
    }
    return S_OK;
}