using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Collections.ObjectModel;

namespace WPF
{
    public class GameViewModel : DependencyObject
    {
        public const int SIZE = 4;
        Model m_model;
        ObjectModel.IGameDescriptor m_game = null;

        public event EventHandler<TilesUpdatedEventArgs> TilesUpdated;

        public GameViewModel()
        { 
        }

        public void Initialize(Model model, ObjectModel.IGameDescriptor game)
        {
            m_model = model;
            m_game = game;
            Player1 = new PlayerViewModel(game.GetHomePlayer());
            Player2 = new PlayerViewModel(game.GetAwayPlayer());
            SetCurrentPlayer(game.GetTurn() == ObjectModel.PlayerTeam.Home ? Player1 : Player2);
            BoardEnabled = true;
            model.GameStateChanged += new EventHandler<GameStateChangedArgs>(model_GameStateChanged);
            SetBoardState();
        }

        public void OnTileMoved(object sender, TileMovedEventArgs e)
        {
            foreach (ObjectModel.ITile t in m_game.GetTiles())
            {
                if (t.Position.X == e.From.X &&
                    t.Position.Y == e.From.Y)
                {
                    m_model.LocalTurnCompleted(t, e.To);
                    return;
                }
            }
            System.Diagnostics.Debug.Assert(false);
        }

        void model_GameStateChanged(object sender, GameStateChangedArgs e)
        {
            SetBoardState();
        }

        public void OnTileSelected(object sender, TileSelectedEventArgs e)
        {
            Targets.Clear();
            foreach (ObjectModel.ITile t in m_game.GetTiles())
            {
                if( t.Position.X == e.Position.X &&
                    t.Position.Y == e.Position.Y )
                {
                    foreach (ObjectModel.IPosition p in t.GetTargets())
                    {
                        Targets.Add( p );
                    }
                }
            }
        }

        public void OnGameOver(object sender, GameOverEventArgs e)
        {
            BoardEnabled = true;
        }

        void SetBoardState()
        {
            m_awayBrush = null;
            m_homeBrush = null;
            Player1 = new PlayerViewModel(m_game.GetHomePlayer());
            Player2 = new PlayerViewModel(m_game.GetAwayPlayer());
            IEnumerable<ObjectModel.ITile> tiles = m_game.GetTiles();
            ObjectModel.PlayerTeam turn = m_game.GetTurn();
            SetCurrentPlayer(turn == ObjectModel.PlayerTeam.Home ? Player1 : Player2);
            Player1.UpdateTetrads();
            Player2.UpdateTetrads();
            if (tiles == null)
            {
                throw new System.Exception("INTERNAL ERROR: No game tiles available!");
            }
            var tileList = new List<Tile>();
            foreach (ObjectModel.ITile engine in tiles)
            {
                Tile t = new Tile();
                t.Face.Expression = Expression.NORMAL;
                DupeTile(engine, t);
                tileList.Add(t);
            }
            if (TilesUpdated != null)
                TilesUpdated(this, new TilesUpdatedEventArgs(tileList));
            BoardEnabled = CurrentPlayer.EnginePlayer.Type == ObjectModel.PlayerType.Local;
        }

        void DupeTile(ObjectModel.ITile engine, Tile ui)
        {
            Board.SetId(ui, engine.Id);
            Board.SetX(ui, engine.Position.X);
            Board.SetY(ui, engine.Position.Y);
            ui.Team = engine.Team;
            ui.Background = engine.Team == ObjectModel.PlayerTeam.Home ? HomeTeamColor : AwayTeamColor;
            ui.RenderTransform = null;
        }

