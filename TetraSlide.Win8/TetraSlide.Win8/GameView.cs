using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TetraSlide.Win8
{
    public class GameView : TsWinRT.ITsView
    {
        TsWinRT.TsGameDescriptor m_currentGame = null;
        public GameView()
        {
            Engine = new TsWinRT.TsEngine();
            Engine.Init(this);
            OnTetradFound += SoundManager.Instance().OnTetradFound;
            OnGameResumed += SoundManager.Instance().OnGameResumed;
            OnGameSuspended += SoundManager.Instance().OnGameSuspended;
            OnGameOver += SoundManager.Instance().OnGameOver;
        }

        public event EventHandler<GameResumedEventArgs> OnGameResumed;
        public event EventHandler<GameSuspendedEventArgs> OnGameSuspended;
        public event EventHandler<GameOverEventArgs> OnGameOver;
        public event EventHandler<StateChangedEventArgs> OnStateChanged;
        public event EventHandler<TetradFoundEventArgs> OnTetradFound;
        public event EventHandler<DoLocalTurnEventArgs> OnDoLocalTurn;
        public event EventHandler<AnimateTurnEventArgs> OnAnimateTurn;


        // TODO: Turn is not needed here.  game descriptor has it!
        public void GameResumed(TsWinRT.TsGameDescriptor game, TsWinRT.TsPlayerTeam turn)
        {
            m_currentGame = game;
            if (OnGameResumed != null)
                OnGameResumed(this, new GameResumedEventArgs(game));
        }
        public void GameSuspended(TsWinRT.TsGameDescriptor game)
        {
            if (OnGameSuspended != null)
                OnGameSuspended(this, new GameSuspendedEventArgs(game));
        }
        public void GameOver(TsWinRT.ITsPlayer winner)
        {
            // TODO:
            if (OnGameOver != null)
                OnGameOver(this, new GameOverEventArgs(winner));
        }
        public void StateChanged()
        {
            // TODO:
            if (OnStateChanged != null)
                OnStateChanged(this, new StateChangedEventArgs());
        }
        public void TetradFound(TsWinRT.TsTurnCallback callback, TsWinRT.ITsPlayer player, TsWinRT.TsTetradShape shape, IList<TsWinRT.TsTile> tiles)
        {
            // TODO:
            if (OnTetradFound != null)
                OnTetradFound(this, new TetradFoundEventArgs(player, shape, tiles));
        }
        public void DoLocalTurn(TsWinRT.TsTurnCallback callback)
        {
            // TODO:
            if (OnDoLocalTurn != null)
                OnDoLocalTurn(this, new DoLocalTurnEventArgs(callback, m_currentGame.Turn));
        }
        public void AnimateTurn(TsWinRT.TsTurnCallback callback, IList<TsWinRT.TsTile> tiles, TsWinRT.TsBoardDirection dir, int dist)
        {
            // TODO:
            if (OnAnimateTurn != null)
                OnAnimateTurn(this, new AnimateTurnEventArgs(callback, tiles, dir, dist));
        }
        public void Log(string msg, string file, int line)
        {
            // TODO:
            //int breakhere = 100;
        }
        public IList<string> GetRemoteGames()
        {
            // TODO:
            var games = new List<string>();
            return games.ToArray();
        }
        public string NewRemoteGame(string localId, string remoteId)
        {
            // TODO:
            return "";
        }
        public void GetRemoteGame(TsWinRT.TsTurnCallback callback, string gameId)
        {
            // TODO:
            //int breakhere = 100;
        }
        public void PutRemoteGame(TsWinRT.TsTurnCallback callback, string gameId, string payload)
        {
            // TODO:
            //int breakhere = 100;
        }
        public void DeleteRemoteGame(TsWinRT.TsTurnCallback callback, string gameId)
        {
            // TODO:
            //int breakhere = 100;
        }

        internal TsWinRT.TsEngine Engine
        {
            get;
            private set;
        }
    }
}
