#pragma once

#include "TsDefinitions.h"
#include "TsTarget.h"
#include "TsPosition.h"

#include <Engine/Interfaces.h>

#include <collection.h>

namespace TsWinRT
{

public ref class TsTile sealed
{
internal:
    explicit TsTile( const Implbits::ITile::Ptr& tile );
public:
	virtual ~TsTile();
    property int Id
    {
        int get();
    }
    property TsPosition^ Pos
    {
        TsPosition^ get();
        void set( TsPosition^ pos );
    }
    property TsPlayerTeam Owner
    {
        TsPlayerTeam get();
    }
    Windows::Foundation::Collections::IVector< TsTarget^ >^ GetTargets();
internal:
    Implbits::ITile::Ptr GetPtr();
private:
    Implbits::ITile::Ptr m_tile;
};

} // TsWinRT
