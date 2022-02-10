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
using Windows.UI.Xaml.Shapes;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace TetraSlide.Win8.Controls
{
    public sealed partial class Tile : UserControl
    {
        public Tile()
        {
            this.InitializeComponent();
            Id = -1;            
        }

        public Rectangle Rectangle
        {
            get { return TileRect; }
        }
        
        public Expression Expression
        {
            get { return (Expression)GetValue(ExpressionProperty); }
            set { SetValue(ExpressionProperty, value); }
        }
        // Using a DependencyProperty as the backing store for Expression.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ExpressionProperty =
            DependencyProperty.Register("Expression", typeof(Expression), typeof(Tile), null);

        public int Id
        {
            get { return (int)GetValue(IdProperty); }
            set { SetValue(IdProperty, value); }
        }
        // Using a DependencyProperty as the backing store for Id.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IdProperty =
            DependencyProperty.Register("Id", typeof(int), typeof(Tile), null);
    }

    public enum Expression
    {
        NONE = 0,
        NORMAL = 1,
        HAPPY,
        ONOES,
        RIGHT,
        LEFT,
        BLINK
    };

}
