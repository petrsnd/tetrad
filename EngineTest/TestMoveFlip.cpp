#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"
#include "Engine/Player.h"
#include "Engine/Move.h"
#include "Engine/GameMechanics.h"
#include "Engine/GameMechanicsUtil.h"

BOOST_AUTO_TEST_CASE( TestTetradResultFlip )
{
    Implbits::TetradResult tr;
    tr.away = Implbits::TETRAD_BACKWARDSL;
    tr.awaySet[0] = Implbits::Position( 1, 1 );
    tr.awaySet[1] = Implbits::Position( 1, 2 );
    tr.awaySet[2] = Implbits::Position( 1, 3 );
    tr.awaySet[3] = Implbits::Position( 0, 3 );
    tr.home = Implbits::TETRAD_SQUARE;
    tr.homeSet[0] = Implbits::Position( 2, 2 );
    tr.homeSet[1] = Implbits::Position( 2, 3 );
    tr.homeSet[2] = Implbits::Position( 3, 2 );
    tr.homeSet[3] = Implbits::Position( 3, 3 );
    auto trexpected = tr;
    trexpected.away = Implbits::TETRAD_SQUARE;
    trexpected.awaySet[0] = Implbits::Position( 1, 2 );
    trexpected.awaySet[1] = Implbits::Position( 1, 3 );
    trexpected.awaySet[2] = Implbits::Position( 0, 2 );
    trexpected.awaySet[3] = Implbits::Position( 0, 3 );
    trexpected.home = Implbits::TETRAD_BACKWARDSL;
    trexpected.homeSet[0] = Implbits::Position( 2, 1 );
    trexpected.homeSet[1] = Implbits::Position( 2, 2 );
    trexpected.homeSet[2] = Implbits::Position( 2, 3 );
    trexpected.homeSet[3] = Implbits::Position( 3, 3 );
    auto trflip = FlipTetradResult( tr );
    BOOST_CHECK( trflip.away == FlipTetradShape( trexpected.away ) );
    BOOST_CHECK( trflip.awaySet[0] == trexpected.awaySet[0] );
    BOOST_CHECK( trflip.awaySet[1] == trexpected.awaySet[1] );
    BOOST_CHECK( trflip.awaySet[2] == trexpected.awaySet[2] );
    BOOST_CHECK( trflip.awaySet[3] == trexpected.awaySet[3] );
    BOOST_CHECK( trflip.home == FlipTetradShape( trexpected.home ) );
    BOOST_CHECK( trflip.homeSet[0] == trexpected.homeSet[0] );
    BOOST_CHECK( trflip.homeSet[1] == trexpected.homeSet[1] );
    BOOST_CHECK( trflip.homeSet[2] == trexpected.homeSet[2] );
    BOOST_CHECK( trflip.homeSet[3] == trexpected.homeSet[3] );
}

BOOST_AUTO_TEST_CASE( TestTetradResultFlipTwice )
{
    Implbits::TetradResult tr;
    tr.away = Implbits::TETRAD_BACKWARDSL;
    tr.awaySet[0] = Implbits::Position( 1, 1 );
    tr.awaySet[1] = Implbits::Position( 1, 2 );
    tr.awaySet[2] = Implbits::Position( 1, 3 );
    tr.awaySet[3] = Implbits::Position( 0, 3 );
    tr.home = Implbits::TETRAD_SQUARE;
    tr.homeSet[0] = Implbits::Position( 2, 2 );
    tr.homeSet[1] = Implbits::Position( 2, 3 );
    tr.homeSet[2] = Implbits::Position( 3, 2 );
    tr.homeSet[3] = Implbits::Position( 3, 3 );
    auto trflip = FlipTetradResult( tr );
    auto trflip2 = FlipTetradResult( trflip );
    BOOST_CHECK( trflip2.away == tr.away );
    BOOST_CHECK( trflip2.awaySet[0] == tr.awaySet[0] );
    BOOST_CHECK( trflip2.awaySet[1] == tr.awaySet[1] );
    BOOST_CHECK( trflip2.awaySet[2] == tr.awaySet[2] );
    BOOST_CHECK( trflip2.awaySet[3] == tr.awaySet[3] );
    BOOST_CHECK( trflip2.home == tr.home );
    BOOST_CHECK( trflip2.homeSet[0] == tr.homeSet[0] );
    BOOST_CHECK( trflip2.homeSet[1] == tr.homeSet[1] );
    BOOST_CHECK( trflip2.homeSet[2] == tr.homeSet[2] );
    BOOST_CHECK( trflip2.homeSet[3] == tr.homeSet[3] );
}

