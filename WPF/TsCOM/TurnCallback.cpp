// TurnCallback.cpp : Implementation of CTurnCallback

#include "stdafx.h"
#include "TurnCallback.h"
#include "Tile.h"
#include "Converters.h"
#include <comutil.h>

// CTurnCallback

STDMETHODIMP CTurnCallback::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ITurnCallback
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CTurnCallback::LocalTurnComplete(ITile* tile, IPosition* target)
{
    if ( ! m_callback )
        return OLE_E_BLANK;
	CTile* t = dynamic_cast< CTile* >( tile );
    m_callback->LocalTurnComplete( t->GetPtr(), ConvertComPositionToImplbitsPosition( target ) );
    return S_OK;
}


STDMETHODIMP CTurnCallback::AnimationComplete(void)
{
    if ( ! m_callback )
        return OLE_E_BLANK;
    m_callback->AnimationComplete();
    return S_OK;
}

STDMETHODIMP CTurnCallback::GetRemoteGameComplete(BSTR game)
{
	if( ! m_callback )
		return OLE_E_BLANK;
	std::string tmp = _bstr_t(game);
	m_callback->GetRemoteGameComplete( tmp );
	return S_OK;
}

STDMETHODIMP CTurnCallback::PutRemoteGameComplete(void)
{
	if( ! m_callback )
		return OLE_E_BLANK;
	m_callback->PutRemoteGameComplete();
	return S_OK;
}

STDMETHODIMP CTurnCallback::DeleteRemoteGameComplete(void)
{
	if( ! m_callback )
		return OLE_E_BLANK;
	m_callback->DeleteRemoteGameComplete();
	return S_OK;
}

