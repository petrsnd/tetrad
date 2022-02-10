#ifdef _MSC_VER
#include "stdafx.h"
#endif
#include "Definitions.h"


namespace Implbits {

BoardDirection& operator++( BoardDirection& d )
{
    assert( d != DIR_END );
    d = static_cast< BoardDirection >( d + 1 );
    return d;
}
BoardDirection operator++( BoardDirection& d, int )
{
    assert( d != DIR_END); 
    ++d;
    return static_cast< BoardDirection >( d - 1 );
}
BoardDirection operator*( BoardDirection d )
{
    assert( d != DIR_END );
    return d;
}

TetradShape& operator++( TetradShape& s )
{
    assert( s != TETRAD_END );
    s = static_cast< TetradShape >( s + 1 );
    return s;
}
TetradShape operator++( TetradShape& s, int )
{
    assert( s != TETRAD_END); 
    ++s;
    return static_cast< TetradShape >( s - 1 );
}
TetradShape operator*( TetradShape s )
{
    assert( s != TETRAD_END );
    return s;
}

} // Implbits
