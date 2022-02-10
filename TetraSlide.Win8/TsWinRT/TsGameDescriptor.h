#pragma once

#include "TsDefinitions.h"
#include "TsPlayer.h"
#include "TsTile.h"

#include <Engine/Interfaces.h>

#include <collection.h>

namespace TsWinRT
{

public ref class TsGameDescriptor sealed
{
internal:
    explicit TsGameDescriptor( const Implbits::IGameDescriptor::Ptr& game );
public:
    virtual ~TsGameDescriptor();
    property Platform::String^ Id
    {
        Platform::String^ get();
    }
    property Platform::String^ FriendlyName
    {
        Platform::String^ get();
    }
    property Windows::Foundation::Collections::IVector< TsTile^ >^ Tiles
    {
        Windows::Foundation::Collections::IVector< TsTile^ >^ get();
    }
    property TsPlayer^ HomePlayer
    {
        TsPlayer^ get();
    }
    property TsPlayer^ AwayPlayer
    {
        TsPlayer^ get();
    }
    property TsPlayerTeam Turn
    {
        TsPlayerTeam get();
    }
internal:
    Implbits::IGameDescriptor::Ptr GetPtr();
private:
    Implbits::IGameDescriptor::Ptr m_game;
};

} // TsWinRT
