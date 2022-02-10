#include "pch.h"

namespace TsDx
{

template < typename Iface >
inline void SafeRelease( Iface** currentObject )
{
    if ( *currentObject != NULL )
    {
        ( *currentObject )->Release();
        *currentObject = NULL;
    }
}

template < typename Iface >
inline Iface* SafeAcquire( Iface* newObject )
{
    if ( newObject != NULL )
        newObject->AddRef();
    return newObject;
}

template < typename Iface >
class TsIUnknownImpl : public Iface
{
public:
    TsIUnknownImpl() :
        m_refCount( 0 )
    {}
    virtual ~TsIUnknownImpl()
    {}
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( /* [in] */ REFIID riid,
                                                      /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject )
    {
        if ( riid == IID_IUnknown || riid == __uuidof( Iface ) )
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        else
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
    }
    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement( &m_refCount );
    }
    virtual ULONG STDMETHODCALLTYPE Release()
    {
        ULONG newCount = InterlockedDecrement( &m_refCount );
        if ( newCount == 0 )
            delete this;
        return newCount;
    }
protected:
    ULONG m_refCount;
};

} // TsDX
