// Player.h : Declaration of the CPlayer

#pragma once
#include "resource.h"       // main symbols
#include <Engine/Interfaces.h>


#include "TsCOM_i.h"



using namespace ATL;


// CPlayer

class ATL_NO_VTABLE CPlayer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPlayer, &CLSID_Player>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPlayer, &IID_IPlayer, &LIBID_TsCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPlayer() : m_player( Implbits::IPlayer::CreateSpec() )
	{
		int breakhere = 100;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PLAYER)


BEGIN_COM_MAP(CPlayer)
	COM_INTERFACE_ENTRY(IPlayer)
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
        m_player = Implbits::IPlayer::Ptr();
	}

    void Init( const Implbits::IPlayer::Ptr& player )
    {
        m_player = player;
    }

public:
	STDMETHOD(get_Type)(PlayerType* pVal);
	STDMETHOD(put_Type)(PlayerType newVal);
	STDMETHOD(get_Identity)(BSTR* pVal);
	STDMETHOD(put_Identity)(BSTR newVal);
	STDMETHOD(get_Name)(BSTR* pVal);
	STDMETHOD(put_Name)(BSTR newVal);
	STDMETHOD(get_Team)(PlayerTeam* pVal);
	STDMETHOD(put_Team)(PlayerTeam newVal);
	STDMETHOD(HasTetrad)(TetradShape shape, ULONG* result);
	STDMETHOD(get_RgbColor)(ULONG* pVal);
	STDMETHOD(put_RgbColor)(ULONG newVal);

private:
    Implbits::IPlayer::Ptr m_player;
};

OBJECT_ENTRY_AUTO(__uuidof(Player), CPlayer)
