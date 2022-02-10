// Tetrad.h : Declaration of the CTetrad

#pragma once
#include "resource.h"       // main symbols



#include "COM_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CTetrad

class ATL_NO_VTABLE CTetrad :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTetrad, &CLSID_Tetrad>,
	public IDispatchImpl<ITetrad, &IID_ITetrad, &LIBID_COMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTetrad()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TETRAD)


BEGIN_COM_MAP(CTetrad)
	COM_INTERFACE_ENTRY(ITetrad)
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

	void SetShape( TETRAD_SHAPE );

	STDMETHOD(get_Shape)(TETRAD_SHAPE* pVal);
	STDMETHOD(get_Positions)(IPosition* positions[4]);
private:
	TETRAD_SHAPE m_shape;
	CComPtr<IPosition> m_postions[4];
};

OBJECT_ENTRY_AUTO(__uuidof(Tetrad), CTetrad)
