using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if NETFX_CORE
namespace TetraSlide.Win8.ObjectModel
#else
namespace WPF.ObjectModel

#endif
{
    class UIPosition
    {

    }

#if COMENGINE
    public class Factory
    {
        public static IPosition NewPosition(int x, int y)
        {
            return new COMPosition(x, y);
        }
        public static IPlayer NewPlayer()
        {
            return new COMPlayer();
        }
    }
#else
    public class Factory
    {
        // Create WinRT objects here...
    }
#endif

    public enum PlayerTeam
    {
        Home = 0,
        Away = 1,
    }

    public enum PlayerType
    {
        Local = 0,
        Computer = 1,
        Remote = 2,
    }

    public enum TetradShape
    {
        None = 0,
        Square = 1,
        Line = 2,
        BackwardsL = 3,
        LShape = 4,
        Downstep = 5,
        Upstep = 6,
        Podium = 7,
    }

    public enum Direction
    {
        None = 0,
        Up = 1,
        Down = 2,
        Left = 3,
        Right = 4,
    }

    

    public interface ITurnCallback
    {
        void AnimationComplete();
        void DeleteRemoteGameComplete();
        void GetRemoteGameComplete(string game);
        void LocalTurnComplete(ITile Tile, IPosition target);
        void PutRemoteGameComplete();
    }

    public interface IPosition
    {
        short X { get; set; }
        short Y { get; set; }
        IPosition Translate(ObjectModel.Direction d, int distance);
    }

    public interface ITile
    {
        int Id { get; }
        IPosition Position { get; set; }
        PlayerTeam Team { get; }
        IEnumerable<IPosition> GetTargets();
    }

    public interface IPlayer
    {
        string Identity { get; set; }
        string Name { get; set; }
        uint RgbColor { get; set; }
        PlayerTeam Team { get; set; }
        PlayerType Type { get; set; }
        uint HasTetrad(TetradShape shape);
    }

    public interface IGameView
    {
        void AnimateTurn(ITurnCallback callback, IEnumerable<int> tileIds, Direction Direction, byte distance);
        void DeleteRemoteGame(ITurnCallback callback, string gameId);
        void DoLocalTurn(ITurnCallback callback);
        void GameOver(IPlayer winner);
        void GameResumed(IGameDescriptor game, PlayerTeam turn);
        void GameSuspended(IGameDescriptor game);
        string GetAuthToken();
        void GetRemoteGame(ITurnCallback callback, string gameId);
        IEnumerable<string> GetRemoteGames();
        void Log(string msg, string file, int Line);
        string NewRemoteGame(string localId, string remoteId);
        void PutRemoteGame(ITurnCallback callback, string gameId, string game);
        void StateChanged();
        void TetradFound(ITurnCallback callback, IPlayer Player, TetradShape shape, IEnumerable<int> tileIds);
    }

    public interface IEngine
    {
        IGameDescriptor CreateGame(IPlayer player1, IPlayer player2);
        IEnumerable<IGameDescriptor> GetGameDescriptors(string localId);
        void Init(IGameView view);
        void ResumeGame(IGameDescriptor game);
        void SuspendGame(IGameDescriptor game);
    }

    public interface IGameDescriptor
    {
        string FriendlyName { get; }
        string Id { get; }
        IPlayer GetAwayPlayer();
        IPlayer GetHomePlayer();
        IEnumerable<ITile> GetTiles();
        PlayerTeam GetTurn();
    }
}
