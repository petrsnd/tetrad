using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPF
{
    public class GameManager
    {
        static GameManager s_instance = null;
        public static GameManager Instance()
        {
            if (s_instance == null)
                s_instance = new GameManager();
            return s_instance;
        }

        public Model Model
        {
            get;
            private set;
        }

        bool m_initialized = false;
        public void InitGameModel( Main mainWindow )
        {
            if (m_initialized)
                throw new Exception("Hey, the game model has already been initialized! Not cool.");
            MainWindow = mainWindow;
            Model = new Model(mainWindow.Dispatcher);
            Model.GameResumed += Model_GameResumed;
            SoundManager sm = SoundManager.Instance();
            Model.TetradFound += new EventHandler<TetradFoundEventArgs>(sm.OnTetradFound);
            Model.GameOver += new EventHandler<GameOverEventArgs>(sm.OnGameOver);
            m_initialized = true;
        }

        void Model_GameResumed(object sender, GameResumedEventArgs e)
        {
            ResumeGame(e.GameDescriptor);
        }

        public void ResumeGame(ObjectModel.IGameDescriptor gameDescriptor)
        {
            if (ViewModel != null || View != null)
                SuspendGame();

            ViewModel = new GameViewModel();
            View = new GameView();
            View.DataContext = ViewModel;
            View.GameBoard.TileMoved += new EventHandler<TileMovedEventArgs>(ViewModel.OnTileMoved);
            View.GameBoard.TileSelected += new EventHandler<TileSelectedEventArgs>(ViewModel.OnTileSelected);
            Model.GameOver += new EventHandler<GameOverEventArgs>(View.OnGameOver);
            Model.GameOver += new EventHandler<GameOverEventArgs>(ViewModel.OnGameOver);
            Model.TetradFound += new EventHandler<TetradFoundEventArgs>(View.OnTetradFound);
            ViewModel.TilesUpdated += new EventHandler<TilesUpdatedEventArgs>(View.OnTilesUpdated);
            ViewModel.Initialize(Model, gameDescriptor);
            Model.OpponentMoveComplete += Model_OpponentMoveComplete;

            App.Mine.Navigator.ViewPopped += Navigator_ViewPopped;
            App.Mine.Navigator.SetCurrentView(View);
        }

        void Model_OpponentMoveComplete(object sender, OpponentMoveCompleteEventArgs e)
        {
            View.GameBoard.PerformMove(e.TileIds, e.Direction, e.Distance,
                    new Action(() =>
                    {
                        View.GameBoard.IsEnabled = true;
                        Model.AnimationCompleted();
                    }));
        }

        void Navigator_ViewPopped(object sender, ViewPoppedEventArgs e)
        {
            if( e.View == View )
                SuspendGame();
        }

        public void SuspendGame()
        {
            // Unhook events, clear View and ViewModel
            if (View != null && ViewModel != null)
            {
                View.GameBoard.TileMoved -= new EventHandler<TileMovedEventArgs>(ViewModel.OnTileMoved);
                View.GameBoard.TileSelected -= new EventHandler<TileSelectedEventArgs>(ViewModel.OnTileSelected);
                Model.GameOver -= new EventHandler<GameOverEventArgs>(View.OnGameOver);
                Model.GameOver -= new EventHandler<GameOverEventArgs>(ViewModel.OnGameOver);
                Model.TetradFound -= new EventHandler<TetradFoundEventArgs>(View.OnTetradFound);
                ViewModel.TilesUpdated -= new EventHandler<TilesUpdatedEventArgs>(View.OnTilesUpdated);
                View.DataContext = null;
                View = null;
                ViewModel = null;
            }
            Model.SuspendGame();
        }

        private GameViewModel ViewModel
        {
            get;
            set;
        }

        private GameView View
        {
            get;
            set;
        }

        private Main MainWindow
        {
            get;
            set;
        }

        private GameManager()
        {
        }
    }
}
