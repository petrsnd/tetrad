using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;

namespace TetraSlide.Win8.ViewModel
{
    public class Tile : DependencyObject
    {
        
        public Tile(TsWinRT.TsTile t, Player owner)
        {
            Init(t, owner);
        }

        public void Init(TsWinRT.TsTile tile, Player owner)
        {
            Owner = owner;
            Id = tile.Id;
            Row = tile.Pos.Y;
            Column = tile.Pos.X;
            TeamBrush = owner.TeamBrush;
            EngineTile = tile;
        }

        public TsWinRT.TsTile EngineTile
        {
            get;
            private set;
        }

        public Player Owner
        {
            get { return (Player)GetValue(OwnerProperty); }
            set { SetValue(OwnerProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Owner.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty OwnerProperty =
            DependencyProperty.Register("Owner", typeof(Player), typeof(Tile), null);

        public int Id
        {
            get { return (int)GetValue(IdProperty); }
            set { SetValue(IdProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Id.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IdProperty =
            DependencyProperty.Register("Id", typeof(int), typeof(Tile), null);

        public int Row
        {
            get { return (int)GetValue(RowProperty); }
            set { SetValue(RowProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Row.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty RowProperty =
            DependencyProperty.Register("Row", typeof(int), typeof(Tile), null);

        public int Column
        {
            get { return (int)GetValue(ColumnProperty); }
            set { SetValue(ColumnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Column.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ColumnProperty =
            DependencyProperty.Register("Column", typeof(int), typeof(Tile), null);

        public Brush TeamBrush
        {
            get { return (Brush)GetValue(TeamBrushProperty); }
            set { SetValue(TeamBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TeamBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TeamBrushProperty =
            DependencyProperty.Register("TeamBrush", typeof(Brush), typeof(Tile), null);

        public TsWinRT.TsPosition GetBoardPosition()
        {
            return new TsWinRT.TsPosition(Column, Row);
        }

        public IList<TsWinRT.TsTarget> GetTargets()
        {
            return EngineTile.GetTargets();
        }
    }
}
