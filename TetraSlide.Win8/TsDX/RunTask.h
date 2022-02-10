#include "pch.h"

namespace TsDX
{

template< typename ReturnT >
static ReturnT RunTask( concurrency::task< ReturnT > t, HRESULT* hr )
{
    ReturnT ret = nullptr;
    try
    {
        ret = t.get();
        *hr = S_OK;
    }
    catch ( Platform::COMException^ ex )
    {
        *hr = ex->HResult;
    }
    catch ( ... )
    {
        *hr = E_UNEXPECTED;
    }
    return ret;
}

} // TsDX
