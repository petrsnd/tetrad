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
using System.Collections;
using System.Windows.Media.Animation;
using System.Collections.ObjectModel;

namespace WPF
{
    
    public class Board : Panel
    {
        Point _startPoint;
        bool _isDown;
        DragConstraints _dragConstraints;
        TargetAdorner _targetOverlay;
        List<Tile> _adjacentDragTiles = new List<Tile>();

        public event EventHandler<TileSelectedEventArgs> TileSelected;
        public event EventHandler<TileMovedEventArgs> TileMoved;

        public Board()
        {
            _isDown = false;
            AllowDrop = true;
        }

        public void SetTiles( IEnumerable<Tile> tiles )
        {
            Children.Clear();
            foreach(Tile t in tiles )
            {
                Children.Add( t );
            }
        }

        public void SetEndGameTiles(ObjectModel.PlayerTeam winner)
        {
            foreach (Tile t in Children)
            {
                if (t.Team == winner)
                    t.SetExpression(Expression.HAPPY);
                else
                    t.SetExpression(Expression.ONOES);

            }
        }

        public ObservableCollection<ObjectModel.IPosition> Targets
        {
            get { return (ObservableCollection<ObjectModel.IPosition>)GetValue(TargetsProperty); }
            set { SetValue(TargetsProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Targets.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TargetsProperty =
            DependencyProperty.Register("Targets",
            typeof(ObservableCollection<ObjectModel.IPosition>), 
            typeof(Board),
            new UIPropertyMetadata(new ObservableCollection<ObjectModel.IPosition>()));

        public ObjectModel.PlayerTeam CurrentPlayer
        {
            get { return (ObjectModel.PlayerTeam)GetValue(CurrentPlayerProperty); }
            set { SetValue(CurrentPlayerProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentPlayer.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentPlayerProperty =
            DependencyProperty.Register("CurrentPlayer", typeof(ObjectModel.PlayerTeam), typeof(Board), new UIPropertyMetadata(ObjectModel.PlayerTeam.Home));

        ObjectModel.Direction m_dragDir;
        ObjectModel.Direction DragDir
        {
            get
            {
                return m_dragDir;
            }
            set
            {
                m_dragDir = value;
            }
        }

        void OnTileSelected(Tile tile)
        {
            if (TileSelected != null)
            {
                TileSelected(this, new TileSelectedEventArgs(tile, 
                    ObjectModel.Factory.NewPosition(GetX(tile), GetY(tile))));
            }
        }

        Tile FindTileElement(object item)
        {
            if (item is Tile) return item as Tile;
            if (item is FrameworkElement)
            {
                
                FrameworkElement child = item as FrameworkElement;
                FrameworkElement ancestor = child.Parent as FrameworkElement;
                while (ancestor != null )
                {
                    if( ancestor is Tile ) return ancestor as Tile;
                    ancestor = ancestor.Parent as FrameworkElement;
                }
            }
            return null;
        }

        void PointerDown()
        {
            if (DragSource.Team != CurrentPlayer)
                return;
            OnTileSelected(DragSource);
            if (Targets.Count == 0)
                return;
            _targetOverlay = new TargetAdorner(this, CalculateTargetRectangles());
            AdornerLayer layer = AdornerLayer.GetAdornerLayer(this);
            layer.Add(_targetOverlay);
            _isDown = true;
        }

        protected override void OnPreviewTouchDown(TouchEventArgs e)
        {
            base.OnPreviewTouchDown(e);
            if (_isDown) return;
            DragSource = FindTileElement(e.Source);
            if (DragSource != null)
            {
                PointerDown();
            }
            e.Handled = true;
        }

        protected override void OnPreviewTouchUp(TouchEventArgs e)
        {
            base.OnPreviewTouchUp(e);
            DragFinished(false);
        }

        protected override void OnPreviewMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnPreviewMouseLeftButtonDown(e);
            if (_isDown) return;
            DragSource = FindTileElement(e.Source);
            
            if (DragSource != null)
            {
                PointerDown();
            }
            e.Handled = true;
        }

        protected override void OnPreviewMouseLeftButtonUp(MouseButtonEventArgs e)
        {
            base.OnPreviewMouseLeftButtonUp(e);
            DragFinished(false);
        }

        protected override void OnPreviewMouseMove(MouseEventArgs e)
        {
            base.OnPreviewMouseMove(e);
            if (_isDown)
            {
                if ((_dragConstraints == null) && ((Math.Abs(e.GetPosition(this).X - _startPoint.X) > SystemParameters.MinimumHorizontalDragDistance) ||
                    (Math.Abs(e.GetPosition(this).Y - _startPoint.Y) > SystemParameters.MinimumVerticalDragDistance)))
                {
                    DragStarted(e);
                }
                if (_dragConstraints != null)
                {
                    DragMove(e);
                }
            }
        }

        Tile DragSource { get; set; }

        private void DragStarted(MouseEventArgs e)
        {
            if (DragSource != null && Targets.Count > 0)
            {
                _startPoint = e.GetPosition(this);
                _dragConstraints = new DragConstraints(PointToPosition(_startPoint), Targets, ActualWidth / BoardSize);

                // Determine the drag direction and the tiles involved
                DragSource.RenderTransform = new TranslateTransform();
                CaptureMouse();
            }
        }

        private void DragMove(MouseEventArgs e)
        {
            if (DragSource == null)
            {
                DragFinished(true);
                return;
            }
            TranslateTransform tt = DragSource.RenderTransform as TranslateTransform;
            if (tt == null)
            {
                DragFinished(true);
                return;
            }
            Point current = e.GetPosition(this);
            ObjectModel.IPosition tilePositionOrig = PointToPosition(_startPoint);
            if (PresentationSource.FromVisual(DragSource) == null)
            {
                DragFinished(true);
                return;
            }
            Point screenPoint = DragSource.PointToScreen(new Point(tt.X, tt.Y));
            ObjectModel.IPosition tilePosition = PointToPosition(this.PointFromScreen(screenPoint));
            if (DragDir == ObjectModel.Direction.None || tilePositionOrig.Equals(tilePosition))
            {
                ObjectModel.Direction newDir = CalculateDragDirection(_startPoint, current);
                if (IsValidDirection(tilePositionOrig, newDir))
                {
                    
                    DragDir = newDir;
                    tt.X = 0;
                    tt.Y = 0;
                    foreach (Tile t in _adjacentDragTiles)
                    {
                        t.RenderTransform = null;
                    }
                    _adjacentDragTiles = FindAdjacentTiles(tilePositionOrig, DragDir);
                    foreach (Tile t in _adjacentDragTiles)
                    {
                        t.RenderTransform = DragSource.RenderTransform;
                    }
                }
            }
            Point dif = DiffPoints(_startPoint, current);
            
            if (DragDir == ObjectModel.Direction.Left ||
                DragDir == ObjectModel.Direction.Right)
            {
                if (dif.X > _dragConstraints.MinX && dif.X < _dragConstraints.MaxX)
                {
                    tt.X = dif.X;
                }
                else
                {
                    // Create and show an X adorner?
                }
            }
            if (DragDir == ObjectModel.Direction.Up ||
                DragDir == ObjectModel.Direction.Down)
            {
                if (dif.Y > _dragConstraints.MinY && dif.Y < _dragConstraints.MaxY)
                {
                    tt.Y = dif.Y;
                }
                else
                {
                    // Create and show an X adorner?
                }
            }
        }

        private void DragFinished(bool cancelled)
        {
            if (_dragConstraints != null && DragSource != null)
            {
                TranslateTransform tt = DragSource.RenderTransform as TranslateTransform;
                Point p = new Point(_startPoint.X, _startPoint.Y);
                if (tt != null)
                {
                    p.Offset(tt.X, tt.Y);
                }

                var pos = PointToPosition(p);
                if (IsValidPosition(pos))
                {
                    if (!cancelled)
                    {
                        var start = ObjectModel.Factory.NewPosition(GetX(DragSource), GetY(DragSource));
                        SetX(DragSource, pos.X);
                        SetY(DragSource, pos.Y);
                        if (TileMoved != null)
                        {
                            TileMoved(this, new TileMovedEventArgs(start, pos));
                        }
                    }
                }
                DragSource.RenderTransform = null;
                this.InvalidateArrange();
                UpdateLayout();
            }
            _dragConstraints = null;
            _isDown = false;
            CleanUpTargetOverlay();
            ReleaseMouseCapture();
            DragDir = ObjectModel.Direction.None;
        }

        public static int GetX(DependencyObject obj)
        {
            return (int)obj.GetValue(XProperty);
        }
        public static void SetX(DependencyObject obj, int value)
        {
            obj.SetValue(XProperty, value);
        }
        // Using a DependencyProperty as the backing store for X.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty XProperty =
            DependencyProperty.RegisterAttached("X", typeof(int), typeof(Board), new UIPropertyMetadata(0));

        public static int GetY(DependencyObject obj)
        {
            return (int)obj.GetValue(YProperty);
        }
        public static void SetY(DependencyObject obj, int value)
        {
            obj.SetValue(YProperty, value);
        }
        // Using a DependencyProperty as the backing store for Y.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty YProperty =
            DependencyProperty.RegisterAttached("Y", typeof(int), typeof(Board), new UIPropertyMetadata(0));


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
            DependencyProperty.RegisterAttached("Id", typeof(int), typeof(Board), new UIPropertyMetadata(0));

        public Brush HomeColor
        {
            get { return (Brush)GetValue(HomeColorProperty); }
            set { SetValue(HomeColorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HomeColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HomeColorProperty =
            DependencyProperty.Register("HomeColor", typeof(Brush), typeof(Board), new UIPropertyMetadata(new SolidColorBrush(Colors.White)));

        public Brush AwayColor
        {
            get { return (Brush)GetValue(AwayColorProperty); }
            set { SetValue(AwayColorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for AwayColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty AwayColorProperty =
            DependencyProperty.Register("AwayColor", typeof(Brush), typeof(Board), new UIPropertyMetadata(new SolidColorBrush(Colors.Black)));

        public Brush GridLineColor
        {
            get { return (Brush)GetValue(GridLineColorProperty); }
            set { SetValue(GridLineColorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for GridLineColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GridLineColorProperty =
            DependencyProperty.Register("GridLineColor", typeof(Brush), typeof(Board), new UIPropertyMetadata(new SolidColorBrush(Colors.Gray)));


        public double TileMargin
        {
            get { return (double)GetValue(TileMarginProperty); }
            set { SetValue(TileMarginProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TileMargin.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TileMarginProperty =
            DependencyProperty.Register("TileMargin", typeof(double), typeof(Board), new UIPropertyMetadata((double)0));

        public double GridLineThickness
        {
            get { return (double)GetValue(GridLineThicknessProperty); }
            set { SetValue(GridLineThicknessProperty, value); }
        }

        // Using a DependencyProperty as the backing store for GridLineThickness.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GridLineThicknessProperty =
            DependencyProperty.Register("GridLineThickness", typeof(double), typeof(Board), new UIPropertyMetadata((double)0));

        string GetDirectionString()
        {
            if (_dragConstraints != null)
            {
                switch (DragDir)
                {
                    case ObjectModel.Direction.Up: return "UP";
                    case ObjectModel.Direction.Down: return "DOWN";
                    case ObjectModel.Direction.Left: return "LEFT";
                    case ObjectModel.Direction.Right: return "RIGHT";
                }
            }
            return null;
        }


        public int BoardSize { get { return 4; } }

        public Size TileSize
        {
            get;
            set;
        }

        Tile FindTile(ObjectModel.IPosition pos)
        {
            foreach( Tile t in this.Children )
            {
                if( GetX(t) == pos.X && GetY(t) == pos.Y)
                    return t;
            }
            return null;
        }

        Tile FindTile(ObjectModel.ITile t)
        {
            return FindTileById(t.Id);
        }

        public Tile FindTileById(int id)
        {
            foreach (Tile tile in this.Children)
            {
                int tileId = GetId(tile);
                if (tileId == id)
                    return tile;
            }
            return null;
        }

        List<Tile> FindAdjacentTiles(ObjectModel.IPosition source, ObjectModel.Direction dir)
        {
            List<Tile> tiles = new List<Tile>();
            if (source == null || dir == ObjectModel.Direction.None) return tiles;
            int y = source.Y;
            int x = source.X;
            Action incrementer = null;
            switch (dir)
            {
                case ObjectModel.Direction.Up:
                    incrementer = new Action(() => --y); break;
                case ObjectModel.Direction.Down:
                    incrementer = new Action(() => ++y); break;
                case ObjectModel.Direction.Left:
                    incrementer = new Action(() => --x); break;
                case ObjectModel.Direction.Right:
                    incrementer = new Action(() => ++x); break;
                default:
                    return tiles;
            }
            while (true)
            {
                incrementer();
                var t = FindTile(ObjectModel.Factory.NewPosition(x, y));
                if (t == null) return tiles;
                tiles.Add(t);
            }
        }

        IEnumerable<Move> GetMoves(IEnumerable<int> tiles, ObjectModel.Direction dir, byte distance)
        {
            foreach (int tileId in tiles)
            {
                Tile boardTile = FindTileById(tileId);
                if (boardTile != null)
                {
                    yield return new Move(ObjectModel.Factory.NewPosition(GetX(boardTile), GetY(boardTile)), dir, distance);
                }
            }
        }

        public void PerformMove( IEnumerable<int> tileIds, ObjectModel.Direction d, byte mag, Action callback)
        {
            // Create a name scope for the page.
            StringBuilder logMsg = new StringBuilder();
            //logMsg.AppendFormat("Animating move: Direction={0}, Magnitude={1}, Tiles={2}",
            //    Logger.DirectionToString( d), mag.ToString(), Logger.TilesToStringList(tileIds));
            Logger.Log(logMsg.ToString());
            NameScope.SetNameScope(this, new NameScope());
            Storyboard moveSb = new Storyboard();
            ParallelTimeline pt = new ParallelTimeline();
            List<Tile> movedTiles = new List<Tile>();
            List<string> transformNames = new List<string>();
            foreach (Move move in GetMoves(tileIds, d, mag))
            {
                ObjectModel.IPosition from = move.From;
                ObjectModel.IPosition to = move.To;
                Duration duration = new Duration(new TimeSpan(0, 0, 0, 0, 500));
                ObjectModel.Direction dir = move.Direction;
                int distance = move.Distance;

                // What is the direction we are moving in?
                double magnitude = 1;
                DependencyProperty animateProperty = TranslateTransform.YProperty;
                switch (dir)
                {
                    case ObjectModel.Direction.Left:
                        animateProperty = TranslateTransform.XProperty;
                        magnitude = -1;
                        break;
                    case ObjectModel.Direction.Right:
                        animateProperty = TranslateTransform.XProperty;
                        break;
                    case ObjectModel.Direction.Up:
                        magnitude = -1;
                        break;
                }
                var tile = FindTile(ObjectModel.Factory.NewPosition(from.X, from.Y));
                var T = new TranslateTransform();
                string transformName = string.Format("TileTransform{0}{1}", from.X, from.Y);
                transformNames.Add(transformName);
                try
                {
                    this.RegisterName(transformName, T);
                }
                catch (System.ArgumentException ex) {
                    Logger.Log("** CAUGHT EXCEPTION SLIDING ANIMATION: TransformName=" + transformName + "\r\n\t" + ex.Message);
                }
                tile.RenderTransform = T;
                movedTiles.Add(tile);
                DoubleAnimation anim = new DoubleAnimation(magnitude * (TileSize.Width * distance), duration);
                Storyboard.SetTargetName(anim, transformName);
                Storyboard.SetTargetProperty(anim, new PropertyPath(animateProperty));
                pt.Children.Add(anim);

            }
            pt.Completed += new EventHandler(
                new Action<object, EventArgs>(delegate(object sender, EventArgs e)
                {
                    //foreach (Tile t in movedTiles)
                    //    t.RenderTransform = null;
                    if (callback != null)
                    {
                        callback();
                    }
                }));
            moveSb.Children.Add(pt);
            moveSb.Begin(this);
            foreach (string name in transformNames)
            {
                this.UnregisterName(name);
            }
        }

        // Override the default Measure method of Panel
        protected override Size MeasureOverride(Size availableSize)
        {
            if (availableSize.Width == double.PositiveInfinity) availableSize.Width = 300;
            if (availableSize.Height == double.PositiveInfinity) availableSize.Height = 300;
            Size desiredSize = new Size(availableSize.Height, availableSize.Height);
            if (availableSize.Width < availableSize.Height)
            {
                desiredSize = new Size(availableSize.Width, availableSize.Width);
            }
            return desiredSize;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            Size actualSize = MeasureOverride(finalSize);
            double dimension = (actualSize.Height - ((BoardSize + 1) * TileMargin)) / 4;
            if (dimension < 0) dimension = 0;
            TileSize = new Size(dimension, dimension);
            foreach (UIElement tile in Children)
            {
                Tile t = tile as Tile;
                int tileX = GetX(t);
                int tileY = GetY(t);
                if (tileX < 0 || tileX >= BoardSize || tileY < 0 || tileY >= BoardSize)
                    throw new InvalidTileCoordinatesException(tileX, tileY);
                
                tile.Arrange(GetTileRect(ObjectModel.Factory.NewPosition(tileX, tileY)));
            }
            return finalSize; // Returns the final Arranged size
        }

        Rect GetTileRect(ObjectModel.IPosition pos)
        {
            Point p = new Point(pos.X * TileSize.Width, pos.Y * TileSize.Height);
            p.X += (pos.X + 1) * TileMargin;
            p.Y += (pos.Y + 1) * TileMargin;
            if (p.X < 0) p.X = 0;
            if (p.Y < 0) p.Y = 0;
            return new Rect(p, TileSize);
        }

        double GridSquareSize
        {
            get
            {
                return this.ActualWidth / BoardSize;
            }
        }

        System.Windows.Media.Pen linePen = null;
        protected override void OnRender(System.Windows.Media.DrawingContext dc)
        {
            base.OnRender(dc);
            if (GridLineThickness > 0)
            {
                if (linePen == null)
                    linePen = new Pen(GridLineColor, GridLineThickness);
                if (TileMargin > 0)
                {
                    for (int i = 1; i < BoardSize; i++)
                    {
                        double offset = (TileSize.Width * i) + (TileMargin * (i + 1)) - (TileMargin / 2);
                        dc.DrawLine(linePen, new Point(TileMargin, offset), new Point(this.ActualWidth - TileMargin, offset));
                        dc.DrawLine(linePen, new Point(offset, TileMargin), new Point(offset, this.ActualHeight - TileMargin));
                    }
                }
            }
        }

        Point DiffPoints(Point orig, Point cur)
        {
            return new Point(cur.X - orig.X, cur.Y - orig.Y);
        }

        ObjectModel.IPosition PointToPosition(Point pos)
        {
            int tilew = (int)this.ActualWidth / BoardSize;
            int tileh = (int)this.ActualHeight / BoardSize;
            if (tilew == 0 || tileh == 0)
                return ObjectModel.Factory.NewPosition(0, 0);
            return ObjectModel.Factory.NewPosition((int)pos.X / tilew, (int)pos.Y / tileh);
        }

        bool IsValidPosition(ObjectModel.IPosition pos)
        {
            if (Targets.Count == 0 )
                return false;
            foreach (ObjectModel.IPosition p in Targets)
            {
                if (p.Equals(pos)) return true;
            }
            return false;
        }

        bool PointsAreClose(Point start, Point current)
        {
            double directionThreshold = TileMargin;
            double diffx = current.X - start.X;
            double diffy = current.Y - start.Y;
            return Math.Abs(diffx) < directionThreshold && Math.Abs(diffy) < directionThreshold;
        }

        ObjectModel.Direction CalculateDragDirection(Point origin, Point dragTo)
        {
            double diffx = dragTo.X - origin.X;
            double diffy = dragTo.Y - origin.Y;
            diffy *= -1;
            double degrees = Math.Round(Math.Atan2(diffy, diffx) * (180 / Math.PI));
            if (degrees < 0)
                degrees = 360 + degrees;
            if (degrees >= 315 || degrees < 45)
                return ObjectModel.Direction.Right;
            else if (degrees >= 45 && degrees < 135)
                return ObjectModel.Direction.Up;
            else if (degrees >= 135 && degrees < 225)
                return ObjectModel.Direction.Left;
            else return ObjectModel.Direction.Down;
        }



        bool IsValidDirection(ObjectModel.IPosition p, ObjectModel.Direction dir)
        {
            if (_dragConstraints == null)
                return false;
            switch (dir)
            {
                case ObjectModel.Direction.Up:
                    return _dragConstraints.MinY < 0;
                case ObjectModel.Direction.Down:
                    return _dragConstraints.MaxY > 0;
                case ObjectModel.Direction.Left:
                    return _dragConstraints.MinX < 0;
                case ObjectModel.Direction.Right:
                    return _dragConstraints.MaxX > 0;
            }
            return false;
        }

        List<Rect> CalculateTargetRectangles()
        {
            List<Rect> rects = new List<Rect>();
            if (Targets.Count == 0)
                return rects;
            foreach( ObjectModel.IPosition p in Targets )
            {
                rects.Add(GetTileRect(p));
            }
            return rects;
        }

        void CleanUpTargetOverlay()
        {
            if (_targetOverlay != null)
            {
                AdornerLayer.GetAdornerLayer(_targetOverlay.AdornedElement).Remove(_targetOverlay);
                _targetOverlay = null;
            }
        }

        public List<Tile> FindTilesByTeam(ObjectModel.PlayerTeam team)
        {
            List<Tile> tiles = new List<Tile>();
            foreach (Tile t in Children)
            {
                if (t.Team == team)
                    tiles.Add(t);
            }
            return tiles;
        }
    }

    public class InvalidTileCoordinatesException : System.Exception
    {
        public InvalidTileCoordinatesException(int x, int y)
            : base(string.Format("Invalid tile coordinates: {0},{1}", x, y))
        {
        }
    }

    public class DragConstraints
    {
        public DragConstraints(ObjectModel.IPosition tile, ObservableCollection<ObjectModel.IPosition> targets, double tilesize)
        {
            // Calculate the constraints based on the valid target positions
            int tilesAbove = 0, tilesBelow = 0, tilesLeft = 0, tilesRight = 0;
            if (targets.Count > 0)
            {
                tilesAbove = targets.Max(p => { if (p.Y < tile.Y) return tile.Y - p.Y; return 0; });
                tilesBelow = targets.Max(p => { if (p.Y > tile.Y) return p.Y - tile.Y; return 0; });
                tilesLeft = targets.Max(p => { if (p.X < tile.X) return tile.X - p.X; return 0; });
                tilesRight = targets.Max(p => { if (p.X > tile.X) return p.X - tile.X; return 0; });
            }

            MinX = tilesize * tilesLeft * -1;
            MaxX = tilesize * tilesRight;
            MinY = tilesize * tilesAbove * -1;
            MaxY = tilesize * tilesBelow;
        }

        public double MinX { get; private set; }
        public double MaxX { get; private set; }
        public double MinY { get; private set; }
        public double MaxY { get; private set; }

    }

    public class TargetAdorner : Adorner
    {
        List<Rect> _targets;
        Pen _renderPen; 

        // Be sure to call the base class constructor.
        public TargetAdorner(UIElement adornedElement, List<Rect> targets )
            : base(adornedElement)
        {
            _targets = targets;

            NameScope.SetNameScope(this, new NameScope());
            LinearGradientBrush renderBrush = new LinearGradientBrush();

            // Create gradient stops for the brush.
            GradientStop stop1 = new GradientStop(Colors.Green, .8);
            GradientStop stop2 = new GradientStop(Colors.White, 1.2);

            // Register a name for each gradient stop with the
            // page so that they can be animated by a storyboard.
            this.RegisterName("GradientStop1", stop1);
            this.RegisterName("GradientStop2", stop2);
            this.RegisterName("RenderBrush", renderBrush);

            // Add the stops to the brush.
            renderBrush.GradientStops.Add(stop1);
            renderBrush.GradientStops.Add(stop2);

            //
            // Animate the first gradient stop's offset from
            // 0.0 to 1.0 and then back to 0.0.
            //
            DoubleAnimation offsetAnimation = new DoubleAnimation();
            offsetAnimation.From = .9;
            offsetAnimation.To = -.2;
            offsetAnimation.Duration = TimeSpan.FromSeconds(1);
            offsetAnimation.AutoReverse = true;
            offsetAnimation.RepeatBehavior = RepeatBehavior.Forever;
            Storyboard.SetTargetName(offsetAnimation, "GradientStop1");
            Storyboard.SetTargetProperty(offsetAnimation,
                new PropertyPath(GradientStop.OffsetProperty));

            //
            // Animate the first gradient stop's offset from
            // 0.0 to 1.0 and then back to 0.0.
            //
            DoubleAnimation offsetAnimation2 = new DoubleAnimation();
            offsetAnimation2.From = 1.2;
            offsetAnimation2.To = .3;
            offsetAnimation2.Duration = TimeSpan.FromSeconds(1);
            offsetAnimation2.AutoReverse = true;
            offsetAnimation2.RepeatBehavior = RepeatBehavior.Forever;
            Storyboard.SetTargetName(offsetAnimation2, "GradientStop2");
            Storyboard.SetTargetProperty(offsetAnimation2,
                new PropertyPath(GradientStop.OffsetProperty));

            DoubleAnimation opacityAnimation = new DoubleAnimation(1, 0.3, new Duration(TimeSpan.FromSeconds(1.5)));
            opacityAnimation.AutoReverse = true;
            opacityAnimation.RepeatBehavior = System.Windows.Media.Animation.RepeatBehavior.Forever;
            Storyboard.SetTargetName(opacityAnimation, "RenderBrush");
            Storyboard.SetTargetProperty(opacityAnimation, new PropertyPath(LinearGradientBrush.OpacityProperty));

            // Create a Storyboard to apply the animations.
            Storyboard gradientStopAnimationStoryboard = new Storyboard();
            gradientStopAnimationStoryboard.Children.Add(offsetAnimation);
            gradientStopAnimationStoryboard.Children.Add(offsetAnimation2);
            gradientStopAnimationStoryboard.Children.Add(opacityAnimation);

            gradientStopAnimationStoryboard.Begin(this);
            _renderPen = new Pen(renderBrush, 3);
        }

        
        protected override void OnRender(DrawingContext drawingContext)
        {
            // Targets get a green square.
            foreach (Rect targetRect in _targets)
            {
                drawingContext.DrawRectangle(null, _renderPen, targetRect);
            }
            
        }

        
    }
}
