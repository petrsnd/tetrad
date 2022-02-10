#include "pch.h"
#include "TsTile.h"
#include "TsConvert.h"

namespace TsWinRT
{

TsTile::TsTile( const Implbits::ITile::Ptr& tile ) :
    m_tile( tile )
{}
TsTile::~TsTile()
{}
int TsTile::Id::get()
{
    return m_tile->GetId();
}
TsPosition^ TsTile::Pos::get()
{
    auto pos = m_tile->GetPos();
    return ref new TsPosition( pos.x, pos.y );
}
void TsTile::Pos::set( TsPosition^ pos )
{
    m_tile->SetPos( Implbits::Position( pos->X, pos->Y ) );
}
TsPlayerTeam TsTile::Owner::get()
{
    return ConvertToTsPlayerTeam( m_tile->GetOwner() );
}
Windows::Foundation::Collections::IVector< TsTarget^ >^ TsTile::GetTargets()
{
    auto ret = ref new Platform::Collections::Vector< TsTarget^ >();
    auto tgts = m_tile->GetTargets();
    for ( auto t_it = std::begin( tgts ); t_it != std::end( tgts ); t_it++ )
        ret->Append( ref new TsTarget( ref new TsPosition( t_it->pos.x, t_it->pos.y ), t_it->valid ) ); // TODO: research BindableAppend??
    return ret;
}
Implbits::ITile::Ptr TsTile::GetPtr()
{
    return m_tile;
}

} // TsWinRT
