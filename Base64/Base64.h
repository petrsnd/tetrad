// My wrapper for the Synesis Base64 encoding library
// which is BSD Licensed
#pragma once
#include <string>
#include <vector>

namespace b64
{

typedef std::vector< unsigned char > DataBuffer;
DataBuffer Base64Decode( const std::string& str );
std::string Base64DecodeAsString( const std::string& str );
std::string Base64Encode( const DataBuffer& buf );
std::string Base64EncodeFromString( const std::string& str );

} // b64
