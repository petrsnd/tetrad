// Position.h : Declaration of the CPosition

#pragma once
#include "resource.h"       // main symbols


#include "TsCOM_i.h"



using namespace ATL;


// CPosition

class ATL_NO_VTABLE CPosition :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPosition, &CLSID_Position>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPosition, &IID_IPosition, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPosition() :
        m_x( 0 ), m_y( 0 )
    {}

DECLARE_REGISTRY_RESOURCEID(IDR_POSITION)


BEGIN_COM_MAP(CPosition)
	COM_INTERFACE_ENTRY(IPosition)
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
	STDMETHOD(get_X)(SHORT* pVal);
	STDMETHOD(put_X)(SHORT newVal);
	STDMETHOD(get_Y)(SHORT* pVal);
	STDMETHOD(put_Y)(SHORT newVal);

private:
    SHORT m_x;
    SHORT m_y;
};

OBJECT_ENTRY_AUTO(__uuidof(Position), CPosition)
