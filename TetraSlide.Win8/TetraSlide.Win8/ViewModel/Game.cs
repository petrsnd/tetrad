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
    class Game : DependencyObject
    {
        TsWinRT.TsGameDescriptor m_currentGame = null;
        TsWinRT.TsTurnCallback m_turnCompleteCallback = null;
        public Game(CreateGame options)
        {
            GameModel = new GameModel();
            IsGameOver = false;
            GameStatusText = "Initializing...";
            
            GameModel.GameView.OnGameResumed += GameView_OnGameResumed;
            GameModel.GameView.OnGameSuspended += GameView_OnGameSuspended;
            GameModel.GameView.OnDoLocalTurn += GameView_OnDoLocalTurn;
            GameModel.GameView.OnAnimateTurn += GameView_OnAnimateTurn;
            GameModel.GameView.OnStateChanged += GameView_OnStateChanged;
            GameModel.GameView.OnTetradFound += GameView_OnTetradFound;
            GameModel.GameView.OnGameOver += GameView_OnGameOver;
            
            App.RootFrame.Navigate(typeof(View.WaitPage), "Creating the game...");
            if (options.GameType == GameType.Solo)
            {
                GameModel.Resume(GameModel.CreateGameVsComputer(
                    options.GetHomeColor(),
                    options.GetDifficultyAsString()));
            }
            else if (options.GameType == GameType.PassNPlay)
            {
                GameModel.Resume(GameModel.CreateGamePassAndPlay(
                    options.GetHomeColor(),
                    options.GetAwayColor()));
            }
        }

        async void GameView_OnTetradFound(object sender, TetradFoundEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    GameBoard.GetBoard().AnimateTetrad(e, () =>
                    {
                        if (e.Player.Team == Player1.Team)
                            Player1.UpdateTetrads();
                        else
                            Player2.UpdateTetrads();
                    });

                });
        }




        public bool IsGameOver
        {
            get { return (bool)GetValue(IsGameOverProperty); }
            set { SetValue(IsGameOverProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsGameOver.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsGameOverProperty =
            DependencyProperty.Register("IsGameOver", typeof(bool), typeof(Game), null);


        async void GameView_OnGameOver(object sender, GameOverEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    this.IsGameOver = true;
                    GameBoard.GetBoard().IsHitTestVisible = false;
                    this.GameOverText = e.Winner.Name + " win" + (string.Compare(e.Winner.Name, "you", StringComparison.OrdinalIgnoreCase) == 0 ? "!" : "s!");
                    GameBoard.SetGameOverText(this.GameOverText);
                    GameBoard.GetBoard().GameOver(e.Winner);
                });
        }

        async void GameView_OnStateChanged(object sender, StateChangedEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    UpdatePlayerStatus();
                    GameBoard.GetBoard().StateChanged(m_currentGame);
                });
        }

        async void GameView_OnAnimateTurn(object sender, AnimateTurnEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    GameBoard.GetBoard().AnimateMove( m_currentGame, e );
                });
        }

        async void GameView_OnDoLocalTurn(object sender, DoLocalTurnEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    // Engine will wait for the UI to call the callback function
                    // which gets everything moving again. :)
                    m_turnCompleteCallback = e.Callback;
                    Controls.Board boardControl = GameBoard.GetBoard();
                    boardControl.CurrentTurn = e.Turn;
                    boardControl.InteractionEnabled = true;
                    
                });
        }

        void boardControl_AnimationComplete(object sender, EventArgs e)
        {
            m_turnCompleteCallback.AnimationComplete();
        }

        void boardControl_TileMoved(object sender, TileMovedEventArgs e)
        {
            m_turnCompleteCallback.LocalTurnComplete(e.Tile.EngineTile, e.Target);
        }

        void UpdatePlayerStatus()
        {
            var currentPlayer = m_currentGame.Turn == TsWinRT.TsPlayerTeam.TsTeamHome ? Player1 : Player2;
            GameStatusText = currentPlayer.TurnName + " Turn";
            CurrentPlayerBrush = currentPlayer.TeamBrush;
        }

        async void GameView_OnGameResumed(object sender, GameResumedEventArgs e)
        {
            await this.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                () =>
                {
                    m_currentGame = e.GameDescriptor;
                    this.Player1 = new Player(e.GameDescriptor.HomePlayer);
                    this.Player2 = new Player(e.GameDescriptor.AwayPlayer);
                    this.GameBoard = new View.GameBoard();
                    this.GameBoard.Loaded += (s, args) =>
                    {
                        Controls.Board boardControl = this.GameBoard.GetBoard();
                        boardControl.GameDescriptor = m_currentGame;
                        boardControl.TileMoved += boardControl_TileMoved;
                        boardControl.AnimationComplete += boardControl_AnimationComplete;
                    };
                    UpdatePlayerStatus();
                    App.RootFrame.Navigate(typeof(TsPage), new PageContent(
                        this,
                        this.GameBoard,
                        new View.GameSnapped()));
                });
        }

        View.GameBoard GameBoard
        {
            get;
            set;
        }

        private bool m_suspending = false;
        public event EventHandler<GameSuspendedEventArgs> GameSuspended;
        public async void Suspend()
        {
            if (!m_suspending)
            {
                if (!IsGameOver)
                {
                    m_suspending = true;
                    await Task.Delay(2000); // Simulate delay on suspend
                    GameModel.Suspend();
                }
            }
        }
        void GameView_OnGameSuspended(object sender, GameSuspendedEventArgs e)
        {
            if (GameSuspended != null)
                GameSuspended(this, e);
            m_suspending = false;
        }

        public Player Player1
        {
            get;
            private set;
        }

        public Player Player2
        {
            get;
            private set;
        }

        public string GameOverText
        {
            get { return (string)GetValue(GameOverTextProperty); }
            set { SetValue(GameOverTextProperty, value); }
        }
        // Using a DependencyProperty as the backing store for GameOverText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GameOverTextProperty =
            DependencyProperty.Register("GameOverText", typeof(string), typeof(Game), null);

        public string GameStatusText
        {
            get { return (string)GetValue(GameStatusTextProperty); }
            set { SetValue(GameStatusTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for GameStatusText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty GameStatusTextProperty =
            DependencyProperty.Register("GameStatusText", typeof(string), typeof(Game), new PropertyMetadata("", new PropertyChangedCallback( (obj, args) =>
            {
                string newvalue = args.NewValue as string;

            })));



        public Brush CurrentPlayerBrush
        {
            get { return (Brush)GetValue(CurrentPlayerBrushProperty); }
            set { SetValue(CurrentPlayerBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentPlayerBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentPlayerBrushProperty =
            DependencyProperty.Register("CurrentPlayerBrush", typeof(Brush), typeof(Game), new PropertyMetadata(new SolidColorBrush(Colors.White)));



        private GameModel GameModel
        {
            get;
            set;
        }
    }
}
