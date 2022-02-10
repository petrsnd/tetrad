// GameView.h : Declaration of the CGameView

#pragma once
#include "resource.h"       // main symbols



#include "TsCOM_i.h"



using namespace ATL;


// CGameView

class ATL_NO_VTABLE CGameView :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CGameView, &CLSID_GameView>,
	public ISupportErrorInfo,
	public IDispatchImpl<IGameView, &IID_IGameView, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CGameView()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GAMEVIEW)


BEGIN_COM_MAP(CGameView)
	COM_INTERFACE_ENTRY(IGameView)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



	STDMETHOD(GameResumed)(IGameDescriptor* game, PlayerTeam turn);
	STDMETHOD(GameSaved)(IGameDescriptor* game);
	STDMETHOD(GameOver)(IPlayer* winner);
	STDMETHOD(TetradFound)(IPlayer* player, TetradShape shape);
	STDMETHOD(DoLocalTurn)(ITurnCallback* callback);
	STDMETHOD(AnimateTurn)(ITurnCallback* callback, ITile* tile, IPosition* position);
};

OBJECT_ENTRY_AUTO(__uuidof(GameView), CGameView)
