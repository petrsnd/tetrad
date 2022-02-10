#pragma once
#include <atlsafe.h>

// TODO: check HRESULTs on all this stuff and throw an exception

// Create a COM object of type ClassT and return 
// a CComPtr to one of it's interfaces IFaceT.
template < typename IFaceT, typename ClassT > 
CComPtr< IFaceT > CoCreateIPtr()
{
    CComObject< ClassT >* rawPtr;
    CComObject< ClassT >::CreateInstance( &rawPtr );
    CComPtr< IFaceT > ret = nullptr;
    rawPtr->QueryInterface( &ret );
    return rawPtr;
};

// Create a new instance of ClassT and return a CComPtr 
// to the new object
template < typename ClassT >
CComPtr< ClassT > CoCreatePtr()
{
    CComObject< ClassT >* rawPtr;
    CComObject< ClassT >::CreateInstance( &rawPtr );
    return rawPtr;
};

template < typename T >
void AppendObject( CComSafeArray< VARIANT >& arr, CComPtr< T >& obj )
{
    IUnknown* iunk = nullptr;
    obj->QueryInterface( &iunk );
    if ( iunk != nullptr )
    {
        CComVariant vt( iunk );
        arr.Add( vt, true ); 
    }
}
