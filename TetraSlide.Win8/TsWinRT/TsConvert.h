#pragma once

#include "TsDefinitions.h"
#include <Engine/Definitions.h>

// The whole purpose of these functions is that the enums in TsWinRT might
// get out of sync with the enums in the Engine.  Don't want to break.

namespace TsWinRT
{

Implbits::PlayerType ConvertToEnginePlayerType( TsPlayerType t );
TsPlayerType ConvertToTsPlayerType( const Implbits::PlayerType t );

Implbits::PlayerTeam ConvertToEnginePlayerTeam( TsPlayerTeam t );
TsPlayerTeam ConvertToTsPlayerTeam( const Implbits::PlayerTeam t );

Implbits::BoardDirection ConvertToEngineBoardDirection( TsBoardDirection d );
TsBoardDirection ConvertToTsBoardDirection( const Implbits::BoardDirection d );

Implbits::TetradShape ConvertToEngineTetradShape( TsTetradShape s );
TsTetradShape ConvertToTsTetradShape( const Implbits::TetradShape s );

} // TsWinRT