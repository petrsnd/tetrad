using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


#if NETFX_CORE
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;
using TetraSlide.Win8.Controls;
namespace TetraSlide.Win8
#else
using System.Windows;
using System.Windows.Data;
namespace WPF
#endif
{
    class BoolToTetradOpacityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            if (value == null || !(bool)value)
                return .15;
            return 1;
        }

        public object ConvertBack(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            throw new NotImplementedException();
        }
    }

    

    class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            bool hide = value == null || !(bool)value;
            if (parameter != null && Boolean.Parse(parameter as string))
            {
                hide = !hide;
            }
            if (hide)
                return Visibility.Collapsed;
            return Visibility.Visible;
        }

        public object ConvertBack(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            throw new NotImplementedException();
        }
    }

    #if !NETFX_CORE     
    class IsRemotePlayerToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            PlayerViewModel pvm = value as PlayerViewModel;
            if (pvm == null)
                return Visibility.Collapsed;
            return pvm.EnginePlayer.Type == ObjectModel.PlayerType.Remote ? Visibility.Visible : Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
    #endif

    class BooleanNotConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            return !(bool)value;
        }

        public object ConvertBack(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            throw new NotImplementedException();
        }
    }

    class StringToBoolConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            return value is string && !string.IsNullOrEmpty(value as string);

        }

        public object ConvertBack(object value, Type targetType, object parameter,
        #if NETFX_CORE
        string language)
        #else
         System.Globalization.CultureInfo culture)
        #endif
        {
            throw new NotImplementedException();
        }
    }
}
