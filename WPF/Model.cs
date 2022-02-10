using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Threading;
using TetraSlide.Api.Clients;
using TetraSlide.Api.Models;

namespace WPF
{

#if COMENGINE
    [ComVisible(true)]
	public class Model : ObjectModel.IGameView
    {
        ObjectModel.IEngine m_engine = new ObjectModel.COMEngine();
        public Model(System.Windows.Threading.Dispatcher dispatcher)
        {
            UIThread = dispatcher;
            m_engine.Init(this);
        }
#else
    // WINRT Stuff here
    public class Model
    {
        public Model(System.Windows.Threading.Dispatcher dispatcher)
        {
            UIThread = dispatcher;
            m_engine.Init(this);
        }
#endif

        System.Windows.Threading.Dispatcher UIThread = null;
        ObjectModel.IGameDescriptor m_currentGame = null;
        
        

        #region Events
        public event EventHandler<OpponentMoveCompleteEventArgs> OpponentMoveComplete;
        public event EventHandler<TetradFoundEventArgs> TetradFound;
        public event EventHandler<GameOverEventArgs> GameOver;
        public event EventHandler<GameResumedEventArgs> GameResumed;
        public event EventHandler<GameSuspendedEventArgs> GameSuspended;
        public event EventHandler<GameStateChangedArgs> GameStateChanged;
        public event EventHandler<LoggedInEventArgs> LoggedIn;
        #endregion

        #region Public Properties
        public string AuthToken
        {
            get
            {
                return UIThread.Invoke(() =>
                    {
                        if (string.IsNullOrEmpty(Properties.Settings.Default.AuthToken))
                            return null;
                        return Properties.Settings.Default.AuthToken;
                    });
            }
            set
            {
                UIThread.Invoke(() =>
                    {
                        Properties.Settings.Default.AuthToken = value;
                        Properties.Settings.Default.Save();
                    });
            }
        }
        #endregion

        #region Public Methods

        public Task<IEnumerable<ObjectModel.IGameDescriptor>> GetGamesAsync()
        {
            return Task.Run<IEnumerable<ObjectModel.IGameDescriptor>>(
                () => 
                {
                    var ret = new List<ObjectModel.IGameDescriptor>();
                    IEnumerable<ObjectModel.IGameDescriptor> retval = m_engine.GetGameDescriptors(GetMyAccount().email_address);
                    if (retval == null)
                        return ret;
                    foreach (ObjectModel.IGameDescriptor gd in retval)
                    {
                        if (gd != null)
                            ret.Add(gd);
                    }
                    return ret;
                });
        }

        public void SuspendGame()
        {
            m_engine.SuspendGame(m_currentGame);
        }

        public void CreateGameVsComputer(ObjectModel.IPlayer player, string difficulty)
        {
            ObjectModel.IPlayer computer = ObjectModel.Factory.NewPlayer();
            computer.Type = ObjectModel.PlayerType.Computer;
            computer.RgbColor = 0xffffffff;
            computer.Team = ObjectModel.PlayerTeam.Away;
            computer.Identity = difficulty;
            ObjectModel.IGameDescriptor gameDescriptor = m_engine.CreateGame(player, computer);
            if (gameDescriptor != null)
            {
                ResumeGame(gameDescriptor);
            }
            else
            {
                throw new GameCreationFailedException();
            }
        }

        public async Task<ObjectModel.IGameDescriptor> CreatePvpGameAsync(ObjectModel.IPlayer player1, ObjectModel.IPlayer player2)
        {
            return await Task.Run<ObjectModel.IGameDescriptor>(
                () =>
                {
                    try
                    {
                        ObjectModel.IGameDescriptor gameDescriptor = m_engine.CreateGame(player1, player2);
                        if (gameDescriptor != null)
                        {
                            return gameDescriptor;
                        }
                    }
                    catch (Exception)
                    { }
                    UIThread.Invoke(new Action(() =>
                        {
                            App.Mine.Navigator.SetCurrentView(new MessageView() { Message = "Unable to contact the server!" });                              
                        }));
                    return null;
                });
        }

