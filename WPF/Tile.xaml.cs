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
using System.Windows.Threading;

namespace WPF
{
    /// <summary>
    /// Interaction logic for Tile.xaml
    /// </summary>
    public partial class Tile : Border
    {
        DispatcherTimer timer = new DispatcherTimer();
        static Random rng = new Random();
        public Tile()
        {
            InitializeComponent();
            Face = new TileFace(this);
            DataContext = this;
            timer.Interval = new TimeSpan(0, 0, 0, 0, rng.Next(500, 15000));
            timer.Tick +=new EventHandler(timer_Tick);
            timer.Start();
        }

        void timer_Tick(object sender, EventArgs e)
        {
            if (Face.Expression == Expression.NONE) return;
            if (Face.Expression == Expression.NORMAL)
            {
                Face = new TileFace(this) { Expression = Expression.BLINK};
                timer.Interval = new TimeSpan(0, 0, 0, 0, rng.Next(300, 500));
            }
            else if (Face.Expression == Expression.BLINK)
            {
                Face = new TileFace(this) { Expression = Expression.NORMAL };
                timer.Interval = new TimeSpan(0, 0, 0, 0, rng.Next(5000, 15000));
            }
        }

        public ObjectModel.PlayerTeam Team
        {
            get { return (ObjectModel.PlayerTeam)GetValue(TeamProperty); }
            set { SetValue(TeamProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Team.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TeamProperty =
            DependencyProperty.Register("Team", typeof(ObjectModel.PlayerTeam), typeof(Tile), new UIPropertyMetadata(ObjectModel.PlayerTeam.Home));

        public TileFace Face
        {
            get { return (TileFace)GetValue(FaceProperty); }
            set { SetValue(FaceProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Face.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FaceProperty =
            DependencyProperty.Register("Face", typeof(TileFace), typeof(Tile));

        public void SetExpression(Expression ex)
        {
            Face = new TileFace(this) { Expression = ex };
        }
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

    public class TileFace
    {
        public TileFace( Tile tile )
        {
            Tile = tile;
            Expression = Expression.NONE;
        }
        public Tile Tile { get; private set; }
        public Expression Expression { get; set; }

        public Uri GetImageUri()
        {
            if (Expression == Expression.NONE) return null;
            int id = (int)Tile.GetValue(Board.IdProperty) ;
            if (id < 0) return null;
            int idchar = (int)id % 6;
            char ch = (char)((int)'A' + idchar);
            string uristr = "pack://application:,,,/Resources/" + string.Format("{0}-0{1}.png", ch, ExpressionToId(Expression) );
            return new Uri(uristr);
        }

        int ExpressionToId(Expression ex)
        {
            return (int)ex;
        }
    }
}
