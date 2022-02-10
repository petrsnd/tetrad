using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
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
    public sealed partial class GameBoard : UserControl
    {
        public GameBoard()
        {
            this.InitializeComponent();
        }

        public Controls.Board GetBoard()
        {
            var g = this.DataContext as TetraSlide.Win8.ViewModel.Game;
            this.StatusText.Text = g.GameStatusText;
            return this.Board;
        }

        public void SetGameOverText(string gameovertext)
        {
            GameOverTextBlock.Text = gameovertext;
            GameOverTextBlock.Visibility = Visibility.Visible;
        }
    } 
}
