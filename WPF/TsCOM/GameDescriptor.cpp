// GameDescriptor.cpp : Implementation of CGameDescriptor

#include "stdafx.h"
#include "GameDescriptor.h"

#include <algorithm>

#include "Player.h"
#include "Tile.h"
#include "Converters.h"


// CGameDescriptor

STDMETHODIMP CGameDescriptor::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IGameDescriptor
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CGameDescriptor::get_Id(BSTR* pVal)
{
    if ( ! m_game )
        return OLE_E_BLANK;
    CComBSTR id = m_game->GetId().c_str();
    *pVal = id.Detach();
    return S_OK;
}


STDMETHODIMP CGameDescriptor::get_FriendlyName(BSTR* pVal)
{
    if ( ! m_game )
        return OLE_E_BLANK;
    CComBSTR name = m_game->GetFriendlyName().c_str();
    *pVal = name.Detach();
    return S_OK;
}


STDMETHODIMP CGameDescriptor::GetTiles(SAFEARRAY** tiles)
{
    if ( ! m_game )
        return OLE_E_BLANK;
    CComSafeArray< VARIANT > arr;
    auto vTiles = m_game->GetTiles();
    try
    {
        std::for_each( std::begin( vTiles ), std::end( vTiles ), [&arr]( Implbits::ITile::Ptr& t )
            {
                CComPtr< ITile > ret = GetCComInterfacePtr< ITile, CTile, Implbits::ITile::Ptr >( t );
                AppendObject( arr, ret );
            } );
        *tiles = arr.Detach();
    }
    catch ( ... )
    {
        // TODO: handle HR Exception
        return E_FAIL;
    }
    return S_OK;
}


STDMETHODIMP CGameDescriptor::GetHomePlayer(IPlayer** player)
{
    if ( ! m_game )
        return OLE_E_BLANK;
    return GetRawComInterfaceNoThrow< IPlayer, CPlayer, Implbits::IPlayer::Ptr >( m_game->GetHomePlayer(), player );
}


STDMETHODIMP CGameDescriptor::GetAwayPlayer(IPlayer** player)
{
    if ( ! m_game )
        return OLE_E_BLANK;
    return GetRawComInterfaceNoThrow< IPlayer, CPlayer, Implbits::IPlayer::Ptr >( m_game->GetAwayPlayer(), player );
}

STDMETHODIMP CGameDescriptor::GetTurn(PlayerTeam* turn)
{
	if( ! m_game )
		return OLE_E_BLANK;
	*turn = ConvertImplbitsTeamToComTeam( m_game->GetTurn());
	return S_OK;
}





