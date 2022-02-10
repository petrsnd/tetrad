using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media;

namespace TetraSlide.Win8.ViewModel
{

    public class Player : DependencyObject
    {
        public Player(TsWinRT.ITsPlayer player)
        {
            EnginePlayer = player;
            if (player.Type == TsWinRT.TsPlayerType.TsPlayerComputer)
                Name = "CPU";
            else
                Name = player.Name;
            TurnName = Name + "'s";
            if (Name == "You")
                TurnName = "Your";
            Team = player.Team;
            UpdateTetrads();
            TeamBrush = new SolidColorBrush(Util.Color.IntToColor(player.Color));
        }

        public void UpdateTetrads()
        {
            TetradLine = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradLine);
            TetradPodium = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradPodium);
            TetradLShape = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradLShape);
            TetradReverseL = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradBackwardsL);
            TetradSquare = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradSquare);
            TetradDownStep = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TsTetradDownStep);
            TetradUpStep = EnginePlayer.HasTetrad(TsWinRT.TsTetradShape.TSTetradUpStep);
        }

        public TsWinRT.ITsPlayer EnginePlayer
        {
            get;
            private set;
        }

        public bool HasAllTetrads()
        {
            return TetradLine &&
                TetradPodium &&
                TetradLShape &&
                TetradReverseL &&
                TetradSquare &&
                TetradDownStep &&
                TetradUpStep;
        }

        public void SetTetrad(TsWinRT.TsTetradShape tetrad)
        {
            switch (tetrad)
            {
                case TsWinRT.TsTetradShape.TsTetradBackwardsL:
                    TetradReverseL = true; break;
                case TsWinRT.TsTetradShape.TsTetradDownStep:
                    TetradDownStep = true; break;
                case TsWinRT.TsTetradShape.TsTetradLine:
                    TetradLine = true; break;
                case TsWinRT.TsTetradShape.TsTetradLShape:
                    TetradLShape = true; break;
                case TsWinRT.TsTetradShape.TsTetradPodium:
                    TetradPodium = true; break;
                case TsWinRT.TsTetradShape.TsTetradSquare:
                    TetradSquare = true; break;
                case TsWinRT.TsTetradShape.TSTetradUpStep:
                    TetradUpStep = true; break;
            }
        }

        public TsWinRT.TsPlayerTeam Team
        {
            get { return (TsWinRT.TsPlayerTeam)GetValue(TeamProperty); }
            set { SetValue(TeamProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Team.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TeamProperty =
            DependencyProperty.Register("Team", typeof(TsWinRT.TsPlayerTeam), typeof(Player), null);

        public string Name
        {
            get { return (string)GetValue(NameProperty); }
            set { SetValue(NameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Name.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty NameProperty =
            DependencyProperty.Register("Name", typeof(string), typeof(Player), null);

        public string TurnName
        {
            get { return (string)GetValue(TurnNameProperty); }
            set { SetValue(TurnNameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TurnName.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TurnNameProperty =
            DependencyProperty.Register("TurnName", typeof(string), typeof(Player), null);

        public bool TetradSquare
        {
            get { return (bool)GetValue(TetradSquareProperty); }
            set { SetValue(TetradSquareProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradSquare.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradSquareProperty =
            DependencyProperty.Register("TetradSquare", typeof(bool), typeof(Player), null);

        public bool TetradLine
        {
            get { return (bool)GetValue(TetradLineProperty); }
            set { SetValue(TetradLineProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradLine.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradLineProperty =
            DependencyProperty.Register("TetradLine", typeof(bool), typeof(Player), null);

        public bool TetradReverseL
        {
            get { return (bool)GetValue(TetradReverseLProperty); }
            set { SetValue(TetradReverseLProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradReverseL.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradReverseLProperty =
            DependencyProperty.Register("TetradReverseL", typeof(bool), typeof(Player), null);

        public bool TetradLShape
        {
            get { return (bool)GetValue(TetradLShapeProperty); }
            set { SetValue(TetradLShapeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradLShape.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradLShapeProperty =
            DependencyProperty.Register("TetradLShape", typeof(bool), typeof(Player), null);

        public bool TetradPodium
        {
            get { return (bool)GetValue(TetradPodiumProperty); }
            set { SetValue(TetradPodiumProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradPodium.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradPodiumProperty =
            DependencyProperty.Register("TetradPodium", typeof(bool), typeof(Player), null);

        public bool TetradUpStep
        {
            get { return (bool)GetValue(TetradUpStepProperty); }
            set { SetValue(TetradUpStepProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradUpStep.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradUpStepProperty =
            DependencyProperty.Register("TetradUpStep", typeof(bool), typeof(Player), null);

        public bool TetradDownStep
        {
            get { return (bool)GetValue(TetradDownStepProperty); }
            set { SetValue(TetradDownStepProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradDownStep.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradDownStepProperty =
            DependencyProperty.Register("TetradDownStep", typeof(bool), typeof(Player), null);

        public Brush TeamBrush
        {
            get { return (Brush)GetValue(TeamBrushProperty); }
            set { SetValue(TeamBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TeamBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TeamBrushProperty =
            DependencyProperty.Register("TeamBrush", typeof(Brush), typeof(Player), null);
    }
}
