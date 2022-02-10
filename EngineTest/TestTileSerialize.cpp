#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"

BOOST_AUTO_TEST_CASE( TestTileSerialize )
{
    auto tile = Implbits::Tile::Create( 1, Implbits::Position( 2, 3 ), Implbits::TEAM_HOME, Implbits::Game::Ptr() );
    std::string encoding = json_spirit::write( tile->JsonEncode() );
    BOOST_CHECK( ! encoding.empty() );
    json_spirit::mValue value;
    BOOST_REQUIRE( json_spirit::read( encoding, value ) );
    BOOST_REQUIRE( value.type() == json_spirit::obj_type );
    auto tiledec = Implbits::Tile::Create();
    tiledec->JsonDecode( value.get_obj() );
    BOOST_CHECK( tile.get() != tiledec.get() );
    BOOST_CHECK( tile->GetId() == tiledec->GetId() );
    BOOST_CHECK( tile->GetOwner() == tiledec->GetOwner() );
    BOOST_CHECK( tile->GetPos() == tiledec->GetPos() );
}
