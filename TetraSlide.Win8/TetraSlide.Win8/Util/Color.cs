using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TetraSlide.Win8.Util
{
    static class Color
    {
        public static int ColorToInt(Windows.UI.Color color)
        {
            var retval = (int)(((uint)color.A << 24) |
                ((uint)color.R << 16) |
                ((uint)color.G << 8) |
                (uint)color.B);
            return retval;
        }

        public static Windows.UI.Color IntToColor(int color) 
        {
            byte A = (byte)((color & 0xFF000000) >> 24);
            byte R = (byte)((color & 0x00FF0000) >> 16);
            byte G = (byte)((color & 0x0000FF00) >> 8);
            byte B = (byte)(color & 0x000000FF);
            var retval =  new Windows.UI.Color() { A = A, R = R, G = G, B = B };
            return retval;
        }
    }
}
