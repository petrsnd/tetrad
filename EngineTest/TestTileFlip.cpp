#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"
#include "Engine/GameMechanicsUtil.h"

BOOST_AUTO_TEST_CASE( TestPositionFlip )
{
    {
        Implbits::Position pos( 0, 0 );
        Implbits::Position posflip = FlipPosition( Implbits::Position( 3, 0 ) );
        BOOST_CHECK( pos == posflip );
    }
    {
        Implbits::Position pos( 1, 1 );
        Implbits::Position posflip = FlipPosition( Implbits::Position( 2, 1 ) );
        BOOST_CHECK( pos == posflip );
    }
    {
        Implbits::Position pos( 2, 2 );
        Implbits::Position posflip = FlipPosition( Implbits::Position( 1, 2 ) );
        BOOST_CHECK( pos == posflip );
    }
    {
        Implbits::Position pos( 3, 3 );
        Implbits::Position posflip = FlipPosition( Implbits::Position( 0, 3 ) );
        BOOST_CHECK( pos == posflip );
    }
}
BOOST_AUTO_TEST_CASE( TestTileFlip )
{
    auto tile = Implbits::Tile::Create( 1, Implbits::Position( 2, 3 ), Implbits::TEAM_HOME, Implbits::Game::Ptr() );
    auto tileflip = Implbits::Tile::Create( 1, Implbits::Position( 1, 3 ), Implbits::TEAM_AWAY, Implbits::Game::Ptr() );
    tileflip->Flip();
    BOOST_CHECK( tile.get() != tileflip.get() );
    BOOST_CHECK( tile->GetId() == tileflip->GetId() );
    BOOST_CHECK( tile->GetOwner() == tileflip->GetOwner() );
    BOOST_CHECK( tile->GetPos() == tileflip->GetPos() );
}
BOOST_AUTO_TEST_CASE( TestTileFlipTwice )
{
    auto tile = Implbits::Tile::Create( 1, Implbits::Position( 1, 1 ), Implbits::TEAM_HOME, Implbits::Game::Ptr() );
    auto tiledup = Implbits::Tile::Create( 1, Implbits::Position( 1, 1 ), Implbits::TEAM_HOME, Implbits::Game::Ptr() );
    tiledup->Flip();
    tiledup->Flip();
    BOOST_CHECK( tile.get() != tiledup.get() );
    BOOST_CHECK( tile->GetId() == tiledup->GetId() );
    BOOST_CHECK( tile->GetOwner() == tiledup->GetOwner() );
    BOOST_CHECK( tile->GetPos() == tiledup->GetPos() );
}
