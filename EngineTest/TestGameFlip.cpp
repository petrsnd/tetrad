#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"
#include "Engine/GameMechanicsUtil.h"


BOOST_AUTO_TEST_CASE( TestGameFlip )
{
    auto home = Implbits::IPlayer::CreateSpec();
    home->SetIdentity( "homeidstring" );
    home->SetName("home");
    home->SetType( Implbits::PLAYER_LOCAL );
    home->SetTeam( Implbits::TEAM_HOME );
    home->SetColor( 192929 );
    auto away = Implbits::IPlayer::CreateSpec();
    away->SetIdentity( "awayidstring" );
    away->SetName("away");
    away->SetType( Implbits::PLAYER_REMOTE );
    away->SetTeam( Implbits::TEAM_AWAY );
    away->SetColor( 233 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto tiles = game->GetTiles();
    auto gameflip = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto oldid = gameflip->GetId();
    gameflip->Flip();
    BOOST_CHECK( game.get() != gameflip.get() );
    BOOST_CHECK( oldid == gameflip->GetId() );
    BOOST_CHECK( game->GetGameBoard()->Encode() == gameflip->GetGameBoard()->Encode() ); // Because the board is in starting position these are equal
    BOOST_CHECK( game->GetFriendlyName() == gameflip->GetFriendlyName() );
    auto tilesflip = gameflip->GetTiles();
    BOOST_REQUIRE( tiles.size() == tilesflip.size() );
    for ( size_t i = 0; i < tiles.size(); i++ )
    {
        BOOST_CHECK( tiles[i]->GetId() == tilesflip[i]->GetId() );
        BOOST_CHECK( tiles[i]->GetOwner() != tilesflip[i]->GetOwner() );
        BOOST_CHECK( tiles[i]->GetOwner() == FlipTeam( tilesflip[i]->GetOwner() ) );
        BOOST_CHECK( tiles[i]->GetPos() != tilesflip[i]->GetPos() );
        BOOST_CHECK( tiles[i]->GetPos() == FlipPosition( tilesflip[i]->GetPos() ) );
    }
    // Make sure the type of home is always LOCAL and away is always REMOTE
    BOOST_CHECK( game->GetHomePlayer()->GetType() == gameflip->GetHomePlayer()->GetType() );
    BOOST_CHECK( game->GetAwayPlayer()->GetType() == gameflip->GetAwayPlayer()->GetType() );
    BOOST_CHECK( game->GetHomePlayer()->GetTeam() == gameflip->GetHomePlayer()->GetTeam() );
    BOOST_CHECK( game->GetAwayPlayer()->GetTeam() == gameflip->GetAwayPlayer()->GetTeam() );
    // Make sure identity, color, team have swapped
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() != gameflip->GetHomePlayer()->GetIdentity() );
    BOOST_CHECK( game->GetHomePlayer()->GetColor() != gameflip->GetHomePlayer()->GetColor() );
    BOOST_CHECK( game->GetHomePlayer()->GetName() != gameflip->GetHomePlayer()->GetName() );
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() == gameflip->GetAwayPlayer()->GetIdentity() );
    BOOST_CHECK( game->GetHomePlayer()->GetColor() == gameflip->GetAwayPlayer()->GetColor() );
    BOOST_CHECK( game->GetHomePlayer()->GetName() == gameflip->GetAwayPlayer()->GetName() );
    BOOST_CHECK( game->GetAwayPlayer()->GetIdentity() != gameflip->GetAwayPlayer()->GetIdentity() );
    BOOST_CHECK( game->GetAwayPlayer()->GetColor() != gameflip->GetAwayPlayer()->GetColor() );
    BOOST_CHECK( game->GetAwayPlayer()->GetName() != gameflip->GetAwayPlayer()->GetName() );
    BOOST_CHECK( game->GetAwayPlayer()->GetIdentity() == gameflip->GetHomePlayer()->GetIdentity() );
    BOOST_CHECK( game->GetAwayPlayer()->GetColor() == gameflip->GetHomePlayer()->GetColor() );
    BOOST_CHECK( game->GetAwayPlayer()->GetName() == gameflip->GetHomePlayer()->GetName() );
    BOOST_CHECK( game->GetTurn() != gameflip->GetTurn() );
}
BOOST_AUTO_TEST_CASE( TestGameFlipTwice )
{
    auto home = Implbits::IPlayer::CreateSpec();
    home->SetIdentity( "homeidstring" );
    home->SetName("home");
    home->SetType( Implbits::PLAYER_LOCAL );
    home->SetTeam( Implbits::TEAM_HOME );
    home->SetColor( 192929 );
    auto away = Implbits::IPlayer::CreateSpec();
    away->SetIdentity( "awayidstring" );
    away->SetName("away");
    away->SetType( Implbits::PLAYER_REMOTE );
    away->SetTeam( Implbits::TEAM_AWAY );
    away->SetColor( 233 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto tiles = game->GetTiles();
    auto gameflip2 = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto oldid = gameflip2->GetId();
    gameflip2->Flip();
    gameflip2->Flip();
    auto tilesflip2 = gameflip2->GetTiles();
    BOOST_CHECK( game.get() != gameflip2.get() );
    BOOST_CHECK( oldid == gameflip2->GetId() );
    BOOST_CHECK( game->GetGameBoard()->Encode() == gameflip2->GetGameBoard()->Encode() );
    BOOST_CHECK( game->GetFriendlyName() == gameflip2->GetFriendlyName() );
    BOOST_REQUIRE( tiles.size() == tilesflip2.size() );
    for ( size_t i = 0; i < tiles.size(); i++ )
    {
        BOOST_CHECK( tiles[i]->GetId() == tilesflip2[i]->GetId() );
        BOOST_CHECK( tiles[i]->GetOwner() == tilesflip2[i]->GetOwner() );
        BOOST_CHECK( tiles[i]->GetPos() == tilesflip2[i]->GetPos() );
    }
    // Make sure the type of home is always LOCAL and away is always REMOTE
    BOOST_CHECK( game->GetHomePlayer()->GetType() == gameflip2->GetHomePlayer()->GetType() );
    BOOST_CHECK( game->GetAwayPlayer()->GetType() == gameflip2->GetAwayPlayer()->GetType() );
    BOOST_CHECK( game->GetHomePlayer()->GetTeam() == gameflip2->GetHomePlayer()->GetTeam() );
    BOOST_CHECK( game->GetAwayPlayer()->GetTeam() == gameflip2->GetAwayPlayer()->GetTeam() );
    // Make sure that identity, color, and team have flipped twice
    BOOST_CHECK( game->GetHomePlayer()->GetIdentity() == gameflip2->GetHomePlayer()->GetIdentity() );
    BOOST_CHECK( game->GetHomePlayer()->GetColor() == gameflip2->GetHomePlayer()->GetColor() );
    BOOST_CHECK( game->GetHomePlayer()->GetName() == gameflip2->GetHomePlayer()->GetName() );
    BOOST_CHECK( game->GetAwayPlayer()->GetName() == gameflip2->GetAwayPlayer()->GetName() );
    BOOST_CHECK( game->GetTurn() == gameflip2->GetTurn() );
}
