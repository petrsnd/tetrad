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
using System.Windows.Shapes;

namespace WPF
{
    /// <summary>
    /// Interaction logic for PlayerInfoDlg.xaml
    /// </summary>
    public partial class PlayerInfo : UserControl
    {
        event EventHandler<PlayerInfoEventArgs> Dismissed;

        public PlayerInfo()
        {
            InitializeComponent();
            DataContext = this;
        }

        public string Player1
        {
            get { return (string)GetValue(Player1Property); }
            set { SetValue(Player1Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player1.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player1Property =
            DependencyProperty.Register("Player1", typeof(string), typeof(PlayerInfo), new UIPropertyMetadata("Player 1"));

        public string Player2
        {
            get { return (string)GetValue(Player2Property); }
            set { SetValue(Player2Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player2.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player2Property =
            DependencyProperty.Register("Player2", typeof(string), typeof(PlayerInfo), new UIPropertyMetadata("Player 2"));

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }


        private void FireDismissed()
        {
            if (Dismissed != null)
            {
                Dismissed(this, new PlayerInfoEventArgs(this));
            }

        }

    }
}
