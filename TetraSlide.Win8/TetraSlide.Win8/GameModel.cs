using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Popups;

namespace TetraSlide.Win8
{
    public class GameModel
    {
        public GameModel()
        {
            GameView = new GameView();
        }

        public string GetVersion()
        {
            return GameView.Engine.Version();
        }

        public GameView GameView
        {
            get;
            private set;
        }

        public string Suspend()
        {
            string gameData = null;
            if (_currentGame != null)
            {
                gameData = GameView.Engine.SaveGame(_currentGame);
            }
            _currentGame = null;
            return gameData;
        }

        private Task _currentGameTask;
        private TsWinRT.TsGameDescriptor _currentGame;
        public void Resume(TsWinRT.TsGameDescriptor gameDescriptor)
        {
            _currentGame = gameDescriptor;
            _currentGameTask = new Task( async () =>
            {
                await Task.Delay(500);  // Simulate game creation delay.
                GameView.Engine.ResumeGame(gameDescriptor);
            });
            _currentGameTask.Start();
        }

        #region Game Creation Methods
        public TsWinRT.TsGameDescriptor LoadSavedGame(string gameData)
        {
            return GameView.Engine.LoadGame(gameData);
        }
        public TsWinRT.TsGameDescriptor CreateGameVsComputer(int homeColor, string difficulty)
        {
            var away = GetPlayerSpec(TsWinRT.TsPlayerType.TsPlayerComputer, TsWinRT.TsPlayerTeam.TsTeamAway, difficulty, "CPU", unchecked((int)0xFF235599)); // TODO: fix color?  random?
            return CreateGame(CreateHomePlayer(homeColor), away);
        }

        public TsWinRT.TsGameDescriptor CreateGamePassAndPlay(int homeColor, int awayColor)
        {
            var home = GetPlayerSpec(TsWinRT.TsPlayerType.TsPlayerLocal, TsWinRT.TsPlayerTeam.TsTeamHome, "Player 1", "Player 1", homeColor);
            var away = GetPlayerSpec(TsWinRT.TsPlayerType.TsPlayerLocal, TsWinRT.TsPlayerTeam.TsTeamAway, "Player 2", "Player 2", awayColor);
            return CreateGame(home, away);
        }

        public TsWinRT.TsGameDescriptor CreateGameRemote(int homeColor, string awayId)
        {
            // TODO: some network stuff?
            var away = GetPlayerSpec(TsWinRT.TsPlayerType.TsPlayerRemote, TsWinRT.TsPlayerTeam.TsTeamAway, awayId, awayId, 0);
            return CreateGame(CreateHomePlayer(homeColor), away);
        }
        #endregion

        #region Private Methods
        private TsWinRT.ITsPlayer GetPlayerSpec(TsWinRT.TsPlayerType type, TsWinRT.TsPlayerTeam team, string id, string name, int color)
        {
            return new TsWinRT.TsPlayerSpec()
            {
                Type = type,
                Identity = id,
                Name = name,
                Color = color,
                Team = team
            };
        }

        private TsWinRT.ITsPlayer CreateHomePlayer(int color)
        {
            // TODO: some stuff for id... maybe get it from Metro if logged in with Windows Live, etc...
            return GetPlayerSpec(TsWinRT.TsPlayerType.TsPlayerLocal, TsWinRT.TsPlayerTeam.TsTeamHome, "TODO", "You", color);
        }

        private TsWinRT.TsGameDescriptor CreateGame(TsWinRT.ITsPlayer home, TsWinRT.ITsPlayer away)
        {
            return GameView.Engine.CreateGame(home, away);
        }
        #endregion
    }
}
