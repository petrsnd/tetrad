#pragma once
#include <memory>
#include <string>
#include <vector>

struct GameData
{
    bool Winner;
    int OpponentLevel;
    int NumMoves;
};
typedef std::vector< GameData > GameDataList;
typedef std::shared_ptr< GameDataList > GameDataListPtr;

GameDataListPtr ParseGameData( const std::string& json );
std::string SerializeGameData( const GameDataListPtr& games );
