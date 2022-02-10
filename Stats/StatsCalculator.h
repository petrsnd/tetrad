#pragma once
#include  "GameData.h"

#include <memory>
#include <string>

class StatsCalculator
{
public:
    typedef std::shared_ptr< StatsCalculator > Ptr;
    static StatsCalculator::Ptr CreateStatsCalculator( const GameDataListPtr& json );
    int NumGames() const;
    int NumWins() const;
    int NumLosses() const;
    double AverageNumMoves() const;
    int Rating() const;
private:
    explicit StatsCalculator( const GameDataListPtr& games );
    GameDataListPtr m_games;
};
