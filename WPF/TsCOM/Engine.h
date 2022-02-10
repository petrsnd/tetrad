// Engine.h : Declaration of the CEngine

#pragma once
#include "resource.h"       // main symbols
#include <Engine/Interfaces.h>


#include "TsCOM_i.h"

#include <map>



using namespace ATL;


// CEngine

class ATL_NO_VTABLE CEngine :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CEngine, &CLSID_Engine>,
	public ISupportErrorInfo,
	public IDispatchImpl<IEngine, &IID_IEngine, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CEngine()
	{}

DECLARE_REGISTRY_RESOURCEID(IDR_ENGINE)


BEGIN_COM_MAP(CEngine)
	COM_INTERFACE_ENTRY(IEngine)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        m_engine = Implbits::IEngine::Instance();
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(Init)(IGameView* view);
	STDMETHOD(CreateGame)(IPlayer* player1, IPlayer* player2, IGameDescriptor** game);
	STDMETHOD(SuspendGame)(IGameDescriptor* game);
	STDMETHOD(ResumeGame)(IGameDescriptor* game);
	STDMETHOD(GetGameDescriptors)(BSTR localId, SAFEARRAY** games );

private:
    Implbits::IEngine::Ptr m_engine;
    std::map< std::string, Implbits::IGameDescriptor::Ptr > m_gameMap;
};

OBJECT_ENTRY_AUTO(__uuidof(Engine), CEngine)
