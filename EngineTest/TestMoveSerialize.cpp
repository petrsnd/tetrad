#include <boost/test/unit_test.hpp>

#include "Engine/Game.h"
#include "Engine/Player.h"
#include "Engine/Move.h"
#include "Engine/GameMechanics.h"

BOOST_AUTO_TEST_CASE( TestMoveSerialize )
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
        auto movedec = Implbits::JsonDecodeMove( value.get_obj() );
        BOOST_CHECK( move.get() != movedec.get() );
        BOOST_CHECK( move->GetWho() == movedec->GetWho() );
        BOOST_CHECK( move->GetFrom() == movedec->GetFrom() );
        BOOST_CHECK( move->GetTo() == movedec->GetTo() );
        auto moveresult = move->GetResult();
        auto movedecresult = movedec->GetResult();
        BOOST_CHECK( moveresult.get() != movedecresult.get() );
        BOOST_CHECK( moveresult->GetDirection() == movedecresult->GetDirection() );
        BOOST_CHECK( moveresult->GetDistance() == movedecresult->GetDistance() );
        auto positions = moveresult->GetPositions();
        auto positionsdec = movedecresult->GetPositions();
        BOOST_REQUIRE( positions.size() == positionsdec.size() );
        for ( size_t i = 0; i < positions.size(); i++ )
        {
            BOOST_CHECK( positions[i] == positionsdec[i] );
        }
        BOOST_REQUIRE( moveresult->HasTetradForTeam( Implbits::TEAM_HOME ) == movedecresult->HasTetradForTeam( Implbits::TEAM_HOME ) );
        BOOST_REQUIRE( moveresult->HasTetradForTeam( Implbits::TEAM_AWAY ) == movedecresult->HasTetradForTeam( Implbits::TEAM_AWAY ) );
        if ( moveresult->HasTetradForTeam( Implbits::TEAM_HOME ) )
        {
            Implbits::TetradShape shape = moveresult->GetTetradForTeam( Implbits::TEAM_HOME );
            Implbits::TetradShape shapedec = movedecresult->GetTetradForTeam( Implbits::TEAM_HOME );
            BOOST_CHECK( shape == shapedec );
            Implbits::ShapeSet ss = moveresult->GetTetradShapeSetForTeam( Implbits::TEAM_HOME );
            Implbits::ShapeSet ssdec = movedecresult->GetTetradShapeSetForTeam( Implbits::TEAM_HOME );
            BOOST_REQUIRE( ss.size() == ssdec.size() );
            for ( size_t i = 0; i < ss.size(); i++ )
            {
                BOOST_CHECK( ss[i] == ssdec[i] );
            }
        }
        if ( moveresult->HasTetradForTeam( Implbits::TEAM_AWAY ) )
        {
            Implbits::TetradShape shape = moveresult->GetTetradForTeam( Implbits::TEAM_AWAY );
            Implbits::TetradShape shapedec = movedecresult->GetTetradForTeam( Implbits::TEAM_AWAY );
            BOOST_CHECK( shape == shapedec );
            Implbits::ShapeSet ss = moveresult->GetTetradShapeSetForTeam( Implbits::TEAM_AWAY );
            Implbits::ShapeSet ssdec = movedecresult->GetTetradShapeSetForTeam( Implbits::TEAM_AWAY );
            BOOST_REQUIRE( ss.size() == ssdec.size() );
            for ( size_t i = 0; i < ss.size(); i++ )
            {
                BOOST_CHECK( ss[i] == ssdec[i] );
            }
        }
        Implbits::GameMechanics::Instance()->ApplyMove( board, move );
        lastmove = move;
    }
}