        public ObservableCollection<ObjectModel.IPosition> Targets
        {
            get { return (ObservableCollection<ObjectModel.IPosition>)GetValue(TargetsProperty); }
            set { SetValue(TargetsProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Targets.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TargetsProperty =
            DependencyProperty.Register("Targets", typeof(ObservableCollection<ObjectModel.IPosition>), typeof(GameViewModel), new UIPropertyMetadata(new ObservableCollection<ObjectModel.IPosition>()));


        public PlayerViewModel Player1
        {
            get { return (PlayerViewModel)GetValue(Player1Property); }
            set { SetValue(Player1Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player1.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player1Property =
            DependencyProperty.Register("Player1", typeof(PlayerViewModel), typeof(GameViewModel));

        public PlayerViewModel Player2
        {
            get { return (PlayerViewModel)GetValue(Player2Property); }
            set { SetValue(Player2Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player2.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player2Property =
            DependencyProperty.Register("Player2", typeof(PlayerViewModel), typeof(GameViewModel));

        public PlayerViewModel CurrentPlayer
        {
            get { return (PlayerViewModel)GetValue(CurrentPlayerProperty); }
            set { SetValue(CurrentPlayerProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentPlayer.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentPlayerProperty =
            DependencyProperty.Register("CurrentPlayer", typeof(PlayerViewModel), typeof(GameViewModel));

        SolidColorBrush m_homeBrush = null;
        public Brush HomeTeamColor
        {
            get
            {
                if (m_homeBrush != null) return m_homeBrush;
                m_homeBrush = new SolidColorBrush(
                    ColorFromUint( Player1.EnginePlayer.RgbColor ) );
                return m_homeBrush;
            }
        }

        SolidColorBrush m_awayBrush = null;
        public Brush AwayTeamColor
        {
            get
            {
                if (m_awayBrush != null) return m_awayBrush;
                m_awayBrush = new SolidColorBrush(
                    ColorFromUint(Player2.EnginePlayer.RgbColor));
                return m_awayBrush;
            }
        }

        Color ColorFromUint(uint color)
        {
            byte a = (byte)(color >> 24);
            byte r = (byte)((color & 0x00FF0000) >> 16);
            byte g = (byte)((color & 0x0000FF00) >> 8);
            byte b = (byte)(color & 0x000000FF);
            return Color.FromArgb(a, r, g, b);
        }

        public string StatusText
        {
            get { return (string)GetValue(StatusTextProperty); }
            set { SetValue(StatusTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for StatusText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty StatusTextProperty =
            DependencyProperty.Register("StatusText", typeof(string), typeof(GameViewModel), new PropertyMetadata(""));

        public bool BoardEnabled
        {
            get { return (bool)GetValue(BoardEnabledProperty); }
            set { SetValue(BoardEnabledProperty, value); }
        }

        // Using a DependencyProperty as the backing store for BoardEnabled.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BoardEnabledProperty =
            DependencyProperty.Register("BoardEnabled", typeof(bool), typeof(GameViewModel));

        public void SetCurrentPlayer(PlayerViewModel player)
        {
            CurrentPlayer = player.EnginePlayer.Team == ObjectModel.PlayerTeam.Home ? Player1 : Player2;
            
            if (CurrentPlayer == Player1)
            {
                if (Player1.EnginePlayer.Type == ObjectModel.PlayerType.Local &&
                    CurrentPlayer.Name == "You")
                    StatusText = "Your Turn";
                else
                    StatusText = CurrentPlayer.TurnName + " Turn";
            }
            else
            {
                StatusText = CurrentPlayer.TurnName + " Turn";
            }
            if (Player1.EnginePlayer.Type == ObjectModel.PlayerType.Local &&
                Player2.EnginePlayer.Type == ObjectModel.PlayerType.Local)
            {
                // Pass & Play mode
                BoardEnabled = true;
            }
        }

        //public void SetTargets(Position pos)
        //{
        //    List<Position> targets = new List<Position>();
        //    TsCOMLib.Tile t = TileForPosition(pos);
        //    if (t != null)
        //    {
        //        foreach (TsCOMLib.Position targetPosition in t.GetTargets())
        //        {
        //            targets.Add(new Position(targetPosition));
        //        }
        //    }
        //}

        public ObjectModel.ITile TileForPosition(ObjectModel.IPosition tpos)
        {
            var tiles = m_game.GetTiles();
            if (tiles == null)
            {
                throw new System.Exception("INTERNAL ERROR: No game tiles available!");
            }
            foreach (ObjectModel.ITile t in tiles)
            {
                if( t.Position.Equals(tpos ) )
                    return t;
            }
            return null;
        }

        
    }

    class TileInfo
    {
        public TileInfo() { }
        public ObjectModel.IPosition Position { get; set; }
        public TsCOMLib.PlayerTeam Team { get; set; }
    }
}
