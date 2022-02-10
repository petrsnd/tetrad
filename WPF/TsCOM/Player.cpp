// Player.cpp : Implementation of CPlayer

#include "stdafx.h"
#include "Player.h"
#include "Converters.h"


// CPlayer

STDMETHODIMP CPlayer::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =  
	{
		&IID_IPlayer
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CPlayer::get_Type(PlayerType* pVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    *pVal = ConvertImplbitsPlayerTypeToComPlayerType( m_player->GetType() );
    return S_OK;
}


STDMETHODIMP CPlayer::put_Type(PlayerType newVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    m_player->SetType( ConvertComPlayerTypeToImplbitsPlayerType( newVal ) );
    return S_OK;
}


STDMETHODIMP CPlayer::get_Identity(BSTR* pVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    CComBSTR s = m_player->GetIdentity().c_str();
    *pVal = s.Detach();
    return S_OK;
}


STDMETHODIMP CPlayer::put_Identity(BSTR newVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    m_player->SetIdentity( ( LPSTR )CW2A( newVal ) );
    return S_OK;
}

STDMETHODIMP CPlayer::get_Name(BSTR* pVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    CComBSTR s = m_player->GetName().c_str();
    *pVal = s.Detach();
    return S_OK;
}


STDMETHODIMP CPlayer::put_Name(BSTR newVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    m_player->SetName( ( LPSTR )CW2A( newVal ) );
    return S_OK;
}


STDMETHODIMP CPlayer::get_Team(PlayerTeam* pVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    *pVal = ConvertImplbitsTeamToComTeam( m_player->GetTeam() );
    return S_OK;
}


STDMETHODIMP CPlayer::put_Team(PlayerTeam newVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    m_player->SetTeam( ConvertComTeamToImplbitsTeam( newVal ) );
    return S_OK;
}


STDMETHODIMP CPlayer::HasTetrad(TetradShape shape, ULONG* result)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    if ( m_player->HasTetrad( ConvertComShapeToImplbitsShape( shape ) ) )
        *result = 1; // true
    else
        *result = 0; // false
    return S_OK;
}


STDMETHODIMP CPlayer::get_RgbColor(ULONG* pVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    *pVal = ( ULONG )m_player->GetColor();
    return S_OK;
}


STDMETHODIMP CPlayer::put_RgbColor(ULONG newVal)
{
    if ( ! m_player )
        return OLE_E_BLANK;
    m_player->SetColor( ( int )newVal );
    return S_OK;
}
