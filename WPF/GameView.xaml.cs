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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace WPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class GameView : UserControl
    {
        bool m_gameOver = false;
        

        public GameView()
        {
            InitializeComponent();
            this.Loaded += new RoutedEventHandler(GameView_Loaded);
            this.Unloaded += new RoutedEventHandler(GameView_Unloaded);
            GameResult.Visibility = Visibility.Collapsed;
            WaitBackButton.DataContext = App.Mine.Navigator;
        }

        void GameView_Unloaded(object sender, RoutedEventArgs e)
        {
            SoundManager.Instance().PlayMenuMusic();
        }

        public void OnSuspendGame(object sender, ViewPoppedEventArgs e)
        {
            if (!m_gameOver)
            {
                try
                {
                    if (e.View is GameView)
                    {
                        App.Mine.Model.SuspendGame();
                    }
                }
                catch (Exception)
                {
                }
            }
        }

        public void OnTilesUpdated(object sender, TilesUpdatedEventArgs e)
        {
            GameBoard.SetTiles(e.Tiles);
        }

        public void OnTetradFound(object sender, TetradFoundEventArgs e)
        {
            //NameScope.SetNameScope(this, new NameScope());
            var sb = new Storyboard();
            var tiles = new List<Tile>();
            var T = new RotateTransform();
            T.CenterX = GameBoard.TileSize.Width / 2;
            T.CenterY = GameBoard.TileSize.Height / 2;
            var transformName = "TileWiggler";
            var animCW = new DoubleAnimation(45, new Duration(TimeSpan.FromSeconds(.25)));
            var animCCW = new DoubleAnimation(-45, new Duration(TimeSpan.FromSeconds(.25)));
            animCCW.BeginTime = TimeSpan.FromSeconds(.3);
            animCW.AutoReverse = true;
            animCCW.AutoReverse = true;
            try
            {
                this.RegisterName(transformName, T);
            }
            catch (System.Exception ex) {
                Logger.Log("** CAUGHT EXCEPTION SLIDING ANIMATION: TransformName=" + transformName + "\r\n\t" + ex.Message);
            }
            Storyboard.SetTargetName(animCW, transformName);
            Storyboard.SetTargetProperty(animCW, new PropertyPath(RotateTransform.AngleProperty));
            Storyboard.SetTargetName(animCCW, transformName);
            Storyboard.SetTargetProperty(animCCW, new PropertyPath(RotateTransform.AngleProperty));
            sb.Children.Add(animCW);
            sb.Children.Add(animCCW);

            ObjectModel.PlayerTeam t = ObjectModel.PlayerTeam.Home;
            foreach (int id in e.TileIds)
            {
                Tile tile = GameBoard.FindTileById(id);
                tile.RenderTransform = T;
                tiles.Add(tile);
                tile.SetExpression(Expression.HAPPY);
                t = tile.Team;
            }
            foreach (Tile tile in GameBoard.FindTilesByTeam(t == ObjectModel.PlayerTeam.Home ? ObjectModel.PlayerTeam.Away : ObjectModel.PlayerTeam.Home))
            {
                tile.SetExpression(Expression.ONOES);
            }
            
            GameBoard.IsEnabled = false;
            sb.Completed += new EventHandler((object ob, EventArgs ea) => 
            {                 
                foreach (Tile ti in tiles)
                {
                    ti.RenderTransform = null;
                    ti.SetExpression(Expression.NORMAL);
                }
                App.Mine.Model.AnimationCompleted();
                GameBoard.IsEnabled = true;
            });
            sb.Begin(this);
            this.UnregisterName(transformName);
        }

        public void OnGameOver(object sender, GameOverEventArgs e)
        {
            m_gameOver = true;
            GameBoard.IsEnabled = false;
            GameResult.Opacity = 0;
            GameResult.Visibility = Visibility.Visible;
            if (e.Winner.Team == ObjectModel.PlayerTeam.Home)
            {
                // TODO: Play victory sound effect
                this.EndGame.Text = "You are victorious!";
                this.EndGame.Fill = GameBoard.HomeColor;
            }
            else
            {
                // TODO: Play defeat sound effect
                this.EndGame.Text = "You were defeated.";
                this.EndGame.Fill = GameBoard.AwayColor;
            }
            GameBoard.SetEndGameTiles(e.Winner.Team);
            Storyboard gameOverEffect = (Storyboard)FindResource("GameOverEffect");
            gameOverEffect.Begin(this);
        }

        void SetEndGameText(string txt, Brush color)
        {
            SolidColorBrush scb = color as SolidColorBrush;
            Color strokeColor = Colors.White;
            if (scb.Color.Equals(Colors.White))
            {
                strokeColor = Colors.Black;
            }
            SolidColorBrush stroke = new SolidColorBrush( strokeColor );
            this.EndGame.Text = txt;
            this.EndGame.Fill = scb;
            this.EndGame.Stroke = stroke;
            this.EndGame.StrokeThickness = 2;
        }

        void GameView_Loaded(object sender, RoutedEventArgs e)
        {
            SoundManager.Instance().PlayGameMusic();
            //game.GameOver += new EventHandler<GameOverEventArgs>(game_GameOver);
            //game.CurrentPlayer = game.Player1;
            //game.SetBoardState();      
            Storyboard tileRotator = (Storyboard)FindResource("TileRotator");
            tileRotator.Begin(this);
           
        }

        
    }
}
