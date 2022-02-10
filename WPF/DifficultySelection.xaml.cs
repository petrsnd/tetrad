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
    /// Interaction logic for DifficultySelection.xaml
    /// </summary>
    public partial class DifficultySelection : UserControl
    {

        public event EventHandler<DifficultySelectionEventArgs> Dismissed;
        public DifficultySelection()
        {
            InitializeComponent();
        }

        private void FireDismissed(string difficulty)
        {
            if (Dismissed != null)
                Dismissed(this, new DifficultySelectionEventArgs(difficulty));
        }

        private void OnEasy(object sender, RoutedEventArgs e)
        {
            FireDismissed("beginner");
        }

        private void OnMedium(object sender, RoutedEventArgs e)
        {
            FireDismissed("intermediate");
        }

        private void OnHard(object sender, RoutedEventArgs e)
        {
            FireDismissed("expert");
        }

        private void OnBack(object sender, RoutedEventArgs e)
        {
            FireDismissed(null);
        }
    }
}
