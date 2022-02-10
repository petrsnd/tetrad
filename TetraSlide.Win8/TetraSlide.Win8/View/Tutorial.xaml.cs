using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace TetraSlide.Win8.View
{
    public sealed partial class Tutorial : UserControl
    {
        public Tutorial()
        {
            this.InitializeComponent();
        }

        async private void Image_ImageOpened_1(object sender, RoutedEventArgs e)
        {
            MessageDialog dlg = new MessageDialog("Image opened");
            await dlg.ShowAsync();
        }

        async private void Image_ImageFailed_1(object sender, ExceptionRoutedEventArgs e)
        {
            MessageDialog dlg = new MessageDialog("Image failed:" + e.ErrorMessage );
            await dlg.ShowAsync();
        }
    }
}
