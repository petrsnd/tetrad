using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace WPF
{
    public class PlayerViewModel : DependencyObject
    {
        public PlayerViewModel(ObjectModel.IPlayer player)
        {
            EnginePlayer = player;
            if (player.Type == ObjectModel.PlayerType.Computer)
                Name = "CPU";
            else
                Name = player.Name;
            TurnName = Name + "'s";
            if (Name == "You")
                TurnName = "Your";
            Team = player.Team;
            UpdateTetrads();
        }

        public void UpdateTetrads()
        {
            TetradLine = EnginePlayer.HasTetrad(ObjectModel.TetradShape.Line) != 0;
            TetradPodium = EnginePlayer.HasTetrad(ObjectModel.TetradShape.Podium) != 0;
            TetradLShape = EnginePlayer.HasTetrad(ObjectModel.TetradShape.LShape) != 0;
            TetradReverseL = EnginePlayer.HasTetrad(ObjectModel.TetradShape.BackwardsL) != 0;
            TetradSquare = EnginePlayer.HasTetrad(ObjectModel.TetradShape.Square) != 0;
            TetradDownStep = EnginePlayer.HasTetrad(ObjectModel.TetradShape.Downstep) != 0;
            TetradUpStep = EnginePlayer.HasTetrad(ObjectModel.TetradShape.Upstep) != 0;
        }

        public ObjectModel.IPlayer EnginePlayer
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

        public void SetTetrad(ObjectModel.TetradShape tetrad)
        {
            switch (tetrad)
            {
                case ObjectModel.TetradShape.BackwardsL:
                    TetradReverseL = true; break;
                case ObjectModel.TetradShape.Downstep:
                    TetradDownStep = true; break;
                case ObjectModel.TetradShape.Line:
                    TetradLine = true; break;
                case ObjectModel.TetradShape.LShape:
                    TetradLShape = true; break;
                case ObjectModel.TetradShape.Podium:
                    TetradPodium = true; break;
                case ObjectModel.TetradShape.Square:
                    TetradSquare = true; break;
                case ObjectModel.TetradShape.Upstep:
                    TetradUpStep = true; break;
            }
        }



        public ObjectModel.PlayerTeam Team
        {
            get { return (ObjectModel.PlayerTeam)GetValue(TeamProperty); }
            set { SetValue(TeamProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Team.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TeamProperty =
            DependencyProperty.Register("Team", typeof(ObjectModel.PlayerTeam), typeof(PlayerViewModel));



        public string Name
        {
            get { return (string)GetValue(NameProperty); }
            set { SetValue(NameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Name.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty NameProperty =
            DependencyProperty.Register("Name", typeof(string), typeof(PlayerViewModel));



        public string TurnName
        {
            get { return (string)GetValue(TurnNameProperty); }
            set { SetValue(TurnNameProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TurnName.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TurnNameProperty =
            DependencyProperty.Register("TurnName", typeof(string), typeof(PlayerViewModel));

        

        public bool TetradSquare
        {
            get { return (bool)GetValue(TetradSquareProperty); }
            set { SetValue(TetradSquareProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradSquare.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradSquareProperty =
            DependencyProperty.Register("TetradSquare", typeof(bool), typeof(PlayerViewModel));

        public bool TetradLine
        {
            get { return (bool)GetValue(TetradLineProperty); }
            set { SetValue(TetradLineProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradLine.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradLineProperty =
            DependencyProperty.Register("TetradLine", typeof(bool), typeof(PlayerViewModel));

        public bool TetradReverseL
        {
            get { return (bool)GetValue(TetradReverseLProperty); }
            set { SetValue(TetradReverseLProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradReverseL.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradReverseLProperty =
            DependencyProperty.Register("TetradReverseL", typeof(bool), typeof(PlayerViewModel));

        public bool TetradLShape
        {
            get { return (bool)GetValue(TetradLShapeProperty); }
            set { SetValue(TetradLShapeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradLShape.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradLShapeProperty =
            DependencyProperty.Register("TetradLShape", typeof(bool), typeof(PlayerViewModel));

        public bool TetradPodium
        {
            get { return (bool)GetValue(TetradPodiumProperty); }
            set { SetValue(TetradPodiumProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradPodium.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradPodiumProperty =
            DependencyProperty.Register("TetradPodium", typeof(bool), typeof(PlayerViewModel));

        public bool TetradUpStep
        {
            get { return (bool)GetValue(TetradUpStepProperty); }
            set { SetValue(TetradUpStepProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradUpStep.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradUpStepProperty =
            DependencyProperty.Register("TetradUpStep", typeof(bool), typeof(PlayerViewModel));

        public bool TetradDownStep
        {
            get { return (bool)GetValue(TetradDownStepProperty); }
            set { SetValue(TetradDownStepProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TetradDownStep.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TetradDownStepProperty =
            DependencyProperty.Register("TetradDownStep", typeof(bool), typeof(PlayerViewModel));


    }
}
