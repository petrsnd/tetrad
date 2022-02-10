using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public static class DoubleExtensions
{
    const double _3 = 0.001;
    const double _4 = 0.0001;

    public static bool IsWithin20(this double left, double right)
    {
        return Math.Abs(left - right) < 20;
    }

    public static bool Equals3DigitPrecision(this double left, double right)
    {
        return Math.Abs(left - right) < _3;
    }

    public static bool Equals4DigitPrecision(this double left, double right)
    {
        return Math.Abs(left - right) < _4;
    }
}
