using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPF
{
    /// <summary>
    /// Interaction logic for ColorSelection.xaml
    /// </summary>
    public partial class ColorSelection : UserControl
    {
        Dictionary<Color, Button> colorMap = new Dictionary<Color, Button>();
        public event EventHandler<ColorSelectionEventArgs> Dismissed;
        ResourceDictionary appResources;
        public ColorSelection()
        {
            InitializeComponent();
            DataContext = this;
            appResources = App.Mine.Resources;
            colorMap.Add((Color)Color.FromArgb(00, 00, 00, 00), null);
            colorMap.Add((Color)appResources["OrangeAccentColor"], this.Orange);
            colorMap.Add((Color)appResources["PurpleColor"], this.Purple);
            colorMap.Add((Color)appResources["RedColor"], this.Red);
            colorMap.Add((Color)appResources["GreenColor"], this.Green);
            colorMap.Add((Color)appResources["BlueColor"], this.Blue);
            colorMap.Add((Color)appResources["YellowColor"], this.Yellow);
        }

        public string Caption
        {
            get { return (string)GetValue(CaptionProperty); }
            set { SetValue(CaptionProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Caption.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CaptionProperty =
            DependencyProperty.Register("Caption", typeof(string), typeof(ColorSelection), new UIPropertyMetadata("Choose your color"));

        private void FireDismissed(Color? selected)
        {
            if (Dismissed != null)
                Dismissed(this, new ColorSelectionEventArgs(selected));
        }

        private void ColorClicked(Button b, SolidColorBrush brush)
        {
            FireDismissed(brush.Color);
        }

        public void DisableColor(Color c)
        {
            Button b = colorMap[c];
            if (b != null)
            {
                b.IsEnabled = false;
            }
            //Image redx = new Image();
            //redx.Source = new BitmapImage(new Uri("pack://application:,,,/Resources/dibsed.png"));
            //redx.Width = 100;
            //redx.Height = 100;
            //redx.Margin = new Thickness(10);
            //b.Content = redx;                    
        }

        private void OrangeClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["OrangeAccentColor"]);
        }

        private void PurpleClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["PurpleColor"]);
        }

        private void RedClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["RedColor"]);
        }

        private void GreenClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["GreenColor"]);
        }

        private void BlueClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["BlueColor"]);
        }

        private void YellowClicked(object sender, RoutedEventArgs e)
        {
            FireDismissed((Color)appResources["YellowColor"]);
        }


    }
}
