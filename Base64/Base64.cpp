#include "Base64.h"

#include <algorithm>
#include <iterator>

#include "b64.h"

namespace b64
{

DataBuffer Base64Decode( const std::string& str )
{
    size_t buflen = b64_decode( str.c_str(), str.length(), NULL, 0 );
    DataBuffer ret( buflen );
	if( !str.empty() )
		b64_decode( str.c_str(), str.length(), static_cast< void* >( &ret[0] ), buflen );
    return ret;
}
std::string Base64DecodeAsString( const std::string& str )
{
    auto ret = Base64Decode( str );
    ret.push_back( NULL );
    return reinterpret_cast< char* >( &ret[0] );
}
std::string Base64Encode( const DataBuffer& buf )
{
    size_t buflen = b64_encode( static_cast< const void* >( &buf[0] ), buf.size(), NULL, 0 );
    DataBuffer ret( buflen );
    b64_encode( static_cast< const void* >( &buf[0] ), buf.size(),  reinterpret_cast< char* >( &ret[0] ), buflen );
    ret.push_back( NULL );
    return reinterpret_cast< char* >( &ret[0] );
}
std::string Base64EncodeFromString( const std::string& str )
{
    DataBuffer buf;
    std::copy( std::begin( str ), std::end( str ), std::back_inserter( buf ) );
    return Base64Encode( buf );
}

} // b64
