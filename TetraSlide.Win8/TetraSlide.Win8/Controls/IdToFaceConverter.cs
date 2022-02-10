using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Data;

namespace TetraSlide.Win8.Controls
{
    class IdToFaceConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            if (value != null && value is Tile)
            {
                return GetImageUri(value as Tile);
            }
            return null;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            throw new NotImplementedException();
        }

        public Uri GetImageUri(Tile t)
        {
            if (t.Expression == Expression.NONE) return null;
            int id = t.Id;
            if (id < 0) return null;
            int idchar = (int)id % 6;
            char ch = (char)((int)'A' + idchar);
            string uristr = "pack://application:,,,/Assets/" + string.Format("{0}-0{1}.png", ch, (int)t.Expression);
            return new Uri(uristr);
        }
    }
}
