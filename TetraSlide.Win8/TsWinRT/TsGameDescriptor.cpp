#include "pch.h"
#include "TsGameDescriptor.h"
#include "TsConvert.h"
#include "WinRTUtil.h"

namespace TsWinRT
{

TsGameDescriptor::TsGameDescriptor( const Implbits::IGameDescriptor::Ptr& game ) :
    m_game( game )
{}
TsGameDescriptor::~TsGameDescriptor()
{}
Platform::String^ TsGameDescriptor::Id::get()
{
    return ConvertStdStringToPlatformString( m_game->GetId() );
}
Platform::String^ TsGameDescriptor::FriendlyName::get()
{
    return ConvertStdStringToPlatformString( m_game->GetFriendlyName() );
}
Windows::Foundation::Collections::IVector< TsTile^ >^ TsGameDescriptor::Tiles::get()
{
    auto ret = ref new Platform::Collections::Vector< TsTile^ >();
    auto tiles = m_game->GetTiles();
    for ( auto t_it = std::begin( tiles ); t_it != std::end( tiles ); t_it++ )
        ret->Append( ref new TsTile( *t_it ) );
    return ret;
}
TsPlayer^ TsGameDescriptor::HomePlayer::get()
{
    return ref new TsPlayer( m_game->GetHomePlayer() );
}
TsPlayer^ TsGameDescriptor::AwayPlayer::get()
{
    return ref new TsPlayer( m_game->GetAwayPlayer() );
}
TsPlayerTeam TsGameDescriptor::Turn::get()
{
    return ConvertToTsPlayerTeam( m_game->GetTurn() );
}
Implbits::IGameDescriptor::Ptr TsGameDescriptor::GetPtr()
{
    return m_game;
}

} // TsWinRT
