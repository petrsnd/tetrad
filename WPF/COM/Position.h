// Position.h : Declaration of the CPosition

#pragma once
#include "resource.h"       // main symbols



#include "COM_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CPosition

class ATL_NO_VTABLE CPosition :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPosition, &CLSID_Position>,
	public IDispatchImpl<IPosition, &IID_IPosition, &LIBID_COMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPosition() : m_x(0), m_y(0)
	{
		int breakhere = 100;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_POSITION)


BEGIN_COM_MAP(CPosition)
	COM_INTERFACE_ENTRY(IPosition)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



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
	SHORT m_x,m_y;
};

OBJECT_ENTRY_AUTO(__uuidof(Position), CPosition)
