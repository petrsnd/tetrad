// Position.cpp : Implementation of CPosition

#include "stdafx.h"
#include "Position.h"


// CPosition

STDMETHODIMP CPosition::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IPosition
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CPosition::get_X(SHORT* pVal)
{
	*pVal = m_x;
	return S_OK;
}


STDMETHODIMP CPosition::put_X(SHORT newVal)
{
	m_x = newVal;
	return S_OK;
}


STDMETHODIMP CPosition::get_Y(SHORT* pVal)
{
	*pVal = m_y;
	return S_OK;
}


STDMETHODIMP CPosition::put_Y(SHORT newVal)
{
	m_y = newVal;
	return S_OK;
}
