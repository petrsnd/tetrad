using System;
using System.Collections.Generic;

namespace TetraSlide.Win8
{

    public class GameResumedEventArgs : EventArgs
    {
        public GameResumedEventArgs(TsWinRT.TsGameDescriptor gd)
        {
            GameDescriptor = gd;
        }
        public TsWinRT.TsGameDescriptor GameDescriptor
        {
            get;
            private set;
        }
    };

    public class TileMovedEventArgs : EventArgs
    {
        public TileMovedEventArgs(ViewModel.Tile source, TsWinRT.TsPosition target)
        {
            Tile = source;
            Target = target;
        }

        public ViewModel.Tile Tile
        {
            get;
            private set;
        }

        public TsWinRT.TsPosition Target
        {
            get;
            private set;
        }
    };

    public class GameSuspendedEventArgs : EventArgs
    {
        public GameSuspendedEventArgs(TsWinRT.TsGameDescriptor gd)
        {
            GameDescriptor = gd;
        }

        public TsWinRT.TsGameDescriptor GameDescriptor
        {
            get;
            private set;
        }
    };

    public class GameOverEventArgs : EventArgs
    {
        public GameOverEventArgs(TsWinRT.ITsPlayer winner)
        {
            Winner = winner;
        }
        public TsWinRT.ITsPlayer Winner
        {
            get;
            private set;
        }
    };

    public class StateChangedEventArgs : EventArgs
    {
        public StateChangedEventArgs()
        {
        }
    };

    public class TetradFoundEventArgs : EventArgs
    {
        public TetradFoundEventArgs(TsWinRT.ITsPlayer player, TsWinRT.TsTetradShape shape, IList<TsWinRT.TsTile> tiles)
        {
            Player = player;
            Shape = shape;
            Tiles = tiles;
        }

        public TsWinRT.ITsPlayer Player
        {
            get;
            private set;
        }

        public TsWinRT.TsTetradShape Shape
        {
            get;
            private set;
        }

        public IList<TsWinRT.TsTile> Tiles
        {
            get;
            private set;
        }
    };

    public class DoLocalTurnEventArgs : EventArgs
    {
        public DoLocalTurnEventArgs(TsWinRT.TsTurnCallback callback, TsWinRT.TsPlayerTeam turn)
        {
            Callback = callback;
            Turn = turn;
        }
        public TsWinRT.TsTurnCallback Callback
        {
            get;
            private set;
        }
        public TsWinRT.TsPlayerTeam Turn
        {
            get;
            private set;
        }
    };

    public class AnimateTurnEventArgs : EventArgs
    {
        public AnimateTurnEventArgs(TsWinRT.TsTurnCallback callback, IList<TsWinRT.TsTile> tiles, TsWinRT.TsBoardDirection dir, int dist)
        {
            Callback = callback;
            Tiles = tiles;
            Direction = dir;
            Distance = dist;
        }

        public TsWinRT.TsTurnCallback Callback
        {
            get;
            private set;
        }

        public IList<TsWinRT.TsTile> Tiles
        {
            get;
            private set;
        }
        public TsWinRT.TsBoardDirection Direction
        {
            get;
            private set;
        }
        public int Distance
        {
            get;
            private set;
        }

    };

}