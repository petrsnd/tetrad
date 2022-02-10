// ConsoleView.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Engine/Interfaces.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace Implbits;

class ConsoleView : public IView
{
public:
	virtual void OnGameStart(const GameBoard::Ptr& board) 
	{
		PrintBoard( board, std::cout );
		PlayTurn( std::cout );
        PrintBoard( board, std::cout );

        std::cout << "Press any key..." << std::endl;
        std::string any;
        std::getline( std::cin, any );
	}
	virtual void SetMutator( const std::shared_ptr< IGameMutator >& mutator )
	{
		m_mutator = mutator;
	}

private:
	std::weak_ptr< IGameMutator > m_mutator;
	void PrintBoard( const GameBoard::Ptr& board, std::ostream& out )
	{
		out << "    1   2   3   4" << std::endl << std::endl; 
		for( int y = 1; y <= board->SizeX(); y++ )
		{
			out << " " << y << " "; 
			for( int x = 1; x <= board->SizeY(); x++ )
			{
				out << '[';
				switch( board->GetPositionState( Position( x, y ) ) )
				{
				case STATE_MINE:
					out << 'X'; break;
				case STATE_THEIRS:
					out << 'O'; break;
				case STATE_EMPTY:
					out << ' '; break;
				case STATE_INVALID:
					out << '?'; break;
				}
				out << "] " ;
			}
			out << std::endl << std::endl;
		}
	}

	void PlayTurn( std::ostream& out )
	{
		out << "It's your turn. Select a tile to move." << std::endl;
		Position from, to;
		std::vector< Position > moves;
		while( moves.empty() )
		{
			from = PromptForPosition( out, "Enter tile coordinates (x,y): " );
			moves = m_mutator.lock()->GetValidMoves( from );
			if ( moves.empty() )
			{
				out << "You can't move that tile!" << std::endl;
			}
		}
		out << std::endl << "Valid moves: " << std::endl;
		std::for_each( begin( moves ), end( moves ), [&out] ( Position& pos )
		{
			out << "  (" << pos.x << "," << pos.y << ")" << std::endl;
		} );
		do 
		{
			to = PromptForPosition( out, "Enter target coordinates (x,y): " );
		} while ( !m_mutator.lock()->MoveTile( from, to ) );

	}

	Position PromptForPosition(std::ostream& out, const std::string& prompt )
	{
		out << prompt;
		std::string coords;
		std::getline( std::cin, coords );
		std::vector<std::string> strs;
		boost::split( strs, coords, boost::is_any_of( ", " ) );
		int x = boost::lexical_cast<int>( strs[0] );
		int y = boost::lexical_cast<int>( strs[1] );
		return Position( x, y );
	}

};


int _tmain(int argc, _TCHAR* argv[])
{
	auto view = std::make_shared<ConsoleView>();
	auto engine = IGameEngine::Create();
	view->SetMutator( engine->GetMutator() );
	engine->SetView(view);
	engine->Start();
	return 0;
}

