// TurnCallback.h : Declaration of the CTurnCallback

#pragma once
#include "resource.h"       // main symbols
#include <Engine/Interfaces.h>


#include "TsCOM_i.h"



using namespace ATL;


// CTurnCallback

class ATL_NO_VTABLE CTurnCallback :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTurnCallback, &CLSID_TurnCallback>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITurnCallback, &IID_ITurnCallback, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTurnCallback()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TURNCALLBACK)


BEGIN_COM_MAP(CTurnCallback)
	COM_INTERFACE_ENTRY(ITurnCallback)
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
        m_callback = Implbits::ITurnCallback::Ptr();
	}

    void Init( const Implbits::ITurnCallback::Ptr& callback )
    {
        m_callback = callback;
    }

public:
	STDMETHOD(LocalTurnComplete)( ITile* tile, IPosition* target );
	STDMETHOD(AnimationComplete)( void );
	STDMETHOD(GetRemoteGameComplete)( BSTR game );
	STDMETHOD(PutRemoteGameComplete)( void );
	STDMETHOD(DeleteRemoteGameComplete)( void );

private:
    Implbits::ITurnCallback::Ptr m_callback;
};

OBJECT_ENTRY_AUTO(__uuidof(TurnCallback), CTurnCallback)
