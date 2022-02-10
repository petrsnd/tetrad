#pragma once

#include "TsPosition.h"

namespace TsWinRT
{

public ref class TsTarget sealed
{
public:
    TsTarget( TsPosition^ pos, bool valid );
    virtual ~TsTarget();
    property TsPosition^ Pos;
    property bool Valid;
};

} // TsWinRT