        BackgroundWorker m_gameThread = null;
        public void ResumeGame(ObjectModel.IGameDescriptor game)
        {
            m_currentGame = game;
            m_gameThread = new BackgroundWorker();
            m_gameThread.RunWorkerCompleted += (sender, e) =>
            {
                // The game is over?  I don't think there's anything to do
                m_gameThread = null;
            };
            m_gameThread.DoWork += (sender, e) =>
            {
                m_engine.ResumeGame(game);
            };
            m_gameThread.RunWorkerAsync();
        }

        ObjectModel.ITurnCallback turnCallback = null;
        public void LocalTurnCompleted(ObjectModel.ITile from, ObjectModel.IPosition to)
        {
            if (turnCallback == null)
                return;
            turnCallback.LocalTurnComplete(from, to);
        }

        ObjectModel.ITurnCallback animationCallback = null;
        public void AnimationCompleted()
        {
            if (animationCallback == null)
                return;
            animationCallback.AnimationComplete();            
        }
        #endregion

        #region IGameView Implementation
        public void AnimateTurn(ObjectModel.ITurnCallback callback, IEnumerable<int> tileIds, ObjectModel.Direction Direction, byte distance)
        {
            animationCallback = callback;
            if (OpponentMoveComplete != null)
            {
                UIThread.Invoke(new Action(() =>
                {
                    OpponentMoveComplete(this, new OpponentMoveCompleteEventArgs(tileIds, Direction, distance));
                }));
            }
        }

        public void DoLocalTurn(ObjectModel.ITurnCallback callback)
        {
            turnCallback = callback;
        }

        void ObjectModel.IGameView.GameOver(ObjectModel.IPlayer winner)
        {
            if (GameOver != null)
            {
                UIThread.Invoke(new Action(() =>
                {
                    GameOver(this, new GameOverEventArgs(winner));
                }));
            }
        }

        void ObjectModel.IGameView.GameResumed(ObjectModel.IGameDescriptor game, ObjectModel.PlayerTeam turn)
        {
            Debug.Assert(GameResumed != null);
            if (GameResumed != null)
            {
                UIThread.Invoke(new Action(() =>
                    {
                        GameResumed(this, new GameResumedEventArgs(game, turn));
                    }));
            }
        }

        void ObjectModel.IGameView.GameSuspended(ObjectModel.IGameDescriptor game)
        {
            // TODO: This really needs to take a string
            // I'm commenting this assert to prevent crashing on back button, but we need to wire this up properly.
            // Debug.Assert(GameSuspended != null);
            if (GameSuspended != null)
                GameSuspended(this, new GameSuspendedEventArgs(game));
        }

        string ObjectModel.IGameView.GetAuthToken()
        {
            return AuthToken;
        }

        public void Log(string msg, string file, int line)
        {
            Logger.Log(msg, file, line.ToString());
        }

        public IEnumerable<string> GetRemoteGames()
        {
            GamesClient gc = GetGamesClient();
            if (gc == null)
                return null;
            var games = gc.GetMine();
            if (games != null)
            {
                var datas = games.Select<Game, string>(g => g.data);
                return datas;
            }
            return null;
        }

        public string NewRemoteGame(string localId, string remoteId)
        {
            var gc = GetGameClient();
            if (gc == null)
                throw new TsException("Failed to create new remote game.");

            var game = gc.Create(localId, remoteId, "");
            return game.id;
        }

        public void GetRemoteGame(ObjectModel.ITurnCallback callback, string gameId)
        {
            GameClient gc = GetGameClient();
            Game g = null;
            BackgroundWorker wsCall = new BackgroundWorker();
            wsCall.RunWorkerCompleted += (sender, e) =>
            {
                if (g == null)
                    callback.GetRemoteGameComplete(null);
                else
                    callback.GetRemoteGameComplete(g.data);
            };
            wsCall.DoWork += (sender, e) =>
            {
                if (gc == null)
                    callback.GetRemoteGameComplete(null);
                g = gc.Get(gameId);  
            };
            wsCall.RunWorkerAsync();
        }

