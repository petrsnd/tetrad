#pragma once
#ifdef _MSC_VER
#include <Windows.h>
#include <ObjBase.h>
#else
// TODO: Mac and other platforms
#endif

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace Implbits {

class GuidCreator
{
public:
    typedef std::shared_ptr< GuidCreator > Ptr;
    static GuidCreator::Ptr Instance()
    {
        static auto inst = GuidCreator::Ptr( new GuidCreator() );
        return inst;
    }
    ~GuidCreator()
    {
#ifdef _MSC_VER
        ::CoUninitialize();
#else
// TODO: Mac and other platforms
#endif
    }
    std::string NewGuid()
    {
#ifdef _MSC_VER
        GUID guid = { 0 };
        ::CoCreateGuid( &guid );
        LPOLESTR olestrGuid = NULL;
        StringFromCLSID( guid, &olestrGuid );
        std::string ret;
        int len = WideCharToMultiByte( CP_UTF8, 0, olestrGuid, static_cast< int >( wcslen( olestrGuid ) ), NULL, 0, NULL, NULL );
        std::vector< char > buf( len + 1, '\0' );
        if ( 0 != WideCharToMultiByte( CP_UTF8, 0, olestrGuid, static_cast< int >( wcslen( olestrGuid ) ), &buf[0], static_cast< int >( buf.size() ), NULL, NULL ) )
            ret = &buf[0];
        ::CoTaskMemFree( olestrGuid );
        return ret;
#else
// TODO: Mac and other platforms
#endif
    }
private:
    GuidCreator()
    {
#ifdef _MSC_VER
        ::CoInitialize( NULL );
#else
// TODO: Mac and other platforms
#endif
    }
};

} // Implbits
