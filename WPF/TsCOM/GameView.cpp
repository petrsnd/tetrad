// GameView.cpp : Implementation of CGameView

#include "stdafx.h"
#include "GameView.h"


// CGameView

STDMETHODIMP CGameView::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IGameView
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CGameView::GameResumed(IGameDescriptor* game, PlayerTeam turn)
{
	// TODO: Add your implementation code here

	return S_OK;
}


STDMETHODIMP CGameView::GameSaved(IGameDescriptor* game)
{
	// TODO: Add your implementation code here

	return S_OK;
}


STDMETHODIMP CGameView::GameOver(IPlayer* winner)
{
	// TODO: Add your implementation code here

	return S_OK;
}


STDMETHODIMP CGameView::TetradFound(IPlayer* player, TetradShape shape)
{
	// TODO: Add your implementation code here

	return S_OK;
}


STDMETHODIMP CGameView::DoLocalTurn(ITurnCallback* callback)
{
	// TODO: Add your implementation code here

	return S_OK;
}


STDMETHODIMP CGameView::AnimateTurn(ITurnCallback* callback, ITile* tile, IPosition* position)
{
	// TODO: Add your implementation code here

	return S_OK;
}