        public void PutRemoteGame(ObjectModel.ITurnCallback callback, string gameId, string game)
        {
            
            GameClient gc = GetGameClient();
            BackgroundWorker wsCall = new BackgroundWorker();
            wsCall.RunWorkerCompleted += (sender, e) =>
            {
                callback.PutRemoteGameComplete();
            };
            wsCall.DoWork += (sender, e) =>
            {
                if (gc == null)
                    throw new TsException("Failed to put game.");
                gc.Update(gameId, game);
            };
            wsCall.RunWorkerAsync();
        }

        public void DeleteRemoteGame(ObjectModel.ITurnCallback callback, string gameId)
        {
            GameClient gc = GetGameClient();
            if (gc == null)
                callback.DeleteRemoteGameComplete();

            gc.Delete(gameId);
            callback.DeleteRemoteGameComplete();
        }

        void ObjectModel.IGameView.TetradFound(ObjectModel.ITurnCallback callback, ObjectModel.IPlayer player, ObjectModel.TetradShape shape, IEnumerable<int> tileIds)
        {
            UIThread.Invoke(new Action(() =>
                {
                    OnTetradFound(callback, player, shape, tileIds);
                }));
        }

        void ObjectModel.IGameView.StateChanged()
        {
            UIThread.Invoke(new Action( () =>
                {
                    if( GameStateChanged != null )
                        GameStateChanged(this, new GameStateChangedArgs() );
                }));
        }
        #endregion

        protected void OnTetradFound(ObjectModel.ITurnCallback callback, ObjectModel.IPlayer player, ObjectModel.TetradShape shape, IEnumerable<int> tileIds)
        {
            animationCallback = callback;
            if( TetradFound != null )
            {
                UIThread.Invoke( new Action( () =>
                {
                    TetradFound( this, new TetradFoundEventArgs( player, shape, tileIds )); 
                }));
            }
        }

        GamesClient GetGamesClient()
        {
            string authToken = GetAuthTokenAsync().Result;
            if (authToken == null)
                return null;
            return new GamesClient(authToken);
        }

        GameClient GetGameClient()
        {
            string authToken = GetAuthTokenAsync().Result;
            if (authToken == null)
                return null;
            return new GameClient(authToken);
        }

        public Task<Account> GetMyAccountAsync()
        {
            return Task.Run<Account>(() =>
                {
                    if (AuthToken != null)
                    {
                        AccountClient act = new AccountClient(AuthToken);
                        Account account = act.GetMy();
                        if (account != null)
                        {
                            if (LoggedIn != null)
                            {
                                UIThread.BeginInvoke(new Action( () =>
                                    {
                                        LoggedIn(this, new LoggedInEventArgs(account));
                                    }));
                            }
                            return account;
                        }
                    }
                    return null;
                });
        }

        public Account GetMyAccount()
        {
            string authToken = GetAuthTokenAsync().Result;
            return GetMyAccountAsync().Result;
        }



        string m_validatedToken = null;
        async Task<string> GetAuthTokenAsync(bool canRetry = true)
        {
            if (m_validatedToken != null) return m_validatedToken;
            string tmp = AuthToken;
            if (tmp == null)
            {
                tmp = await LoadAuthTokenAsync();
                canRetry = false;
            }
            if (await ValidateTokenAsync(tmp))
            {
                AuthToken = tmp;
                m_validatedToken = tmp;
            }
            else
            {
                if (canRetry)
                    AuthToken =  await GetAuthTokenAsync(false);
                else
                    AuthToken = null;
            }
            return AuthToken;
        }

        Task<string> LoadAuthTokenAsync()
        {
            return Task.Run<string>(new Func<string>(() =>
            {
                AuthTokenLoader loader = new AuthTokenLoader();
                loader.Load(UIThread);
                if (loader.Success)
                    return loader.AuthToken;
                throw new TsException(loader.Error);
            }));
        }

        Task<bool> ValidateTokenAsync(string token)
        {
            return Task.Run<bool>(new Func<bool>(() =>
            {
                try
                {
                    AccountClient act = new AccountClient(token);
                    act.GetMy();
                    return true;
                }
                catch (TetraSlide.Api.Clients.ApiClientException )
                {
                    return false;
                }
            }));
        }
    }
}
