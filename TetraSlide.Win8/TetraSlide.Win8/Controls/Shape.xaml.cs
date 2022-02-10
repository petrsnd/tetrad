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
    public sealed partial class Shape : UserControl
    {
        public Shape()
        {
            this.InitializeComponent();
            this.Designation = TsWinRT.TsTetradShape.TsTetradBackwardsL;
            this.Loaded += Shape_Loaded;
        }

        void Shape_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (Tile t in ShapeGrid.Children)
            {
                Binding binding = new Binding();
                binding.Source = this;
                binding.Path = new PropertyPath("Background");
                t.Rectangle.SetBinding(Rectangle.FillProperty, binding);
            }
        }

        public static int GetId(DependencyObject obj)
        {
            return (int)obj.GetValue(IdProperty);
        }
        public static void SetId(DependencyObject obj, int value)
        {
            obj.SetValue(IdProperty, value);
        }
        // Using a DependencyProperty as the backing store for Id.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IdProperty =
            DependencyProperty.RegisterAttached("Id", typeof(int), typeof(Shape), null);

        public TsWinRT.TsTetradShape Designation
        {
            get { return (TsWinRT.TsTetradShape)GetValue(DesignationProperty); }
            set { SetValue(DesignationProperty, value); }
        }
        // Using a DependencyProperty as the backing store for Designation.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty DesignationProperty =
            DependencyProperty.Register("Designation", typeof(TsWinRT.TsTetradShape), typeof(Shape), null);

        protected override Size ArrangeOverride(Size finalSize)
        {
            Size size = MeasureOverride(finalSize);
            double tileSize = GetTileSize(finalSize);
            foreach (Tile tile in ShapeGrid.Children)
            {
                switch (this.Designation)
                {
                    case TsWinRT.TsTetradShape.TsTetradBackwardsL:
                        ArrangeBackwardsL(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TsTetradDownStep:
                        ArrangeDownStep(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TsTetradLine:
                        ArrangeLine(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TsTetradLShape:
                        ArrangeLShape(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TsTetradPodium:
                        ArrangePodium(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TsTetradSquare:
                        ArrangeSquare(tile, tileSize); break;
                    case TsWinRT.TsTetradShape.TSTetradUpStep:
                        ArrangeUpStep(tile, tileSize); break;
                } 
            }
            return size;

        }

        protected override Size MeasureOverride(Size availableSize)
        {
            double tileSize = GetTileSize(availableSize);
            return new Size(tileSize * 4, tileSize * 2);
        }

        double GetTileSize(Size availableSize)
        {
            double w = availableSize.Width == double.PositiveInfinity ? 30 : availableSize.Width / 4;
            double h = availableSize.Height == double.PositiveInfinity ? 30 : availableSize.Height / 2;
            if (h < w)
                return h;
            else
                return w;
        }

        void ArrangeBackwardsL(Tile t, double size)
        {
            double half = size / 2;
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(half, 0, size, size)); break;
                case 1: t.Arrange(new Rect(half, size, size, size)); break;
                case 2: t.Arrange(new Rect(size + half, size, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2 + half, size, size, size)); break;
            }
        }

        void ArrangeDownStep(Tile t, double size)
        {
            double half = size / 2;
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(half, 0, size, size)); break;
                case 1: t.Arrange(new Rect(size + half, 0, size, size)); break;
                case 2: t.Arrange(new Rect(size + half, size, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2 + half, size, size, size)); break;
            }
        }
        
        void ArrangeLine(Tile t, double size)
        {
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(0, 0, size, size)); break;
                case 1: t.Arrange(new Rect(size, 0, size, size)); break;
                case 2: t.Arrange(new Rect(size * 2, 0, size, size)); break;
                case 3: t.Arrange(new Rect(size * 3, 0, size, size)); break;
            }
        }

        void ArrangeLShape(Tile t, double size)
        {
            double half = size / 2;
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(half, size, size, size)); break;
                case 1: t.Arrange(new Rect(size + half, size, size, size)); break;
                case 2: t.Arrange(new Rect(size * 2 + half, size, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2 + half, 0, size, size)); break;
            }
        }

        void ArrangePodium(Tile t, double size)
        {
            double half = size / 2;
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(half, size, size, size)); break;
                case 1: t.Arrange(new Rect(size + half, size, size, size)); break;
                case 2: t.Arrange(new Rect(size + half, 0, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2 + half, size, size, size)); break;
            }
        }

        void ArrangeSquare(Tile t, double size)
        {
            switch (GetId(t))
            {
                case 0:
                    
                    t.Arrange(new Rect(size, 0, size, size)); break;
                case 1: t.Arrange(new Rect(size, size, size, size)); break;
                case 2: t.Arrange(new Rect(size * 2, 0, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2, size, size, size)); break;
            }
            
        }

        void ArrangeUpStep(Tile t, double size)
        {
            double half = size / 2;
            switch (GetId(t))
            {
                case 0: t.Arrange(new Rect(half, size, size, size)); break;
                case 1: t.Arrange(new Rect(size + half, size, size, size)); break;
                case 2: t.Arrange(new Rect(size + half, 0, size, size)); break;
                case 3: t.Arrange(new Rect(size * 2 + half, 0, size, size)); break;
            }
        }
    }
}
