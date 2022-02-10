// MoveResult.cpp : Implementation of CMoveResult

#include "stdafx.h"
#include "MoveResult.h"
#include "ComHelper.h"
#include <algorithm>

// CMoveResult

void CMoveResult::SetPositions( const std::vector< CComPtr< IPosition > >& positions )
{
	m_positions = positions;
}

void CMoveResult::SetDistance( BYTE distance )
{
	m_distance = distance;
}

void CMoveResult::SetDirection( BOARD_DIRECTION direction )
{
	m_direction = direction;
}
void CMoveResult::SetMyTetrad( ITetrad* tetrad )
{
	m_myTetrad = tetrad;
}
	
void CMoveResult::SetTheirTetrad( ITetrad* tetrad )
{
	m_theirTetrad = tetrad;
}

STDMETHODIMP CMoveResult::GetTilePositions(SAFEARRAY ** retval)
{
	if( m_positions.size() == 0 )
		return S_OK;
	CComSafeArray<VARIANT> arr;
	std::for_each( begin( m_positions ), end( m_positions ), [& arr]( CComPtr<IPosition>& pos ) {
		AppendObject( arr, pos );
	});
	
	// Uncomment the line below after adding stuff. 
	*retval = arr.Detach();
	return S_OK;
}


STDMETHODIMP CMoveResult::get_Distance(BYTE* pVal)
{
	*pVal = m_distance;
	return S_OK;
}


STDMETHODIMP CMoveResult::get_Direction(BOARD_DIRECTION* pVal)
{
	*pVal = m_direction;
	return S_OK;
}


STDMETHODIMP CMoveResult::get_MyNewTetrad(ITetrad** pVal)
{
	*pVal = m_myTetrad;
	return S_OK;
}


STDMETHODIMP CMoveResult::get_TheirNewTetrad(ITetrad** pVal)
{
	*pVal = m_theirTetrad;
	return S_OK;
}
