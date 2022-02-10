#pragma once

#include <string>

namespace TsWinRT
{

Platform::String^ ConvertStdStringToPlatformString( const std::string& s );
std::string ConvertPlatformStringToStdString( Platform::String^ s );

} // TsWinRT