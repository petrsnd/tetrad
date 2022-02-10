using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Collections.ObjectModel;
using System.Timers;

namespace WPF
{
    public class GameViewModel : DependencyObject
    {
        public const int SIZE = 4;
        COMLib.TetradEngine engine = null;
        
        public event EventHandler<GameOverEventArgs> GameOver;

        public GameViewModel(GameView view)
        {
            engine = new COMLib.TetradEngine();
            engine.Init();
            BoardEnabled = true;
            View = view;
        }

        public GameView View
        {
            get;
            set;
        }

        public void SetBoardState()
        {
            uint boardstate = engine.GetBoard(COMLib.PLAYER.ME);
            List<Position> home = new List<Position>();
            List<Position> away = new List<Position>();
            for (int y = 0; y < SIZE; y++)
            {
                for (int x = 0; x < SIZE; x++)
                {
                    Team t = CalculateTeam((uint)x, (uint)y, boardstate);
                    if (t == Team.HOME)
                    {
                        home.Add(new Position(x, y));
                    }
                    else if (t == Team.AWAY)
                    {
                        away.Add(new Position(x, y));
                    }
                }
            }
            View.GameBoard.SetState(home, away);
        }
        Team CalculateTeam(uint x, uint y, uint state)
        {
            Team t = Team.HOME;
            int index = ((int)y * (int)SIZE) + (int)x;
            uint mask = ((uint)1) << index;
            if ((state & (mask << 16)) == 0)
            {
                t = Team.AWAY;
            }
            if ((state & mask) != 0)
            {
                t = Team.NONE;
            }
            return t;
        }
                       
        public PlayerViewModel Player1
        {
            get { return (PlayerViewModel)GetValue(Player1Property); }
            set { SetValue(Player1Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player1.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player1Property =
            DependencyProperty.Register("Player1", typeof(PlayerViewModel), typeof(GameViewModel));

        public PlayerViewModel Player2
        {
            get { return (PlayerViewModel)GetValue(Player2Property); }
            set { SetValue(Player2Property, value); }
        }

        // Using a DependencyProperty as the backing store for Player2.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty Player2Property =
            DependencyProperty.Register("Player2", typeof(PlayerViewModel), typeof(GameViewModel));

        public PlayerViewModel CurrentPlayer
        {
            get { return (PlayerViewModel)GetValue(CurrentPlayerProperty); }
            set { SetValue(CurrentPlayerProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentPlayer.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentPlayerProperty =
            DependencyProperty.Register("CurrentPlayer", typeof(PlayerViewModel), typeof(GameViewModel));

        public bool BoardEnabled
        {
            get { return (bool)GetValue(BoardEnabledProperty); }
            set { SetValue(BoardEnabledProperty, value); }
        }

        // Using a DependencyProperty as the backing store for BoardEnabled.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BoardEnabledProperty =
            DependencyProperty.Register("BoardEnabled", typeof(bool), typeof(GameViewModel));

        

        public void SetTargets(Position tile)
        {
            List<Position> targets = new List<Position>();
            Array arr = engine.GetTargets(tile.ToEnginePosition());
            if (arr != null)
            {
                foreach (COMLib.Position pos in arr)
                {
                    targets.Add(new Position(pos));
                }
                View.GameBoard.SetTargets(targets);
            }
        }

        public void MoveTile(Board board, Position from, Position to)
        {
            COMLib.MoveResult moveResult = engine.Move(from.ToEnginePosition(), to.ToEnginePosition());

            // 2. Get results from the game engine
            MoveResults result = TranslateResults(moveResult);
            if (result == null)
            {
                MessageBox.Show(string.Format("Invalid move. Returned null result from {0},{1} to {2},{3}\r\n\r\nPossibly you have reversed the last move.  That's not allowed.",
                    from.X, from.Y, to.X, to.Y));
                return;
            }
            Player1.SetTetrad(moveResult.MyNewTetrad);   
            Player2.SetTetrad(moveResult.TheirNewTetrad);
            if (CurrentPlayer is ComputerPlayerViewModel)
            {
                board.PerformMoves(result.Moves, new Action(delegate
                {
                    PostMoveActions();
                }));
            }
            else
            {
                PostMoveActions();
                
                Action waitForOpponent = new Action(delegate
                {
                    var res = engine.GetOpponentMove();
                    Dispatcher.BeginInvoke(new Action(delegate
                    {
                        MoveResults cpuMove = TranslateResults(res);
                        Player1.SetTetrad(res.MyNewTetrad);
                        Player2.SetTetrad(res.TheirNewTetrad);
                        View.GameBoard.PerformMoves(cpuMove.Moves, new Action(delegate
                        {
                            PostMoveActions();
                        }));
                    }));
                });

                BoardEnabled = false;
                waitForOpponent.BeginInvoke(null, null);
            }
        }

        void cpuTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            
        }


        void PostMoveActions()
        {
            if (CurrentPlayer == Player1)
                CurrentPlayer = Player2;
            else
                CurrentPlayer = Player1;
            SetBoardState();
            PlayerViewModel winner = null;
            if (Player1.HasAllTetrads())
            {
                winner = Player1;
            }
            else if (Player2.HasAllTetrads())
            {
                winner = Player2;
            }

            if (winner != null)
            {
                this.Dispatcher.Invoke(new Action(delegate()
                {
                    if (GameOver != null)
                    {
                        GameOver(this, new GameOverEventArgs(winner));
                    }
                }), null);
            }
            BoardEnabled = true;
        }


        MoveResults TranslateResults(COMLib.MoveResult result)
        {
            if (result == null) return null;
            List<Move> moves = new List<Move>();
            Direction dir = TranslateToDirection(result.Direction);
            Array positions = result.GetTilePositions();
            if (positions != null)
            {
                foreach (COMLib.Position pos in positions)
                {
                    Position from = new Position(pos);
                    moves.Add(new Move(dir, result.Distance, from,
                        GetOffsetTile(from, dir, result.Distance)));
                }
            }
            return new MoveResults( true, moves );
        }

        Position GetOffsetTile(Position tile, Direction dir, byte distance)
        {
            switch (dir)
            {
                case Direction.UP:
                    return new Position(tile.X, tile.Y - distance);
                case Direction.DOWN:
                    return new Position(tile.X, tile.Y + distance);
                case Direction.LEFT:
                    return new Position(tile.X - distance, tile.Y);
                case Direction.RIGHT:
                    return new Position(tile.X + distance, tile.Y);
            }
            throw new System.Exception("WTF!?");
        }

        Direction TranslateToDirection(COMLib.BOARD_DIRECTION dir)
        {
            switch (dir)
            {
                case COMLib.BOARD_DIRECTION.DIR_DOWN:
                    return Direction.DOWN;
                case COMLib.BOARD_DIRECTION.DIR_UP:
                    return Direction.UP; 
                case COMLib.BOARD_DIRECTION.DIR_RIGHT:
                    return Direction.RIGHT; 
                case COMLib.BOARD_DIRECTION.DIR_LEFT:
                    return Direction.LEFT; 
            }
            throw new System.Exception("WTF?");
        }

        COMLib.PLAYER TranslatePlayer(PlayerViewModel player)
        {
            if (player.Equals( Player1 ) )
                return COMLib.PLAYER.ME;
            return COMLib.PLAYER.THEM;
        }
    }

    class TileInfo
    {
        public TileInfo() { }
        public Position Position { get; set; }
        public Team Team { get; set; }
    }
}
