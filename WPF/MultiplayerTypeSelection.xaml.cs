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
    /// Interaction logic for MultiplayerTypeSelection.xaml
    /// </summary>
    public partial class MultiplayerTypeSelection : UserControl
    {
        public MultiplayerTypeSelection(Main main)
        {
            InitializeComponent();
            MainWindow = main;
        }

        public Main MainWindow
        {
            get;
            private set;
        }

        private void OnPassAndPlay(object sender, RoutedEventArgs e)
        {

        }
    }
}
