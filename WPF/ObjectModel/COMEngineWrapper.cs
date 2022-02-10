using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPF.ObjectModel
{

    internal class Convert
    {
        internal static Direction Direction(TsCOMLib.Direction d)
        {
            switch (d)
            {
                case TsCOMLib.Direction.Down: return WPF.ObjectModel.Direction.Down;
                case TsCOMLib.Direction.Left: return WPF.ObjectModel.Direction.Left;
                case TsCOMLib.Direction.Right: return WPF.ObjectModel.Direction.Right;
                case TsCOMLib.Direction.Up: return WPF.ObjectModel.Direction.Up;
            }
            return WPF.ObjectModel.Direction.None;
        }
        internal static WPF.ObjectModel.PlayerType PlayerType(TsCOMLib.PlayerType pt)
        {
            switch (pt)
            {
                case TsCOMLib.PlayerType.Computer: return WPF.ObjectModel.PlayerType.Computer;
                case TsCOMLib.PlayerType.Local: return WPF.ObjectModel.PlayerType.Local;
                case TsCOMLib.PlayerType.Remote: return WPF.ObjectModel.PlayerType.Remote;
            }
            throw new Exception("Fail");
        }
        internal static TsCOMLib.PlayerType PlayerType(WPF.ObjectModel.PlayerType pt)
        {
            switch (pt)
            {
                case WPF.ObjectModel.PlayerType.Computer: return TsCOMLib.PlayerType.Computer;
                case WPF.ObjectModel.PlayerType.Local: return TsCOMLib.PlayerType.Local;
                case WPF.ObjectModel.PlayerType.Remote: return TsCOMLib.PlayerType.Remote;
            }
            throw new Exception("Fail");
        }
        internal static WPF.ObjectModel.PlayerTeam PlayerTeam(TsCOMLib.PlayerTeam pt)
        {
            switch (pt)
            {
                case TsCOMLib.PlayerTeam.Away: return WPF.ObjectModel.PlayerTeam.Away;
            }
            return WPF.ObjectModel.PlayerTeam.Home;
        }
        internal static TsCOMLib.PlayerTeam PlayerTeam(PlayerTeam pt)
        {
            switch (pt)
            {
                case WPF.ObjectModel.PlayerTeam.Away: return TsCOMLib.PlayerTeam.Away;
            }
            return TsCOMLib.PlayerTeam.Home;
        }
        internal static TetradShape TetradShape(TsCOMLib.TetradShape s)
        {
            switch (s)
            {
                case TsCOMLib.TetradShape.BackwardsL: return ObjectModel.TetradShape.BackwardsL;
                case TsCOMLib.TetradShape.Downstep: return ObjectModel.TetradShape.Downstep;
                case TsCOMLib.TetradShape.Line: return ObjectModel.TetradShape.Line;
                case TsCOMLib.TetradShape.LShape: return ObjectModel.TetradShape.LShape;
                case TsCOMLib.TetradShape.Podium: return ObjectModel.TetradShape.Podium;
                case TsCOMLib.TetradShape.Square: return ObjectModel.TetradShape.Square;
                case TsCOMLib.TetradShape.Upstep: return ObjectModel.TetradShape.Upstep;
            }
            return ObjectModel.TetradShape.None;
        }
        internal static TsCOMLib.TetradShape TetradShape(TetradShape s)
        {
            switch (s)
            {
                case WPF.ObjectModel.TetradShape.BackwardsL: return TsCOMLib.TetradShape.BackwardsL;
                case WPF.ObjectModel.TetradShape.Downstep:  return TsCOMLib.TetradShape.Downstep;
                case WPF.ObjectModel.TetradShape.Line:      return TsCOMLib.TetradShape.Line; 
                case WPF.ObjectModel.TetradShape.LShape:    return TsCOMLib.TetradShape.LShape; 
                case WPF.ObjectModel.TetradShape.Podium:    return TsCOMLib.TetradShape.Podium; 
                case WPF.ObjectModel.TetradShape.Square:    return TsCOMLib.TetradShape.Square; 
                case WPF.ObjectModel.TetradShape.Upstep:    return TsCOMLib.TetradShape.Upstep; 
            }
            return TsCOMLib.TetradShape.None;
        }
    }

    [System.Runtime.InteropServices.ComVisible(true)]
    public class COMTurnCallback : ITurnCallback
    {
        TsCOMLib.TurnCallback m_callback;
        internal COMTurnCallback(TsCOMLib.TurnCallback cb)
        {
            m_callback = cb;
        }
        public void AnimationComplete() { m_callback.AnimationComplete(); }
        public void DeleteRemoteGameComplete() { m_callback.DeleteRemoteGameComplete(); }
        public void GetRemoteGameComplete(string game) { m_callback.GetRemoteGameComplete(game); }
        public void LocalTurnComplete(ITile Tile, IPosition target)
        {
            TsCOMLib.Tile t = (Tile as COMTile).GetTile();
            TsCOMLib.Position p = (target as COMPosition).GetPosition();
            m_callback.LocalTurnComplete( t,p );
        }
        public void PutRemoteGameComplete()
        {
            m_callback.PutRemoteGameComplete();
        }
    }


    public class COMPosition : IPosition
    {
        TsCOMLib.Position m_pos;
        internal COMPosition(TsCOMLib.Position pos)
        {
            m_pos = pos;
        }
        internal COMPosition(int x, int y)
        {
            m_pos = new TsCOMLib.Position();
            X = (short)x;
            Y = (short)y;
        }
        public short X
        {
            get
            {
                return m_pos.X;
            }
            set
            {
                m_pos.X = value;
            }
        }
        public short Y
        {
            get
            {
                return m_pos.Y;
            }
            set
            {
                m_pos.Y = value;
            }
        }
        internal TsCOMLib.Position GetPosition() { return m_pos; }
        public override bool Equals(object obj)
        {
            if (obj is TsCOMLib.Position)
            {
                TsCOMLib.Position other = obj as TsCOMLib.Position;
                return this.X == other.X && this.Y == other.Y;
            }
            if (!(obj is IPosition))
            {
                return false;
            }
            IPosition p = obj as IPosition;
            return this.X == p.X && this.Y == p.Y;
        }

        public IPosition Translate(ObjectModel.Direction d, int distance)
        {
            int dist = Math.Abs(distance);
            switch (d)
            {
                case ObjectModel.Direction.Up: return new COMPosition(X, Y - dist);
                case ObjectModel.Direction.Down: return new COMPosition(X, Y + dist);
                case ObjectModel.Direction.Left: return new COMPosition(X - dist, Y);
                case ObjectModel.Direction.Right: return new COMPosition(X + dist, Y);
            };
            return new COMPosition(X, Y);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

    }

    [System.Runtime.InteropServices.ComVisible(true)]
    public class COMTile : ITile
    {
        TsCOMLib.Tile m_tile;
        internal COMTile(TsCOMLib.Tile t)
        {
            m_tile = t;
        }
        public int Id { get { return m_tile.Id; } }
        public IPosition Position
        {
            get
            {
                return new COMPosition(m_tile.Position);
            }
            set
            {
                m_tile.Position = (value as COMPosition).GetPosition();
            }
        }
        public PlayerTeam Team { get { return Convert.PlayerTeam(m_tile.Team); } }
        public IEnumerable<IPosition> GetTargets() 
        {
            foreach (TsCOMLib.Position pos in m_tile.GetTargets())
                yield return new COMPosition(pos);
        }
        internal TsCOMLib.Tile GetTile() { return m_tile; }
    }

    [System.Runtime.InteropServices.ComVisible(true)]
    public class COMPlayer : IPlayer
    {
        TsCOMLib.Player m_player;
        internal COMPlayer(TsCOMLib.Player p)
        {
            m_player = p;
        }
        internal COMPlayer()
        {
            m_player = new TsCOMLib.Player();
        }
        public string Identity 
        {
            get { return m_player.Identity; }
            set { m_player.Identity = value; }
        }
        public string Name
        {
            get { return m_player.Name; }
            set { m_player.Name = value; }
        }
        public uint RgbColor
        {
            get { return m_player.RgbColor; }
            set { m_player.RgbColor = value; }
        }
        public PlayerTeam Team
        {
            get { return Convert.PlayerTeam(m_player.Team); }
            set { m_player.Team = Convert.PlayerTeam(value); }
        }
        public PlayerType Type
        {
            get { return Convert.PlayerType(m_player.Type); }
            set { m_player.Type = Convert.PlayerType(value); }
        }
        public uint HasTetrad(TetradShape shape)
        {
            return m_player.HasTetrad(Convert.TetradShape(shape));
        }
        internal TsCOMLib.Player GetPlayer() { return m_player; }
    }

    //public class COMGameView : IGameView
    //{
    //    void AnimateTurn(ITurnCallback callback, Array tileIds, Direction Direction, byte distance);
    //    void DeleteRemoteGame(ITurnCallback callback, string gameId);
    //    void DoLocalTurn(ITurnCallback callback);
    //    void GameOver(IPlayer winner);
    //    void GameResumed(IGameDescriptor game, PlayerTeam turn);
    //    void GameSuspended(IGameDescriptor game);
    //    string GetAuthToken();
    //    void GetRemoteGame(ITurnCallback callback, string gameId);
    //    Array GetRemoteGames();
    //    void Log(string msg, string file, int Line);
    //    string NewRemoteGame(string localId, string remoteId);
    //    void PutRemoteGame(ITurnCallback callback, string gameId, string game);
    //    void StateChanged();
    //    void TetradFound(ITurnCallback callback, IPlayer Player, TetradShape shape, Array tileIds);
    //}

    [System.Runtime.InteropServices.ComVisible(true)]
    public class COMEngine : IEngine, TsCOMLib.GameView
    {
        TsCOMLib.Engine m_engine;
        IGameView m_view;
        public COMEngine()
        {
            m_engine = new TsCOMLib.Engine();
        }
        public IGameDescriptor CreateGame(IPlayer player1, IPlayer player2)
        {
            TsCOMLib.Player p1 = (player1 as COMPlayer).GetPlayer();
            TsCOMLib.Player p2 = (player2 as COMPlayer).GetPlayer();
            return new COMGameDescriptor( m_engine.CreateGame(p1, p2));
        }

        public IEnumerable<IGameDescriptor> GetGameDescriptors(string localId)
        {
            var gds = m_engine.GetGameDescriptors(localId);
            if (gds == null) 
                yield break;
            foreach (TsCOMLib.GameDescriptor gd in gds)
            {
                yield return new COMGameDescriptor(gd);
            }
        }

        public void Init(IGameView view)
        {
            m_view = view;
            m_engine.Init(this);
        }

        public void ResumeGame(IGameDescriptor game)
        {
            m_engine.ResumeGame((game as COMGameDescriptor).GetGameDescriptor());
        }

        public void SuspendGame(IGameDescriptor game)
        {
            m_engine.SuspendGame((game as COMGameDescriptor).GetGameDescriptor());
        }

        public void AnimateTurn(TsCOMLib.TurnCallback callback, Array tileIds, TsCOMLib.Direction Direction, byte distance)
        {
            List<int> tiles = new List<int>();
            foreach (uint i in tileIds)
                tiles.Add((int)i);
            m_view.AnimateTurn(new COMTurnCallback(callback), tiles, Convert.Direction(Direction), distance);
        }

        public void DeleteRemoteGame(TsCOMLib.TurnCallback callback, string gameId)
        {
            m_view.DeleteRemoteGame(new COMTurnCallback(callback), gameId);
        }

        public void DoLocalTurn(TsCOMLib.TurnCallback callback)
        {
            m_view.DoLocalTurn(new COMTurnCallback(callback));
        }

        public void GameOver(TsCOMLib.Player winner)
        {
            m_view.GameOver(new COMPlayer(winner));
        }

        public void GameResumed(TsCOMLib.GameDescriptor game, TsCOMLib.PlayerTeam turn)
        {
            m_view.GameResumed(new COMGameDescriptor(game), Convert.PlayerTeam(turn));
        }

        public void GameSuspended(TsCOMLib.GameDescriptor game)
        {
            m_view.GameSuspended(new COMGameDescriptor(game));
        }

        public string GetAuthToken()
        {
            return m_view.GetAuthToken();
        }

        public void GetRemoteGame(TsCOMLib.TurnCallback callback, string gameId)
        {
            m_view.GetRemoteGame(new COMTurnCallback(callback), gameId);
        }

        public Array GetRemoteGames()
        {
            return m_view.GetRemoteGames().ToArray();
        }

        public void Log(string msg, string file, int line)
        {
            m_view.Log(msg, file, line);
        }

        public string NewRemoteGame(string localId, string remoteId)
        {
            return m_view.NewRemoteGame(localId, remoteId);
        }

        public void PutRemoteGame(TsCOMLib.TurnCallback callback, string gameId, string game)
        {
            m_view.PutRemoteGame(new COMTurnCallback(callback), gameId, game);
        }

        public void StateChanged()
        {
            m_view.StateChanged();
        }

        public void TetradFound(TsCOMLib.TurnCallback callback, TsCOMLib.Player Player, TsCOMLib.TetradShape shape, Array tileIds)
        {
            List<int> tiles = new List<int>();
            foreach( uint i in tileIds )
                tiles.Add((int)i);
            m_view.TetradFound(new COMTurnCallback(callback), new COMPlayer(Player), Convert.TetradShape(shape), tiles );
        }
    }

    [System.Runtime.InteropServices.ComVisible(true)]
    public class COMGameDescriptor : IGameDescriptor
    {
        TsCOMLib.GameDescriptor m_gd;
        IPlayer m_homePlayer;
        IPlayer m_awayPlayer;
        internal COMGameDescriptor(TsCOMLib.GameDescriptor gd)
        {
            m_gd = gd;
            m_homePlayer = new COMPlayer(m_gd.GetHomePlayer());
            m_awayPlayer = new COMPlayer(m_gd.GetAwayPlayer());
        }
        public string FriendlyName { get { return m_gd.FriendlyName; } }
        public string Id { get { return m_gd.Id; } }

        public IPlayer GetAwayPlayer()
        {
            return m_awayPlayer;
        }
        public IPlayer GetHomePlayer()
        {
            return m_homePlayer;
        }
        public IEnumerable<ITile> GetTiles()
        {
            foreach (TsCOMLib.Tile t in m_gd.GetTiles())
                yield return new COMTile(t);
        }
        public PlayerTeam GetTurn()
        {
            return Convert.PlayerTeam(m_gd.GetTurn());
        }
        internal TsCOMLib.GameDescriptor GetGameDescriptor() { return m_gd; }
    }
}
