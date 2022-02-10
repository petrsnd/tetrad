using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace WPF
{
    class UIHelper
    {
        public static IEnumerable<T> FindChildrenOfType<T>(DependencyObject obj) where T : DependencyObject
        {
            List<T> retval = new List<T>();
            if( obj == null )
                return retval;
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(obj); i++)
            {
                DependencyObject child = VisualTreeHelper.GetChild(obj, i);
                if (child is T)
                {
                    retval.Add(child as T);
                }                
            }
            return retval;
        }

        public static IEnumerable<T> FindDescendantsOfType<T>(DependencyObject obj) where T : DependencyObject
        {
            List<T> retval = new List<T>(FindChildrenOfType<T>(obj));
            foreach (DependencyObject child in retval)
            {
                retval.AddRange(FindDescendantsOfType<T>(child));
            }
            return retval;
        }
    }
}
