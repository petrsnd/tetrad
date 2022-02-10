using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.UI;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;

namespace TetraSlide.Win8.ViewModel
{
    enum GameType
    {
        Solo,
        PvP,
        PassNPlay
    }

    enum GameDifficulty
    {
        Beginner,
        Easy,
        Intermediate,
        Hard,
        Expert
    };


    class CreateGame : DependencyObject
    {
        public CreateGame()
        {
            PlaySoloCommand = new RelayCommand(
                (param) =>
                {
                    var vm = new CreateGame();
                    vm.GameType = ViewModel.GameType.Solo;
                    vm.Commands = vm.DifficultyMenu.Commands;
                    App.RootFrame.Navigate(typeof(TsPage), new PageContent(vm,
                        new View.MainMenu(),
                        new View.SnappedCopoutView()));
                });

            PlayPassNPlayCommand = new RelayCommand(
                (param) =>
                {
                    var vm = new CreateGame();
                    vm.GameType = ViewModel.GameType.PassNPlay;
                    App.RootFrame.Navigate(typeof(TsPage), new PageContent(vm,
                        new View.CreateGame(),
                        new View.SnappedCopoutView()));
                });

            SetDifficultyCommand = new RelayCommand(
                (param) =>
                {
                    GameDifficulty = DifficultyFromString(param as string);
                    App.RootFrame.Navigate(typeof(TsPage), new PageContent(this,
                        new View.CreateGame(),
                        new View.SnappedCopoutView()));
                });

            Lurn2PlayCommand = new RelayCommand(
                (param) =>
                {
                    App.RootFrame.Navigate(typeof(TsPage), new PageContent(this,
                        new View.Tutorial(),
                        new View.SnappedCopoutView()));
                });

            DifficultyMenu = new Menu();
            DifficultyMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Beginner",
                Subtitle = "Play a game against a computer opponent",
                Command = SetDifficultyCommand,
                CommandParameter = "Beginner"
            });
            DifficultyMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Easy",
                Subtitle = "Play a game against a computer opponent",
                Command = SetDifficultyCommand,
                CommandParameter = "Easy"
            });
            DifficultyMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Intermediate",
                Subtitle = "Play a game against a friend that is sitting next to you",
                Command = SetDifficultyCommand,
                CommandParameter = "Intermediate"
            });
            DifficultyMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Hard",
                Subtitle = "Play a game against a computer opponent",
                Command = SetDifficultyCommand,
                CommandParameter = "Hard"
            });
            DifficultyMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Expert",
                Subtitle = "Play a game against an online opponent",
                Command = SetDifficultyCommand,
                CommandParameter = "Expert"
            });

            GameTypeMenu = new ViewModel.Menu();
            GameTypeMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Play Solo",
                Subtitle = "Play a game against a computer opponent",
                Command = PlaySoloCommand
            });
            GameTypeMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Pass 'n Play",
                Subtitle = "Play a game against a friend that is sitting next to you",
                Command = PlayPassNPlayCommand
            });
            /*
            GameTypeMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Play Online",
                Subtitle = "Play a game against an online opponent",
                Command = new RelayCommand(
                    async x => {
                        MessageDialog dlg = new MessageDialog("This mode is not available in the beta.");
                        await dlg.ShowAsync();
                    })
            });
            */
            GameTypeMenu.Commands.Add(new ViewModel.MenuCommandItem()
            {
                Title = "Tutorial",
                Subtitle = "Learn how to play",
                Command = Lurn2PlayCommand
            });
            Commands = GameTypeMenu.Commands;
        }

        public ICommand Lurn2PlayCommand
        {
            get;
            private set;
        }

        public ICommand PlaySoloCommand
        {
            get;
            private set;
        }

        public ICommand PlayPassNPlayCommand
        {
            get;
            private set;
        }

        public ICommand SetDifficultyCommand
        {
            get;
            private set;
        }

        public GameDifficulty GameDifficulty
        {
            get;
            set;
        }

        public GameType GameType
        {
            get;
            private set;
        }

        public string Prompt
        {
            get
            {
                switch (GameType)
                {
                    case GameType.Solo:
                        return "Select your color:";
                    case GameType.PassNPlay:
                        return (HomeBrush != null ?
                            "Player 2, select your color:" :
                            "Player 1, select your color");
                    case GameType.PvP:
                        return "Hmm, what goes here!?";
                }
                return "Huh?";
            }
        }

        public Brush HomeBrush
        {
            get { return (Brush)GetValue(HomeColorProperty); }
            set { SetValue(HomeColorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HomeBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HomeColorProperty =
            DependencyProperty.Register("HomeBrush", typeof(Brush), typeof(CreateGame), null);

        public Brush AwayBrush
        {
            get { return (Brush)GetValue(AwayColorProperty); }
            set { SetValue(AwayColorProperty, value); }
        }

        // Using a DependencyProperty as the backing store for AwayBrush.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty AwayColorProperty =
            DependencyProperty.Register("AwayBrush", typeof(Brush), typeof(CreateGame), null);

        public ViewModel.Menu DifficultyMenu
        {
            get;
            private set;
        }

        public ViewModel.Menu GameTypeMenu
        {
            get; 
            private set;
        }
        
        private ICommand m_colorClicked;
        public ICommand ColorClicked
        {
            get
            {
                if (m_colorClicked == null)
                    m_colorClicked = new RelayCommand(param =>
                        {
                            if (GameType == ViewModel.GameType.Solo)
                            {
                                this.HomeBrush = (param as Button).Background;
                                App.Mine.Game = new Game(this);
                            }
                            else if (GameType == ViewModel.GameType.PassNPlay)
                            {
                                Button selected = (param as Button);
                                if (HomeBrush == null)
                                {
                                    CreateGame copy = new CreateGame() { GameType = this.GameType };
                                    copy.HomeBrush = selected.Background;
                                    App.RootFrame.Navigate(typeof(TsPage), new PageContent( copy,
                                        new View.CreateGame(),
                                        new View.CreateGameSnapped()));
                                }
                                else
                                {
                                    this.AwayBrush = (param as Button).Background;
                                    App.Mine.Game = new Game(this);
                                }
                            }
                        },
                        param =>
                        {
                            if (HomeBrush == null || param == null)
                            {
                                return true;
                            }
                            Button b = param as Button;
                            if (b == null) return false;
                            return HomeBrush != b.Background;
                        });
                return m_colorClicked;
            }
        }

        public ObservableCollection<MenuCommandItem> Commands
        {
            get { return (ObservableCollection<MenuCommandItem>)GetValue(CommandsProperty); }
            set { SetValue(CommandsProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Commands.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CommandsProperty =
            DependencyProperty.Register("Commands", typeof(ObservableCollection<MenuCommandItem>), typeof(CreateGame), null);

        private GameDifficulty DifficultyFromString(string str)
        {
            if (str == null)
                return GameDifficulty.Beginner;
            if (str.Equals("Beginner", StringComparison.OrdinalIgnoreCase))
                return GameDifficulty.Beginner;
            if (str.Equals("Easy", StringComparison.OrdinalIgnoreCase))
                return GameDifficulty.Easy;
            if (str.Equals("Intermediate", StringComparison.OrdinalIgnoreCase))
                return GameDifficulty.Intermediate;
            if (str.Equals("Hard", StringComparison.OrdinalIgnoreCase))
                return GameDifficulty.Hard;
            if (str.Equals("Expert", StringComparison.OrdinalIgnoreCase))
                return GameDifficulty.Expert;
            return GameDifficulty.Beginner;
        }

        public string GetDifficultyAsString()
        {
            return GameDifficulty.ToString();
        }

        public int GetHomeColor()
        {
            return BrushToColor(HomeBrush);
        }

        public int GetAwayColor()
        {
            return BrushToColor(AwayBrush);
        }

        private int BrushToColor(Brush brush)
        {
            Color? c = null;
            if (brush is LinearGradientBrush)
            {
                c = (brush as LinearGradientBrush).GradientStops.First().Color;
            }
            if (c.HasValue)
                return Util.Color.ColorToInt(c.Value);
            return unchecked((int)0xFFFFFFFF);
        }

    }
}
