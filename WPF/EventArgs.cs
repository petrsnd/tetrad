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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;


namespace WPF
{
    public class ViewPoppedEventArgs : EventArgs
    {
        public ViewPoppedEventArgs(UserControl view)
        {
            View = view;
        }

        public UserControl View
        {
            get;
            private set;
        }
    }
    public class TilesUpdatedEventArgs : EventArgs
    {
        public TilesUpdatedEventArgs(List<Tile> tiles)
        {
            Tiles = new List<Tile>(tiles);
        }

        public List<Tile> Tiles
        {
            get;
            private set;
        }
    }

    public class LoggedInEventArgs : EventArgs
    {
        public LoggedInEventArgs(TetraSlide.Api.Models.Account account)
        {
            Account = account;
        }

        public TetraSlide.Api.Models.Account Account
        {
            get;
            private set;
        }
    }

    public class OpponentMoveCompleteEventArgs : EventArgs
    {
        public OpponentMoveCompleteEventArgs(IEnumerable<int> tileIds, ObjectModel.Direction direction, byte distance)
        {
            List<int> ids = new List<int>();
            if (tileIds != null)
            {
                foreach (uint id in tileIds)
                    ids.Add((int)id);
            }
            TileIds = ids;
            Direction = direction;
            Distance = distance;
        }

        public byte Distance
        {
            get;
            private set;
        }

        public IEnumerable<int> TileIds
        {
            get;
            private set;
        }

        public ObjectModel.Direction Direction
        {
            get;
            private set;
        }

        public Action Completed
        {
            get;
            private set;
        }
    }

    public class BeginLocalTurnEventArgs : EventArgs
    {

    }

    public class GameStateChangedArgs : EventArgs
    {
        public GameStateChangedArgs()
        {
        }
    }

    public class GameResumedEventArgs : EventArgs
    {
        public GameResumedEventArgs(ObjectModel.IGameDescriptor game, ObjectModel.PlayerTeam turn)
        {
            GameDescriptor = game;
            Turn = turn;
        }

        public ObjectModel.IGameDescriptor GameDescriptor
        {
            get;
            private set;
        }

        public ObjectModel.PlayerTeam Turn
        {
            get;
            private set;
        }
    }
    
    public class GameSuspendedEventArgs : EventArgs
    {
        public GameSuspendedEventArgs(ObjectModel.IGameDescriptor game)
        {
            GameDescriptor = game;
        }

        public ObjectModel.IGameDescriptor GameDescriptor
        {
            get;
            private set;
        }
    }

    public class AuthTokenEventArgs : EventArgs
    {
        public AuthTokenEventArgs(bool success, string context)
        {
            Success = success;
            if (Success)
                AuthToken = context;
            else
                Error = context;
        }
        public bool Success
        {
            get;
            private set;
        }

        public string Error
        {
            get;
            private set;
        }

        public string AuthToken
        {
            get;
            private set;
        }
    }

    public class StateChangedEventArgs : EventArgs
    {
        public StateChangedEventArgs()
        {
        }
    };

    public class TileSelectedEventArgs : EventArgs
    {
        public TileSelectedEventArgs(Tile t, ObjectModel.IPosition p)
        {
            Tile = t;
            Position = p;
        }

        public Tile Tile
        {
            get;
            private set;
        }

        public ObjectModel.IPosition Position
        {
            get;
            private set;
        }
    }


    public class TileMovedEventArgs : EventArgs
    {
        public TileMovedEventArgs(ObjectModel.IPosition from, ObjectModel.IPosition to)
        {
            From = from;
            To = to;
        }

        public ObjectModel.IPosition From   
        {
            get;
            private set;
        }

        public ObjectModel.IPosition To
        {
            get;
            private set;
        }
    }

    public class LocalTurnCompleteEventArgs : EventArgs
    {
        public LocalTurnCompleteEventArgs(ObjectModel.ITile tile, ObjectModel.IPosition pos)
        {
            Tile = tile;
            Position = pos;
        }

        public ObjectModel.ITile Tile
        {
            get;
            private set;
        }

        public ObjectModel.IPosition Position
        {
            get;
            private set;
        }
    }

    public class GameOverEventArgs : EventArgs
    {
        public GameOverEventArgs(ObjectModel.IPlayer winner)
        {
            Winner = winner;
        }

        public ObjectModel.IPlayer Winner
        {
            get;
            private set;
        }
    }

    public class TetradFoundEventArgs : EventArgs
    {
        public TetradFoundEventArgs(ObjectModel.IPlayer player, ObjectModel.TetradShape shape, IEnumerable<int> tileIds)
        {
            Player = player;
            Shape = shape;
            TileIds = tileIds;
        }

        public ObjectModel.IPlayer Player
        {
            get;
            private set;
        }

        public ObjectModel.TetradShape Shape
        {
            get;
            private set;
        }

        public IEnumerable<int> TileIds
        {
            get;
            private set;
        }

        public Action Completed
        {
            get;
            private set;
        }
    }

    public class ColorSelectionEventArgs : EventArgs
    {
        public ColorSelectionEventArgs(Color? color)
        {
            SelectedColor = color;
        }

        public Color? SelectedColor
        {
            get;
            private set;
        }
    }

    public class DifficultySelectionEventArgs : EventArgs
    {
        public DifficultySelectionEventArgs(string difficulty)
        {
            Difficulty = difficulty;
        }

        public string Difficulty
        {
            get;
            private set;
        }
    }

    public class PlayerInfoEventArgs : EventArgs
    {
        public PlayerInfoEventArgs(PlayerInfo info)
        {
            PlayerInfo = info;
        }

        public PlayerInfo PlayerInfo
        {
            get;
            private set;
        }
    }
}