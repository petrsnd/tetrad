#include <boost/test/unit_test.hpp>

#include "Engine/Player.h"

BOOST_AUTO_TEST_CASE( TestPlayerSerialize )
{
    auto player = Implbits::PlayerFactory::Instance()->Create( Implbits::PLAYER_COMPUTER, Implbits::TEAM_AWAY, "intermediate", "dude", 1234 );
    player->AddTetrad( Implbits::TETRAD_BACKWARDSL );
    player->AddTetrad( Implbits::TETRAD_LSHAPE );
    player->AddTetrad( Implbits::TETRAD_SQUARE );
    player->AddTetrad( Implbits::TETRAD_LINE );
    std::string encoding = json_spirit::write( player->JsonEncode() );
    BOOST_CHECK( ! encoding.empty() );
    json_spirit::mValue value;
    BOOST_REQUIRE( json_spirit::read( encoding, value ) );
    BOOST_REQUIRE( value.type() == json_spirit::obj_type );
    Implbits::PlayerBase::Ptr playerdec = std::make_shared< Implbits::PlayerSpec >();
    playerdec->JsonDecode( value.get_obj() );
    BOOST_CHECK( player.get() != playerdec.get() );
    BOOST_CHECK( player->GetIdentity() == playerdec->GetIdentity() );
    BOOST_CHECK( player->GetName() == playerdec->GetName() );
    BOOST_CHECK( player->GetTeam() == playerdec->GetTeam() );
    BOOST_CHECK( player->GetType() == playerdec->GetType() );
    BOOST_CHECK( player->GetColor() == playerdec->GetColor() );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_SQUARE ) == true );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_LINE ) == true );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_BACKWARDSL ) == true );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_LSHAPE ) == true );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_DOWNSTEP ) == false );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_UPSTEP ) == false );
    BOOST_CHECK( player->HasTetrad( Implbits::TETRAD_PODIUM ) == false );
}
