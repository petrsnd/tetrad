#include "JsonSerializeUtil.h"

#include <algorithm>


namespace Implbits {

json_spirit::mObject JsonEncodePosition( const Position& pos )
{
    json_spirit::mObject obj;
    obj.insert( json_spirit::mObject::value_type( "x", pos.x ) );
    obj.insert( json_spirit::mObject::value_type( "y", pos.y ) );
    return obj;
}

Position JsonDecodePosition( json_spirit::mObject& json )
{
    Position pos;
    pos.x = json["x"].get_int();
    pos.y = json["y"].get_int();
    return pos;
}

json_spirit::mObject JsonEncodeTetradResult( const TetradResult& tr )
{
    json_spirit::mObject obj;
    obj.insert( json_spirit::mObject::value_type( "home", tr.home ) );
    json_spirit::mArray arrHomeSet;
    std::for_each( std::begin( tr.homeSet ), std::end( tr.homeSet ), [&arrHomeSet]( const Position& pos )
        {
            arrHomeSet.push_back( JsonEncodePosition( pos ) );
        } );
    obj.insert( json_spirit::mObject::value_type( "homeSet", arrHomeSet ) );
    obj.insert( json_spirit::mObject::value_type( "away", tr.away ) );
    json_spirit::mArray arrAwaySet;
    std::for_each( std::begin( tr.awaySet ), std::end( tr.awaySet ), [&arrAwaySet]( const Position& pos )
        {
            arrAwaySet.push_back( JsonEncodePosition( pos ) );
        } );
    obj.insert( json_spirit::mObject::value_type( "awaySet", arrAwaySet ) );
    return obj;
}

TetradResult JsonDecodeTetradResult( json_spirit::mObject& json )
{
    TetradResult tr;
    tr.home = static_cast< TetradShape >( json["home"].get_int() );
    json_spirit::mArray arrHomeSet = json["homeSet"].get_array();
    std::transform( std::begin( arrHomeSet ), std::end( arrHomeSet ), std::begin( tr.homeSet ),
        []( json_spirit::mValue& o ) -> Position
            {
                return JsonDecodePosition( o.get_obj() );
            } );
    tr.away = static_cast< TetradShape >( json["away"].get_int() );
    json_spirit::mArray arrAwaySet = json["awaySet"].get_array();
    std::transform( std::begin( arrAwaySet ), std::end( arrAwaySet ), std::begin( tr.awaySet ),
        []( json_spirit::mValue& o ) -> Position
            {
                return JsonDecodePosition( o.get_obj() );
            } );
    return tr;
}

} // Implbits
