#pragma once

namespace TsWinRT
{

public ref class TsPosition sealed
{
public:
    TsPosition( int x, int y );
    virtual ~TsPosition();
    property int X;
    property int Y;
};

} // TsWinRT
