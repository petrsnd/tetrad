// GameDescriptor.h : Declaration of the CGameDescriptor

#pragma once
#include "resource.h"       // main symbols
#include <Engine/Interfaces.h>


#include "TsCOM_i.h"



using namespace ATL;


// CGameDescriptor

class ATL_NO_VTABLE CGameDescriptor :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CGameDescriptor, &CLSID_GameDescriptor>,
	public ISupportErrorInfo,
	public IDispatchImpl<IGameDescriptor, &IID_IGameDescriptor, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CGameDescriptor()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GAMEDESCRIPTOR)


BEGIN_COM_MAP(CGameDescriptor)
	COM_INTERFACE_ENTRY(IGameDescriptor)
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
        m_game = Implbits::IGameDescriptor::Ptr();
	}

    void Init( const Implbits::IGameDescriptor::Ptr& game )
    {
        m_game = game;
    }

    Implbits::IGameDescriptor::Ptr GetPtr() const
    {
        return m_game;
    }

public:
	STDMETHOD(get_Id)(BSTR* pVal);
	STDMETHOD(get_FriendlyName)(BSTR* pVal);
	STDMETHOD(GetTiles)(SAFEARRAY ** tiles);
	STDMETHOD(GetHomePlayer)(IPlayer** player);
	STDMETHOD(GetAwayPlayer)(IPlayer** player);
	STDMETHOD(GetTurn)(PlayerTeam* turn);


private:
    Implbits::IGameDescriptor::Ptr m_game;
};

OBJECT_ENTRY_AUTO(__uuidof(GameDescriptor), CGameDescriptor)
