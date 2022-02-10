// TetradEngine.h : Declaration of the CTetradEngine

#pragma once
#include "resource.h"       // main symbols



#include "COM_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

#include <Interfaces.h>
#include <memory>

// CTetradEngine

class ATL_NO_VTABLE CTetradEngine :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTetradEngine, &CLSID_TetradEngine>,
	public ISupportErrorInfo,
	public IDispatchImpl<ITetradEngine, &IID_ITetradEngine, &LIBID_COMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTetradEngine();

DECLARE_REGISTRY_RESOURCEID(IDR_TETRADENGINE)


BEGIN_COM_MAP(CTetradEngine)
	COM_INTERFACE_ENTRY(ITetradEngine)
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
	STDMETHOD(GetBoard)(PLAYER player, ULONG* encoded);
	STDMETHOD(GetTetrads)(PLAYER player, ULONG* tetrads);
	STDMETHOD(GetTargets)(IPosition* position, SAFEARRAY** targets);
	STDMETHOD(Move)(IPosition* from, IPosition* to, IMoveResult** result);
	STDMETHOD(GetOpponentMove)(IMoveResult** result);
	STDMETHOD(Init)();

private:
	Implbits::IGameEngine::Ptr m_engine;
	Implbits::IGameMutator::Ptr m_mutator;
};

OBJECT_ENTRY_AUTO(__uuidof(TetradEngine), CTetradEngine)
