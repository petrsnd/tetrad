// Tile.h : Declaration of the CTile

#pragma once
#include "resource.h"       // main symbols
#include <Engine/Interfaces.h>


#include "TsCOM_i.h"



using namespace ATL;


// CTile

class ATL_NO_VTABLE CTile :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTile, &CLSID_Tile>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITile, &IID_ITile, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTile()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TILE)


BEGIN_COM_MAP(CTile)
	COM_INTERFACE_ENTRY(ITile)
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
        m_tile = Implbits::ITile::Ptr();
	}

    void Init( const Implbits::ITile::Ptr& tile )
    {
        m_tile = tile;
    }

    Implbits::ITile::Ptr GetPtr() const
    {
        return m_tile;
    }

public:
	STDMETHOD(get_Id)(int* pVal);
	STDMETHOD(get_Position)(IPosition** pVal);
	STDMETHOD(put_Position)(IPosition* newVal);
	STDMETHOD(get_Team)(PlayerTeam* pVal);
	STDMETHOD(GetTargets)(SAFEARRAY ** targets);

private:
	Implbits::ITile::Ptr m_tile;
};

OBJECT_ENTRY_AUTO(__uuidof(Tile), CTile)