BOOST_AUTO_TEST_CASE( TestMoveFlip )
{
    auto home = Implbits::PlayerFactory::Instance()->Create( Implbits::PLAYER_COMPUTER, Implbits::TEAM_HOME, "beginner", "cmp1", 1234 );
    auto away = Implbits::PlayerFactory::Instance()->Create( Implbits::PLAYER_COMPUTER, Implbits::TEAM_AWAY, "beginner", "cmp2", 2345 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto board = game->GetGameBoard();
    auto move = home->GetMove( board, Implbits::IMove::Ptr() );
    std::string encoding = json_spirit::write( move->JsonEncode() );
    BOOST_CHECK( ! encoding.empty() );
    json_spirit::mValue value;
    BOOST_REQUIRE( json_spirit::read( encoding, value ) );
    BOOST_REQUIRE( value.type() == json_spirit::obj_type );
    auto moveflip = Implbits::JsonDecodeMove( value.get_obj() );
    auto moveflip2 = Implbits::JsonDecodeMove( value.get_obj() );
    moveflip->Flip();
    moveflip2->Flip();
    moveflip2->Flip();
    BOOST_CHECK( move.get() != moveflip.get() );
    BOOST_CHECK( move.get() != moveflip2.get() );
    BOOST_CHECK( move->GetWho() != moveflip->GetWho() );
    BOOST_CHECK( move->GetWho() == moveflip2->GetWho() );
    auto from = move->GetFrom();
    auto fromflip = moveflip->GetFrom();
    BOOST_CHECK( from != fromflip );
    BOOST_CHECK( from == FlipPosition( fromflip ) );
    BOOST_CHECK( move->GetFrom() == moveflip2->GetFrom() );
    auto to = move->GetTo();
    auto toflip = moveflip->GetTo();
    BOOST_CHECK( to != toflip );
    BOOST_CHECK( to== FlipPosition( toflip ) );
    BOOST_CHECK( move->GetTo() == moveflip2->GetTo() );
    auto moveresult = move->GetResult();
    auto moveflipresult = moveflip->GetResult();
    auto moveflip2result = moveflip2->GetResult();
    BOOST_CHECK( moveresult.get() != moveflipresult.get() );
    BOOST_CHECK( moveresult.get() != moveflip2result.get() );
    if ( moveresult->GetDirection() == Implbits::DIR_UP || moveresult->GetDirection() == Implbits::DIR_DOWN )
        BOOST_CHECK( moveresult->GetDirection() == moveflipresult->GetDirection() );
    else
        BOOST_CHECK( moveresult->GetDirection() != moveflipresult->GetDirection() );
    BOOST_CHECK( moveresult->GetDirection() == FlipBoardDirection( moveflipresult->GetDirection() ) );
    BOOST_CHECK( moveresult->GetDirection() == moveflip2result->GetDirection() );
    BOOST_CHECK( moveresult->GetDistance() == moveflipresult->GetDistance() );
    BOOST_CHECK( moveresult->GetDistance() == moveflip2result->GetDistance() );
    auto positions = moveresult->GetPositions();
    auto positionsflip = moveflipresult->GetPositions();
    auto positionsflip2 = moveflip2result->GetPositions();
    BOOST_REQUIRE( positions.size() == positionsflip.size() );
    for ( size_t i = 0; i < positions.size(); i++ )
    {
        BOOST_CHECK( positions[i] != positionsflip[i] );
        BOOST_CHECK( positions[i] == FlipPosition( positionsflip[i] ) );
    }
    BOOST_REQUIRE( positions.size() == positionsflip2.size() );
    for ( size_t i = 0; i < positions.size(); i++ )
    {
        BOOST_CHECK( positions[i] == positionsflip2[i] );
    }
}
BOOST_AUTO_TEST_CASE( TestMoveFlipTwice )
{
    auto home = Implbits::PlayerFactory::Instance()->Create( Implbits::PLAYER_COMPUTER, Implbits::TEAM_HOME, "beginner", "cmp1", 1234 );
    auto away = Implbits::PlayerFactory::Instance()->Create( Implbits::PLAYER_COMPUTER, Implbits::TEAM_AWAY, "beginner", "cmp2", 2345 );
    auto game = Implbits::Game::CreateNewGame( Implbits::DefaultGameIdInitiator, home, away );
    auto board = game->GetGameBoard();
    Implbits::IMove::Ptr lastmove;
    for ( int n = 0; n < 100; n++ )
    {
        Implbits::IMove::Ptr move;
        if ( ! lastmove || lastmove->GetWho() == Implbits::STATE_AWAY )
            move = home->GetMove( board, lastmove );
        else
            move = away->GetMove( board, lastmove );
        std::string encoding = json_spirit::write( move->JsonEncode() );
        BOOST_CHECK( ! encoding.empty() );
        json_spirit::mValue value;
        BOOST_REQUIRE( json_spirit::read( encoding, value ) );
        BOOST_REQUIRE( value.type() == json_spirit::obj_type );
        auto moveflip2 = Implbits::JsonDecodeMove( value.get_obj() );
        moveflip2->Flip();
        moveflip2->Flip();
        BOOST_CHECK( move.get() != moveflip2.get() );
        BOOST_CHECK( move->GetWho() == moveflip2->GetWho() );
        BOOST_CHECK( move->GetFrom() == moveflip2->GetFrom() );
        BOOST_CHECK( move->GetTo() == moveflip2->GetTo() );
        auto moveresult = move->GetResult();
        auto moveflip2result = moveflip2->GetResult();
        BOOST_CHECK( moveresult.get() != moveflip2result.get() );
        BOOST_CHECK( moveresult->GetDirection() == moveflip2result->GetDirection() );
        BOOST_CHECK( moveresult->GetDistance() == moveflip2result->GetDistance() );
        auto positions = moveresult->GetPositions();
        auto positionsflip2 = moveflip2result->GetPositions();
        BOOST_REQUIRE( positions.size() == positionsflip2.size() );
        for ( size_t i = 0; i < positions.size(); i++ )
        {
            BOOST_CHECK( positions[i] == positionsflip2[i] );
        }
        BOOST_REQUIRE( moveresult->HasTetradForTeam( Implbits::TEAM_HOME ) == moveflip2result->HasTetradForTeam( Implbits::TEAM_HOME ) );
        BOOST_REQUIRE( moveresult->HasTetradForTeam( Implbits::TEAM_AWAY ) == moveflip2result->HasTetradForTeam( Implbits::TEAM_AWAY ) );
        if ( moveresult->HasTetradForTeam( Implbits::TEAM_HOME ) )
        {
            Implbits::TetradShape shape = moveresult->GetTetradForTeam( Implbits::TEAM_HOME );
            Implbits::TetradShape shapeflip2 = moveflip2result->GetTetradForTeam( Implbits::TEAM_HOME );
            BOOST_CHECK( shape == shapeflip2 );
            Implbits::ShapeSet ss = moveresult->GetTetradShapeSetForTeam( Implbits::TEAM_HOME );
            Implbits::ShapeSet ssflip2 = moveflip2result->GetTetradShapeSetForTeam( Implbits::TEAM_HOME );
            BOOST_REQUIRE( ss.size() == ssflip2.size() );
            for ( size_t i = 0; i < ss.size(); i++ )
            {
                BOOST_CHECK( ss[i] == ssflip2[i] );
            }
        }
        if ( moveresult->HasTetradForTeam( Implbits::TEAM_AWAY ) )
        {
            Implbits::TetradShape shape = moveresult->GetTetradForTeam( Implbits::TEAM_AWAY );
            Implbits::TetradShape shapeflip2 = moveflip2result->GetTetradForTeam( Implbits::TEAM_AWAY );
            BOOST_CHECK( shape == shapeflip2 );
            Implbits::ShapeSet ss = moveresult->GetTetradShapeSetForTeam( Implbits::TEAM_AWAY );
            Implbits::ShapeSet ssflip2 = moveflip2result->GetTetradShapeSetForTeam( Implbits::TEAM_AWAY );
            BOOST_REQUIRE( ss.size() == ssflip2.size() );
            for ( size_t i = 0; i < ss.size(); i++ )
            {
                BOOST_CHECK( ss[i] == ssflip2[i] );
            }
        }
        Implbits::GameMechanics::Instance()->ApplyMove( board, move );
        lastmove = move;
    }
}
