#pragma once
#include "Definitions.h"
#include "JsonSerializable.h"


namespace Implbits {

json_spirit::mObject JsonEncodePosition( const Position& pos );
Position JsonDecodePosition( json_spirit::mObject& json );

json_spirit::mObject JsonEncodeTetradResult( const TetradResult& tr );
TetradResult JsonDecodeTetradResult( json_spirit::mObject& json );

}