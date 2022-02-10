#include "pch.h"
#include "TsPlayer.h"
#include "TsConvert.h"
#include "WinRTUtil.h"

namespace TsWinRT
{

TsPlayerSpec::TsPlayerSpec()
{}
TsPlayerSpec::~TsPlayerSpec()
{}
bool TsPlayerSpec::HasTetrad( TsTetradShape shape )
{
    return false;
}
int TsPlayerSpec::GetNumberOfMoves()
{
    return -1;
}

TsPlayer::TsPlayer( const Implbits::IPlayer::Ptr& player ) :
    m_player( player )
{}
TsPlayer::~TsPlayer()
{}
TsPlayerType TsPlayer::Type::get()
{
    return ConvertToTsPlayerType( m_player->GetType() );
}
void TsPlayer::Type::set( TsPlayerType type )
{
    m_player->SetType( ConvertToEnginePlayerType( type ) );
}
Platform::String^ TsPlayer::Identity::get()
{
    return ConvertStdStringToPlatformString( m_player->GetIdentity() );
}
void TsPlayer::Identity::set( Platform::String^ id )
{
    m_player->SetIdentity( ConvertPlatformStringToStdString( id ) );
}
Platform::String^ TsPlayer::Name::get()
{
    return ConvertStdStringToPlatformString( m_player->GetName() );
}
void TsPlayer::Name::set( Platform::String^ name )
{
    m_player->SetName( ConvertPlatformStringToStdString( name ) );
}
TsPlayerTeam TsPlayer::Team::get()
{
    return ConvertToTsPlayerTeam( m_player->GetTeam() );
}
void TsPlayer::Team::set( TsPlayerTeam team )
{
    m_player->SetTeam( ConvertToEnginePlayerTeam( team ) );
}
int TsPlayer::Color::get()
{
    return m_player->GetColor();
}
void TsPlayer::Color::set( int color )
{
    m_player->SetColor( color );
}
bool TsPlayer::HasTetrad( TsTetradShape shape )
{
    return m_player->HasTetrad( ConvertToEngineTetradShape( shape ) );
}
int TsPlayer::GetNumberOfMoves()
{
    return m_player->GetNumberOfMoves();
}

} // TsWinRT
