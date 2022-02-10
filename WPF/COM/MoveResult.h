// MoveResult.h : Declaration of the CMoveResult

#pragma once
#include "resource.h"       // main symbols



#include "COM_i.h"

#include <vector>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CMoveResult

class ATL_NO_VTABLE CMoveResult :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMoveResult, &CLSID_MoveResult>,
	public IDispatchImpl<IMoveResult, &IID_IMoveResult, &LIBID_COMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMoveResult()
	{
	}

	virtual ~CMoveResult()
	{
		int breakhere = 100;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MOVERESULT)


BEGIN_COM_MAP(CMoveResult)
	COM_INTERFACE_ENTRY(IMoveResult)
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

	void SetDistance( BYTE distance );
	void SetDirection( BOARD_DIRECTION );
	void SetMyTetrad( ITetrad* tetrad );
	void SetTheirTetrad( ITetrad* tetrad );
	void SetPositions( const std::vector< CComPtr< IPosition > >& positions );


	STDMETHOD(GetTilePositions)(SAFEARRAY ** retval);
	STDMETHOD(get_Distance)(BYTE* pVal);
	STDMETHOD(get_Direction)(BOARD_DIRECTION* pVal);
	STDMETHOD(get_MyNewTetrad)(ITetrad** pVal);
	STDMETHOD(get_TheirNewTetrad)(ITetrad** pVal);

private:
	BYTE m_distance;
	BOARD_DIRECTION m_direction;
	CComPtr<ITetrad> m_myTetrad;
	CComPtr<ITetrad> m_theirTetrad;
	std::vector< CComPtr< IPosition > > m_positions;
};

OBJECT_ENTRY_AUTO(__uuidof(MoveResult), CMoveResult)
