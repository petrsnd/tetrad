#include "StatsCalculator.h"
#include <algorithm>
#include <numeric>

StatsCalculator::Ptr StatsCalculator::CreateStatsCalculator( const GameDataListPtr& games )
{
    return StatsCalculator::Ptr( new StatsCalculator( games ) );
}
StatsCalculator::StatsCalculator( const GameDataListPtr& games ) :
    m_games( games )
{}
int StatsCalculator::NumGames() const
{
    return static_cast< int >( m_games->size() );
}
int StatsCalculator::NumWins() const
{
    return static_cast< int >( std::count_if( m_games->begin(), m_games->end(),
                                   []( const GameData& data ) -> bool { return data.Winner; } ) );
}
int StatsCalculator::NumLosses() const
{
    return static_cast< int >( std::count_if( m_games->begin(), m_games->end(),
                                   []( const GameData& data ) -> bool { return data.Winner; } ) );
}
double StatsCalculator::AverageNumMoves() const
{
    int totalMoves = std::accumulate( m_games->begin(), m_games->end(), 0,
                         []( int i, const GameData& data ) -> int { return i + data.NumMoves; } );
    return totalMoves / NumGames();
}
int StatsCalculator::Rating() const
{
    return std::accumulate( m_games->begin(), m_games->end(), 100,
               []( int i, const GameData& data ) -> int
               {
                   return i + ( data.Winner ? 2 ^ data.OpponentLevel: 0 )
                       + ( data.Winner ? static_cast< int >( 1000 / data.NumMoves ) : static_cast< int >( data.NumMoves / 25 ) );
               } );
}
