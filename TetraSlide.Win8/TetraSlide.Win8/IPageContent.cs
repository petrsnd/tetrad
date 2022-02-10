using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;

namespace TetraSlide.Win8
{
    public interface IPageContent
    {
        FrameworkElement Content { get; }
        FrameworkElement SnappedContent { get; }
        DependencyObject ViewModel { get; }
    }
}
