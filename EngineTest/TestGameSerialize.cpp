#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"
#include "Base64/Base64.h"

BOOST_AUTO_TEST_CASE( TestGameSerialize )
{
    auto home = Implbits::IPlayer::CreateSpec();
    home->SetIdentity( "homeidstring" );
    home->SetType( Implbits::PLAYER_LOCAL );
    home->SetTeam( Implbits::TEAM_HOME );
    home->SetColor( 192929 );
    auto away = Implbits::IPlayer::CreateSpec();
    away->SetIdentity( "awayidstring" );
    away->SetType( Implbits::PLAYER_REMOTE );
    away->SetTeam( Implbits::TEAM_AWAY );
    away->SetColor( 233 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto tiles = game->GetTiles();
    std::string encoding = json_spirit::write( game->JsonEncode() );
    json_spirit::mValue value;
    BOOST_REQUIRE( json_spirit::read( encoding, value ) );
    BOOST_REQUIRE( value.type() == json_spirit::obj_type );
    auto gamedec = Implbits::Game::Create();
    gamedec->JsonDecode( value.get_obj() );
    auto tilesdec = gamedec->GetTiles();
    BOOST_CHECK( game.get() != gamedec.get() );
    BOOST_CHECK( game->GetSerializer() != gamedec->GetSerializer() );
    BOOST_CHECK( game->GetSerializer() == "" );
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() == gamedec->GetSerializer() );
    BOOST_CHECK( game->GetId() == gamedec->GetId() );
    BOOST_CHECK( game->GetGameBoard()->Encode() == gamedec->GetGameBoard()->Encode() );
    BOOST_CHECK( game->GetFriendlyName() == gamedec->GetFriendlyName() );
    BOOST_REQUIRE( tiles.size() == tilesdec.size() );
    for ( size_t i = 0; i < tiles.size(); i++ )
    {
        BOOST_CHECK( tiles[i]->GetId() == tilesdec[i]->GetId() );
        BOOST_CHECK( tiles[i]->GetOwner() == tilesdec[i]->GetOwner() );
        BOOST_CHECK( tiles[i]->GetPos() == tilesdec[i]->GetPos() );
    }
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() == gamedec->GetHomePlayer()->GetIdentity() );
    BOOST_CHECK( game->GetHomePlayer()->GetColor() == gamedec->GetHomePlayer()->GetColor() );
    BOOST_CHECK( game->GetHomePlayer()->GetTeam() == gamedec->GetHomePlayer()->GetTeam() );
    BOOST_CHECK( game->GetTurn() == gamedec->GetTurn() );
}

BOOST_AUTO_TEST_CASE( TestGameSerializeB64 )
{
    auto home = Implbits::IPlayer::CreateSpec();
    home->SetIdentity( "fartyface" );
    home->SetType( Implbits::PLAYER_LOCAL );
    home->SetTeam( Implbits::TEAM_HOME );
    home->SetColor( 192929 );
    auto away = Implbits::IPlayer::CreateSpec();
    away->SetIdentity( "buggerbutt" );
    away->SetType( Implbits::PLAYER_COMPUTER );
    away->SetTeam( Implbits::TEAM_AWAY );
    away->SetColor( 233 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto tiles = game->GetTiles();
    std::string encoding = json_spirit::write( game->JsonEncode() );
    std::string b64 = b64::Base64EncodeFromString( encoding );
    auto gamedec = Implbits::Game::LoadGame( b64 );
    auto tilesdec = gamedec->GetTiles();
    BOOST_CHECK( game.get() != gamedec.get() );
    BOOST_CHECK( game->GetId() == gamedec->GetId() );
    BOOST_CHECK( game->GetGameBoard()->Encode() == gamedec->GetGameBoard()->Encode() );
    BOOST_CHECK( game->GetFriendlyName() == gamedec->GetFriendlyName() );
    BOOST_REQUIRE( tiles.size() == tilesdec.size() );
    for ( size_t i = 0; i < tiles.size(); i++ )
    {
        BOOST_CHECK( tiles[i]->GetId() == tilesdec[i]->GetId() );
        BOOST_CHECK( tiles[i]->GetOwner() == tilesdec[i]->GetOwner() );
        BOOST_CHECK( tiles[i]->GetPos() == tilesdec[i]->GetPos() );
    }
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() == gamedec->GetHomePlayer()->GetIdentity() );
    BOOST_CHECK( game->GetHomePlayer()->GetColor() == gamedec->GetHomePlayer()->GetColor() );
    BOOST_CHECK( game->GetHomePlayer()->GetTeam() == gamedec->GetHomePlayer()->GetTeam() );
}
