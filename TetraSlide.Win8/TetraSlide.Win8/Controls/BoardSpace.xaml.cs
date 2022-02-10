using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace TetraSlide.Win8.Controls
{
    public sealed partial class BoardSpace : UserControl
    {
        static Brush HightlightBrush = new SolidColorBrush(Colors.LawnGreen);
        
        public BoardSpace()
        {
            this.InitializeComponent();
        }

        public void SetTile(Tile tile)
        {
            this.TileBorder.Child = tile;
        }

        public bool Targeted
        {
            get { return (bool)GetValue(TargetedProperty); }
            set { SetValue(TargetedProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Targeted.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TargetedProperty =
            DependencyProperty.Register("Targeted", typeof(bool), typeof(BoardSpace), new PropertyMetadata(false,
                new PropertyChangedCallback((obj,args) =>{
                    BoardSpace space = obj as BoardSpace;
                    if( space == null ) return;
                    if( (bool)args.NewValue && !space.Invalid )
                    {
                        space.ValidDecoration.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        space.ValidDecoration.Visibility = Visibility.Collapsed;
                    }
                })));

        public bool Invalid
        {
            get { return (bool)GetValue(InvalidProperty); }
            set { SetValue(InvalidProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Targeted.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty InvalidProperty =
            DependencyProperty.Register("Invalid", typeof(bool), typeof(BoardSpace), new PropertyMetadata(false,
                new PropertyChangedCallback((obj, args) =>
                {
                    BoardSpace space = obj as BoardSpace;
                    if (space == null) return;
                    if (space.Targeted)
                        space.Targeted = false;
                    if ((bool)args.NewValue)
                    {
                        space.InvalidDecoration.Visibility = Visibility.Visible;
                    }
                    else
                    {
                        space.InvalidDecoration.Visibility = Visibility.Collapsed;
                    }
                })));
    }
}
