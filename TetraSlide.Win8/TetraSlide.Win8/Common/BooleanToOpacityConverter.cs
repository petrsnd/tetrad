using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;

namespace TetraSlide.Win8.Common
{
    class BooleanToOpacityConverter : IValueConverter
    {
        const double FADED_OUT = .4;
        const double FADED_IN = 1;
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            return (value is bool && (bool)value) ? FADED_IN : FADED_OUT;
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            throw new NotImplementedException();
        }
    }
}
