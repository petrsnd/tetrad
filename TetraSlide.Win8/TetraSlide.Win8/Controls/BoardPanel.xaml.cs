using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Automation.Peers;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace TetraSlide.Win8.Controls
{
    public sealed partial class BoardPanel : Grid
    {
        public BoardPanel()
        {
            this.InitializeComponent();
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            Size squareSize = MeasureOverride(finalSize);
            double tileSize = squareSize.Width / 4;
            foreach (ContentPresenter cp in Children)
            {
                var t = cp.Content as ViewModel.Tile;
                int tileRow = t.Row;
                int tileCol = t.Column;
                double x = tileCol * tileSize;
                double y = tileRow * tileSize;
                cp.Arrange(new Rect(x, y, tileSize, tileSize));
            }
            return squareSize;
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            double h = 0;
            double w = 0;
            if (availableSize.Height == double.PositiveInfinity)
                h = 300;
            else
                h = availableSize.Height;

            if (availableSize.Width == double.PositiveInfinity)
                w = 300;
            else
                w = availableSize.Width;
            if (w > h)
                return new Size(h, h);
            else
                return new Size(w, w);
        }
    }
}
