#include "pch.h"
#include "TsTarget.h"

namespace TsWinRT
{

TsTarget::TsTarget( TsPosition^ pos, bool valid )
{
    Pos = pos;
    Valid = valid;
}
TsTarget::~TsTarget()
{}

} // TsWinRT