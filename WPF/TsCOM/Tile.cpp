// Tile.cpp : Implementation of CTile

#include "stdafx.h"
#include "Tile.h"

#include <algorithm>

#include "Position.h"
#include "Converters.h"

// CTile

STDMETHODIMP CTile::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ITile
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CTile::get_Id(int* pVal)
{
    if ( ! m_tile )
        return OLE_E_BLANK;
    *pVal = m_tile->GetId();
    return S_OK;
}


STDMETHODIMP CTile::get_Position(IPosition** pVal)
{
    if ( ! m_tile )
        return OLE_E_BLANK;
    auto pos = ConvertImplbitsPositionToComPosition( m_tile->GetPos() );
    *pVal = pos.Detach();
    // TODO: handle HR Exception?
    return S_OK;
}


STDMETHODIMP CTile::put_Position(IPosition* newVal)
{
    if ( ! m_tile )
        return OLE_E_BLANK;
    auto pos = ConvertComPositionToImplbitsPosition( newVal );
    m_tile->SetPos( pos );
    // TODO: handle HR Exception?
    return S_OK;
}


STDMETHODIMP CTile::get_Team(PlayerTeam* pVal)
{
    if ( ! m_tile )
        return OLE_E_BLANK;
    *pVal = ConvertImplbitsTeamToComTeam( m_tile->GetOwner() );
    return S_OK;
}


STDMETHODIMP CTile::GetTargets(SAFEARRAY ** targets)
{
    if ( ! m_tile )
        return OLE_E_BLANK;
    CComSafeArray< VARIANT > arr;
    auto vTargets = m_tile->GetTargets();
    if ( vTargets.size() )
    {
        std::for_each( std::begin( vTargets ), std::end( vTargets ), [&arr]( Implbits::Position& pos )
            {
                // TODO: handle HR Exceptions?
                AppendObject( arr, ConvertImplbitsPositionToComPosition( pos ) );
            } );
        *targets = arr.Detach();
        return S_OK;
    }
    ULONG zero = 0;
    arr.Create( zero, zero );
    *targets = arr.Detach();
    return S_OK;
}
