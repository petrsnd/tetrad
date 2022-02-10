// Tetrad.cpp : Implementation of CTetrad

#include "stdafx.h"
#include "Tetrad.h"


// CTetrad

void CTetrad::SetShape( TETRAD_SHAPE shape )
{
	m_shape = shape;
}


STDMETHODIMP CTetrad::get_Shape(TETRAD_SHAPE* pVal)
{
	*pVal = m_shape;
	return S_OK;
}

STDMETHODIMP CTetrad::get_Positions(IPosition* positions[4])
{
	return S_OK;
}
