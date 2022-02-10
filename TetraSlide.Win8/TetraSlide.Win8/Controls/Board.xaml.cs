using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Core;
using Windows.UI.Input;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Shapes;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace TetraSlide.Win8.Controls
{
    
    public sealed partial class Board : Grid
    {
        private const int MAX_TILES = 12;
        private List<Tile> TileControls = new List<Tile>(MAX_TILES);
        private List<TsWinRT.TsTarget> m_targets = new List<TsWinRT.TsTarget>();
        Tile SelectedTile = null;
        Point? dragStart = null;
        DragConstraints dragConstraints = null;
        List<Tile> draggingTiles = new List<Tile>();
        Page m_page = null;

        public event EventHandler<TileMovedEventArgs> TileMoved;
        public event EventHandler<EventArgs> AnimationComplete;


        public Board()
        {
            this.InitializeComponent();
            this.DataContext = this;
            this.Loaded += Board_Loaded;
            Tiles = new ObservableCollection<ViewModel.Tile>();
            GridLinesBrush = new SolidColorBrush(Color.FromArgb(0x33, 0xFF, 0xFF, 0xFF));            
        }

        void Board_Loaded(object sender, RoutedEventArgs e)
        {
        	DependencyObject obj = this;
            while (true)
            {
                obj = VisualTreeHelper.GetParent(obj);
                if (obj is Page)
                {
                    m_page = obj as Page;
                    break;
                }
                // Hang if we aren't the child of a Page, cuz wtf?
            }
            InitTileControls();
            UpdateTiles();
            this.PointerMoved += OnPointerMoved;
            this.PointerPressed += OnPointerPressed;
            this.PointerReleased += OnPointerReleased;
        }

        object locker = new object();
        bool m_animating = false;
        bool IsAnimating
        {
            get {
                lock (locker)
                {
                    return m_animating;
                }
            }
            set {
                lock (locker)
                {
                    m_animating = value;
                }
            }
        }

        public ViewModel.Player HomePlayer
        {
            get;
            private set;
        }

        public ViewModel.Player AwayPlayer
        {
            get;
            private set;
        }

        private TsWinRT.TsGameDescriptor m_desc;
        public TsWinRT.TsGameDescriptor GameDescriptor
        {
            get { return m_desc; }
            set
            {
                m_desc = value;
                Tiles = new ObservableCollection<ViewModel.Tile>();
                HomePlayer = new ViewModel.Player(m_desc.HomePlayer);
                AwayPlayer = new ViewModel.Player(m_desc.AwayPlayer);
                ClearDraggingTiles();
                foreach (TsWinRT.TsTile tile in m_desc.Tiles)
                {
                    ViewModel.Player owner = tile.Owner == TsWinRT.TsPlayerTeam.TsTeamAway ? AwayPlayer : HomePlayer;
                    ViewModel.Tile t = new ViewModel.Tile(tile, owner);
                    Tiles.Add(t);
                }
                CurrentTurn = m_desc.Turn;
                InteractionEnabled = CurrentTurn == TsWinRT.TsPlayerTeam.TsTeamHome;
                InitTileControls();
                UpdateTiles();
            }
        }

        private void ClearDraggingTiles()
        {
            foreach (Tile t in draggingTiles)
            {
                t.RenderTransform = null;
            }
            draggingTiles.Clear();
        }

        public TsWinRT.TsPlayerTeam CurrentTurn
        {
            get { return (TsWinRT.TsPlayerTeam)GetValue(CurrentTurnProperty); }
            set { SetValue(CurrentTurnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentTurn.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentTurnProperty =
            DependencyProperty.Register("CurrentTurn", typeof(TsWinRT.TsPlayerTeam), typeof(Board), new PropertyMetadata(TsWinRT.TsPlayerTeam.TsTeamHome));

        public Boolean InteractionEnabled
        {
            get { return (Boolean)GetValue(InteractionEnabledProperty); }
            set { SetValue(InteractionEnabledProperty, value); }
        }

        // Using a DependencyProperty as the backing store for InteractionEnabled.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty InteractionEnabledProperty =
            DependencyProperty.Register("InteractionEnabled", typeof(Boolean), typeof(Board), null);


        public ObservableCollection<ViewModel.Tile> Tiles
        {
            get { return (ObservableCollection<ViewModel.Tile>)GetValue(TilesProperty); }
            set { SetValue(TilesProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Tiles.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TilesProperty =
            DependencyProperty.Register("Tiles", typeof(ObservableCollection<ViewModel.Tile>), typeof(Board), null);


        public Brush GridLinesBrush
        {
            get { return (Brush)GetValue(GridLinesBrushProperty); }
            set { SetValue(GridLinesBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for GridLinesBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GridLinesBrushProperty =
            DependencyProperty.Register("GridLinesBrush", typeof(Brush), typeof(Board), null);

        BoardSpace HitTest(Point pt)
        {
            var hits = VisualTreeHelper.FindElementsInHostCoordinates(pt, m_page);
            //var hits2 = VisualTreeHelper.FindElementsInHostCoordinates(pt, this);
            foreach (UIElement ele in hits)
            {
                if (ele is BoardSpace)
                    return ele as BoardSpace;
            }
            return null;
        }

        TsWinRT.TsPosition PositionFromBoardSpace(BoardSpace bs)
        {
            if (bs == null) return null;
            Border border = VisualTreeHelper.GetParent(bs) as Border;
            int y = Grid.GetRow(border);
            int x = Grid.GetColumn(border);
            var pos = new TsWinRT.TsPosition(x, y);
            return IsValidTarget(pos) ? pos : null;
        }

        // cursorPosition = e.GetCurrentPoint(null).Position
        TsWinRT.TsPosition GetIntendedTarget(Point cursorPosition, TranslateTransform dragTransform)
        {
            BoardSpace bs = HitTest(cursorPosition);
            var pos = PositionFromBoardSpace(bs);
            if (pos == null)
            {
                if (dragStart.HasValue && dragTransform != null)
                {
                    Point pt = new Point(dragStart.Value.X, dragStart.Value.Y);
                    pt = dragTransform.TransformPoint(pt);
                    GeneralTransform gt = this.TransformToVisual(m_page);
                    pt = gt.TransformPoint(pt);
                    bs = HitTest(pt);
                    pos = PositionFromBoardSpace(bs);
                }
            }
            return pos;
        }

        void AnimateToPoint( TranslateTransform tt, Point p, Action callback )
        {
            double dx = Math.Abs(p.X - tt.X);
            double dy = Math.Abs(p.Y - tt.Y);
            if (dx < 20 && dy < 20)
            {
                callback();
                return;
            }

            IsAnimating = true;
            Storyboard sb = new Storyboard();
            Duration animationDuration = new Duration(new TimeSpan(0, 0, 0, 0, 300));
            var ease = new BounceEase()
                    {
                        EasingMode = EasingMode.EaseOut,
                        Bounces = 1,
                        Bounciness = 1
                    };
            Timeline timelineX = new DoubleAnimation()
                {
                    To = p.X,
                    From = tt.X,
                    Duration = animationDuration,
                    AutoReverse = false,
                    //EasingFunction = ease
                };
            Timeline timelineY = new DoubleAnimation()
                {
                    To = p.Y,
                    From = tt.Y,
                    Duration = animationDuration,
                    AutoReverse = false,
                    //EasingFunction = ease
                };
            sb.Children.Add(timelineX);
            sb.Children.Add(timelineY);
            
            Storyboard.SetTarget(timelineX, tt);
            Storyboard.SetTargetProperty(timelineX, "X");
            Storyboard.SetTarget(timelineY, tt);
            Storyboard.SetTargetProperty(timelineY, "Y");
            
            sb.Completed += (s, a) =>
            {
                this.Resources.Remove("reset_animation");
                callback();
                IsAnimating = false;
            };
            
            this.Resources.Add("reset_animation", sb);
            sb.Begin();
        }

        TsWinRT.TsPosition DiffPositions(TsWinRT.TsPosition curpos, TsWinRT.TsPosition newpos)
        {
            return new TsWinRT.TsPosition(newpos.X - curpos.X, newpos.Y - curpos.Y);
        }

        Point CalculateOffsetPoint(Point original, TsWinRT.TsPosition curpos, TsWinRT.TsPosition newpos)
        {
            double tileSize = GetTileSize();
            var positionDiff = DiffPositions(curpos, newpos);
            double newx = original.X + (tileSize * positionDiff.X);
            double newy = original.Y + (tileSize * positionDiff.Y);
            return new Point(newx - original.X, newy - original.Y);
        }

        bool IsDragging
        {
            get
            {
                return dragStart != null && dragConstraints != null && SelectedTile != null;
            }
        }

        void OnPointerReleased(object sender, PointerRoutedEventArgs e)
        {
            // Use this action in order to capture e.Pointer
            Action CleanupDrag = new Action( delegate(){
                if (SelectedTile != null)
                {
                    SelectedTile.ReleasePointerCapture(e.Pointer);
                }
                EndDrag();
                ClearTargets();
            });
            if( IsDragging )
            {
                DoDragging(e.GetCurrentPoint(this).Position);
            }
            else
            {
                CleanupDrag();
                return;
            }
            
            var tt = SelectedTile.RenderTransform as TranslateTransform;
            var newPos = GetIntendedTarget(e.GetCurrentPoint(null).Position, tt);
            if (newPos == null)
            {
                // Animate the render transform back down to 0
                var tr = SelectedTile.RenderTransform as TranslateTransform;
                if (tr != null)
                {
                    AnimateToPoint(tr, new Point(0, 0), delegate() {
                        CleanupDrag();
                        ClearDraggingTiles();
                    });
                }
            }
            else
            {
                // To smoothly animate into the final position:
                // Figure out a new "target point" where we need to animate to.  This is
                //    dragStart.X or dragStart.Y + target distance * width of a square. Animate
                //    the transform to the new target point.
                ViewModel.Tile tileVm = SelectedTile.DataContext as ViewModel.Tile;
                if (tt != null)
                {
                    Point moveTo = CalculateOffsetPoint(dragStart.Value, tileVm.EngineTile.Pos, newPos);
                    AnimateToPoint(tt, moveTo, delegate()
                    {
                        FireTileMoved(tileVm, newPos);
                        CleanupDrag();
                    });
                    return;
                }
            }
        }

        void OnPointerPressed(object sender, PointerRoutedEventArgs e)
        {
            Tile t = GetTileSource(e.OriginalSource);
            if (t != null)
            {
                ViewModel.Tile tileVm = t.DataContext as ViewModel.Tile;
                if (tileVm.Owner.Team != CurrentTurn)
                    return; // Can't mess with other player's tiles.
                UpdateTargets(tileVm);
                SelectedTile = t;
                dragStart = e.GetCurrentPoint(this).Position;
                SelectedTile.CapturePointer(e.Pointer);
            } 
        }
        
        void OnPointerMoved(object sender, PointerRoutedEventArgs e)
        {
            if (IsAnimating) return;
            if (e.Pointer.IsInContact)
            {
                if (SelectedTile != null && dragStart.HasValue)
                {
                    // Update the tile's render transform...
                    Point dragTo = e.GetCurrentPoint(this).Position;
                    if (dragConstraints == null )
                    {
                        BeginDrag(dragTo);
                    }
                    else
                    {
                        // Check for direction changes here...
                        var tt = SelectedTile.RenderTransform as TranslateTransform;
                        if (tt != null && TestDirectionChange(dragTo))
                        {
                            if (tt != null)
                            {
                                Point ds = dragStart.Value;
                                Tile st = SelectedTile;
                                EndDrag();
                                SelectedTile = st;
                                dragStart = ds;
                                BeginDrag(dragTo);
                                var tileVm = SelectedTile.DataContext as ViewModel.Tile;
                            }
                        }
                        else
                        {
                            DoDragging(dragTo);
                        }
                    }
                }
            }
        }

        bool TestDirectionChange(Point dragTo)
        {
            double tileSize = GetTileSize();
            // Find the center point of the original tile.

            var tt = SelectedTile.RenderTransform as TranslateTransform;
            double offx = Math.Abs(tt.X);// - centerx);
            double offy = Math.Abs(tt.Y);// - centery);
            if (offx < tileSize / 2 && offy < tileSize / 2)
            {
                // You are within the original space?
                // Determine new drag constraints
                if (dragConstraints.CalculateDragDirection(dragStart.Value, dragTo) != dragConstraints.CurrentDirection)
                {
                    // Direction change.
                    return true;
                }
            }
            return false;
        }

        double GetTileSize()
        {
            return GetBoardSpace(new TsWinRT.TsPosition(0,0)).ActualWidth;
        }

        void BeginDrag(Point dragTo)
        {
            dragConstraints = new DragConstraints(this, dragStart.Value, SelectedTile, 
                GetTilePosition(SelectedTile), BuildTilePositionMap() );
            dragConstraints.SetDragAxis(dragTo);
            
            CalculateDraggingTiles(dragStart.Value, dragTo);

            TranslateTransform tt = new TranslateTransform();
            foreach (Tile t in draggingTiles)
                t.RenderTransform = tt;
            SelectedTile.IsHitTestVisible = false;
            DoDragging(dragTo);
        }

        void DoDragging(Point dragTo)
        {
            TranslateTransform tt = SelectedTile.RenderTransform as TranslateTransform;
            if (tt == null) return;
            dragConstraints.DoDrag(dragTo, tt);
        }

        void EndDrag()
        {
           
            if (SelectedTile != null)
            {
                SelectedTile.IsHitTestVisible = true;
                SelectedTile = null;
            }
            dragStart = null;
            dragConstraints = null;
        }

        void CalculateDraggingTiles(Point origin, Point dragTo)
        {
            ClearDraggingTiles();
            draggingTiles.Add(SelectedTile);
            TsWinRT.TsBoardDirection direction = dragConstraints.CalculateDragDirection(origin, dragTo);
            ViewModel.Tile tileVm = SelectedTile.DataContext as ViewModel.Tile;
            draggingTiles.AddRange( FindAdjacentTiles( tileVm, direction ) );
        }

        List<Tile> FindAdjacentTiles(ViewModel.Tile source, TsWinRT.TsBoardDirection dir)
        {
            List<Tile> tiles = new List<Tile>();
            if (source == null || !dragConstraints.CanMove(dir)) return tiles;
            int y = source.EngineTile.Pos.Y;
            int x = source.EngineTile.Pos.X;
            Action incrementer = null;
            switch (dir)
            {
                case TsWinRT.TsBoardDirection.TsDirUp:
                    incrementer = new Action(() => --y); break;
                case TsWinRT.TsBoardDirection.TsDirDown:
                    incrementer = new Action(() => ++y); break;
                case TsWinRT.TsBoardDirection.TsDirLeft:
                    incrementer = new Action(() => --x); break;
                case TsWinRT.TsBoardDirection.TsDirRight:
                    incrementer = new Action(() => ++x); break;
                default:
                    return tiles;
            }
            while (true)
            {
                incrementer();
                var t = GetTileElement(new TsWinRT.TsPosition(x, y));
                if (t == null) return tiles;
                tiles.Add(t);
            }
        }

        public void GameOver( TsWinRT.ITsPlayer player)
        {
            InteractionEnabled = false;
        }

        public void AnimateTetrad(TetradFoundEventArgs args, Action postAnimationCallback)
        {
            InteractionEnabled = false;
            // Do animation here
            List< Tile > tilesToAnimate = new List< Tile >();
            ScaleTransform st = new ScaleTransform();
            st.CenterX = GetTileSize() / 2;
            st.CenterY = st.CenterX;
            foreach (TsWinRT.TsTile tile in args.Tiles)
            {
                Tile tileUIElement = GetTileElement(tile.Pos);
                tilesToAnimate.Add( tileUIElement );
                tileUIElement.RenderTransform = st;
            }
            BounceEase ease = new BounceEase()
            {
                Bounces = 2,
                Bounciness = 1,
                EasingMode = EasingMode.EaseInOut
            };
            Storyboard sb = new Storyboard();
            Timeline scaleX = new DoubleAnimation()
            {
                To = .8,
                From = 1,
                Duration = new Duration(new TimeSpan(0, 0, 0, 0, 200)),
                //EasingFunction = ease,
                AutoReverse = true
            };
            Timeline scaleY = new DoubleAnimation()
            {
                To = .8,
                From = 1,
                Duration = new Duration(new TimeSpan(0, 0, 0, 0, 200)),
                //EasingFunction = ease,
                AutoReverse = true
            };

            sb.Children.Add(scaleX);
            sb.Children.Add(scaleY);
            Storyboard.SetTarget(scaleX, st);
            Storyboard.SetTarget(scaleY, st);
            Storyboard.SetTargetProperty(scaleX, "ScaleX");
            Storyboard.SetTargetProperty(scaleY, "ScaleY");
            sb.Completed += (s, a) =>
            {
                foreach( Tile x in tilesToAnimate ) 
                {
                    x.RenderTransform = null;
                }
                InteractionEnabled = true;
                // Run this code synchronously when the animation completes!
                postAnimationCallback();
                if (AnimationComplete != null)
                    AnimationComplete(this, new EventArgs());
            };
            sb.Begin();
            
        }

        void ClearTargets()
        {
            foreach (Border border in Children)
            {
                BoardSpace bs = border.Child as BoardSpace;
                if (bs != null)
                {
                    bs.Invalid = false;
                    bs.Targeted = false;
                }
            }
        }

        Dictionary<TsWinRT.TsPosition, BoardSpace> BuildTilePositionMap()
        {
            var targetmap = new Dictionary<TsWinRT.TsPosition, BoardSpace>();
            foreach (var target in m_targets)
            {
                if( target.Valid )
                    targetmap.Add(target.Pos, GetBoardSpace(target.Pos));
            }
            return targetmap;
        }

        void UpdateTargets(ViewModel.Tile tileVm)
        {
            m_targets.Clear();         
            foreach (var target in tileVm.GetTargets())
            {
                var boardSpace = GetBoardSpace(target.Pos);
                if (target.Valid)
                {
                    m_targets.Add(target);
                    if (boardSpace != null)
                        boardSpace.Targeted = true;
                }
                else
                {
                    if( boardSpace != null )
                        boardSpace.Invalid = true;
                }
            }
        }

        

        void InitTileControls()
        {
            TileControls = new List<Tile>(MAX_TILES);
            foreach (ViewModel.Tile t in Tiles)
            {
                Tile control = new Tile()
                {
                    DataContext = t,
                    Name = "Tile_" + t.EngineTile.Id.ToString()
                };
                
                TileControls.Add(control);
               
            }
        }

        void UpdateTiles()
        {
            foreach( Border border in Children )
            {
                (border.Child as BoardSpace).SetTile(null);
            }
            foreach (Tile t in TileControls)
            {
                ViewModel.Tile tileVM = t.DataContext as ViewModel.Tile;
                if (tileVM != null)
                {
                    Border newPosition = GetBorderElement(tileVM.GetBoardPosition());
                    (newPosition.Child as BoardSpace).SetTile(t);
                }
            }
        }

        BoardSpace GetBoardSpace(DependencyObject obj)
        {
            if (obj == null) return null;
            if (obj is BoardSpace) return obj as BoardSpace;
            if (obj is FrameworkElement)
                return GetBoardSpace(VisualTreeHelper.GetParent(obj));
            return null;
        }

        internal BoardSpace GetBoardSpace(TsWinRT.TsPosition pos)
        {
            var border = GetBorderElement(pos);
            if (border == null) return null;
            return border.Child as BoardSpace;
        }

        Border GetBorderElement(TsWinRT.TsPosition bp)
        {
            return FindGridElement<Border>(Children, bp);
        }

        Tile GetTileElement(TsWinRT.TsPosition pos)
        {
            var boardSpace = GetBoardSpace(pos);
            if (boardSpace == null) return null;
            var bsGrid = boardSpace.Content as Grid;
            foreach (Border child in bsGrid.Children)
                if (child.Name == "TileBorder")
                    return child.Child as Tile;
            return null;
        }

        TsWinRT.TsPosition GetTilePosition(Tile t)
        {
            var bs = GetBoardSpace(t);
            if (bs != null)
            {
                var border = bs.Parent as Border;
                int itemX = Grid.GetColumn(border);
                int itemY = Grid.GetRow(border);
                return new TsWinRT.TsPosition(itemX, itemY);
            }
            throw new Exception("Tile is not contained by the board!");
        }

        ElementType FindGridElement<ElementType>(IEnumerable lookIn, TsWinRT.TsPosition bp) where ElementType : FrameworkElement
        {
            foreach (FrameworkElement o in lookIn)
            {
                if (o is ElementType)
                {
                    int itemX = Grid.GetColumn(o);
                    int itemY = Grid.GetRow(o);
                    if (bp.X == itemX && bp.Y == itemY)
                    {
                        return o as ElementType;
                    }
                }
            }
            return null;
        }

        Tile GetTileSource(object obj)
        {
            if (obj == null) return null;
            DependencyObject ele = obj as DependencyObject;
            while (ele != null)
            {
                if (ele is Tile) return ele as Tile;
                ele = VisualTreeHelper.GetParent(ele);
            }
            return null;
        }

        void FireTileMoved(ViewModel.Tile source, TsWinRT.TsPosition pos)
        {
            if (TileMoved != null)
                TileMoved(this, new TileMovedEventArgs(source, pos));
            InteractionEnabled = false;
        }

        // This makes the board square.
        protected override Size ArrangeOverride(Size finalSize)
        {
            if (finalSize.Height > finalSize.Width)
                base.ArrangeOverride(new Size(finalSize.Width, finalSize.Width));
            return base.ArrangeOverride(new Size(finalSize.Height, finalSize.Height));
        }

        private class MoveAnimationProperties
        {
            public MoveAnimationProperties(Board board, Tile t, TsWinRT.TsPosition tilePos, TsWinRT.TsBoardDirection dir, int distance)
            {
                TsWinRT.TsPosition newPos = tilePos;
                switch (dir)
                {
                    case TsWinRT.TsBoardDirection.TsDirDown:
                        newPos.Y += distance;
                        AnimationProperty = "Y";
                        break;
                    case TsWinRT.TsBoardDirection.TsDirUp:
                        newPos.Y -= distance;
                        AnimationProperty = "Y";
                        break;
                    case TsWinRT.TsBoardDirection.TsDirLeft:
                        newPos.X -= distance;
                        AnimationProperty = "X";
                        break;
                    case TsWinRT.TsBoardDirection.TsDirRight:
                        newPos.X += distance;
                        AnimationProperty = "X";
                        break;
                }
                var bs = board.GetBoardSpace(newPos);
                var tileTopLeft = t.TransformToVisual(board).TransformPoint(new Point(0, 0));
                var targetTopLeft = bs.TransformToVisual(board).TransformPoint(new Point(0, 0));
                AnimationValue = targetTopLeft.X - tileTopLeft.X;
                if (AnimationProperty == "Y")
                    AnimationValue = targetTopLeft.Y - tileTopLeft.Y;
            }

            public string AnimationProperty
            {
                get;
                private set;
            }

            double AnimationValue
            {
                get;
                set;
            }

            public Timeline CreateAnimation()
            {
                DoubleAnimation da = new DoubleAnimation();
                da.To = AnimationValue;
                da.Duration = new Duration(new TimeSpan(0, 0, 0, 0, 500));
                return da;
            }
        };

        public void AnimateMove(TsWinRT.TsGameDescriptor gameDescriptor, AnimateTurnEventArgs e)
        {
            Storyboard sb = new Storyboard();
            foreach (TsWinRT.TsTile tile in e.Tiles)
            {
                var t = GetTileElement(tile.Pos);
                var props = new MoveAnimationProperties(this, t, tile.Pos, e.Direction, e.Distance); 
                t.RenderTransform = new TranslateTransform();
                Timeline timeline = props.CreateAnimation();
                sb.Children.Add(timeline);
                timeline.AutoReverse = false;
                
                Storyboard.SetTarget(timeline, t.RenderTransform);
                Storyboard.SetTargetProperty(timeline, props.AnimationProperty);
            }
            sb.Completed += (s, a) =>
            {
                this.Resources.Remove("opponent_move_animation");
                //this.GameDescriptor = gameDescriptor;
                if (this.AnimationComplete != null)
                    this.AnimationComplete(this, new EventArgs());
            };
            this.Resources.Add("opponent_move_animation", sb);
            sb.Begin();
        }

        public void StateChanged(TsWinRT.TsGameDescriptor gameDescriptor)
        {
            this.GameDescriptor = gameDescriptor;
        }

        bool PointsAreClose(Point start, Point current)
        {
            double directionThreshold = 30;
            double diffx = current.X - start.X;
            double diffy = current.Y - start.Y;
            return Math.Abs(diffx) < directionThreshold && Math.Abs(diffy) < directionThreshold;
        }

        bool IsValidTarget(TsWinRT.TsPosition pos)
        {
            if (m_targets == null) return false;
            foreach (var target in m_targets)
            {
                if (target.Valid && 
                    target.Pos.X == pos.X &&
                    target.Pos.Y == pos.Y)
                    return true;
            }
            return false;
        }

        List<Tile> FindAdjacentTiles(TsWinRT.TsPosition source, TsWinRT.TsBoardDirection dir)
        {
            List<Tile> tiles = new List<Tile>();
            if (source == null || dir == TsWinRT.TsBoardDirection.TsDirInvalid) return tiles;
            int y = source.Y;
            int x = source.X;
            Action incrementer = null;
            switch (dir)
            {
                case TsWinRT.TsBoardDirection.TsDirUp:
                    incrementer = new Action(() => --y); break;
                case TsWinRT.TsBoardDirection.TsDirDown:
                    incrementer = new Action(() => ++y); break;
                case TsWinRT.TsBoardDirection.TsDirLeft:
                    incrementer = new Action(() => --x); break;
                case TsWinRT.TsBoardDirection.TsDirRight:
                    incrementer = new Action(() => ++x); break;
                default:
                    return tiles;
            }
            while (true)
            {
                incrementer();
                var t = GetTileElement(new TsWinRT.TsPosition(x, y));
                if (t == null) return tiles;
                tiles.Add(t);
            }
        }
    }
}
