#include "pch.h"
#include "WinRTUtil.h"

#include <vector>

namespace TsWinRT
{

Platform::String^ ConvertStdStringToPlatformString( const std::string& s )
{
    int len = MultiByteToWideChar( CP_UTF8, 0, s.c_str(), static_cast< int >( s.length() ), NULL, 0 );
    std::vector< wchar_t > buf( len + 1, L'\0' );
    if ( 0 == MultiByteToWideChar( CP_UTF8, 0, s.c_str(), static_cast< int >( s.length() ), &buf[0], static_cast< int >( buf.size() ) ) )
        return L"";
    return ref new Platform::String( &buf[0] );
}
std::string ConvertPlatformStringToStdString( Platform::String^ s )
{
    int len = WideCharToMultiByte( CP_UTF8, 0, s->Data(), static_cast< int >( wcslen( s->Data() ) ), NULL, 0, NULL, NULL );
    std::vector< char > buf( len + 1, '\0' );
    if ( 0 == WideCharToMultiByte( CP_UTF8, 0, s->Data(), static_cast< int >( wcslen( s->Data() ) ), &buf[0], static_cast< int >( buf.size() ), NULL, NULL ) )
        return "";
    return &buf[0];
}

} // TsWinRT
