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
using System.Windows.Shapes;
using System.ComponentModel;

namespace WPF
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : UserControl
    {
        App app = null;

        public MainMenu()
        {
            app = App.Mine;
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void VsComputerClicked(object sender, RoutedEventArgs e)
        {
            DifficultySelection ds = new DifficultySelection();
            ds.Dismissed += (obj2, difficultyArgs) =>
            {
                ColorSelection cs = new ColorSelection();
                cs.Dismissed += (obj, colorArgs) =>
                {
                    app.Navigator.SetCurrentView(new MessageView() { Message = "Creating the game..." });
                    var player = ObjectModel.Factory.NewPlayer();
                    player.Type = ObjectModel.PlayerType.Local;
                    player.Identity = "You";
                    player.Name = "You";
                    player.RgbColor = Util.ToRgbColor(colorArgs.SelectedColor.Value);
                    player.Team = ObjectModel.PlayerTeam.Home;
                    app.Model.CreateGameVsComputer(player, difficultyArgs.Difficulty);
                };
                app.Navigator.PushCurrentView(cs);
            };
            app.Navigator.PushCurrentView(ds);
        }

        

        private void PvPClicked(object sender, RoutedEventArgs e)
        {
            App.Mine.Navigator.SetCurrentView(new MessageView() { Message = "Loading account information... please wait" });
            BackgroundWorker gameCreator = new BackgroundWorker();
            TetraSlide.Api.Models.Account account = null;
            gameCreator.DoWork += new DoWorkEventHandler((s, args) =>
            {
                account = App.Mine.Model.GetMyAccount();
            });
            gameCreator.RunWorkerCompleted +=new RunWorkerCompletedEventHandler( (s, args) =>
            {
                if( account == null )
                {
                    App.Mine.Navigator.SetCurrentView( new MessageView(){Message = "Failed to load your account settings."} );
                    return;
                }
                InvitePlayerView ipv = new InvitePlayerView();
                ipv.Account = account;
                App.Mine.Navigator.PushCurrentView(ipv);
                });
            gameCreator.RunWorkerAsync();
        }

        private void OnPassAndPlay(object sender, RoutedEventArgs e)
        {
            ColorSelection p1Color = new ColorSelection();
            p1Color.Caption = "Player 1, select your color:";
            p1Color.Dismissed +=  (obj, colorArgs) =>
            {
                if (colorArgs.SelectedColor == null)
                {
                    app.Navigator.PopCurrentView();
                    return;
                }
                else
                {
                    ColorSelection p2Color = new ColorSelection();
                    p2Color.Caption = "Player 2, select your color:";
                    p2Color.DisableColor(colorArgs.SelectedColor.Value);
                    p2Color.Dismissed += async (obj2, colorArgs2) =>
                    {
                        if (colorArgs2.SelectedColor == null)
                        {
                            app.Navigator.PopCurrentView();
                            return;
                        }
                        else
                        {
                            app.Navigator.SetCurrentView(new MessageView() { Message = "Creating the game..." });
                            var p1 = ObjectModel.Factory.NewPlayer();
                            p1.Type = ObjectModel.PlayerType.Local;
                            p1.Identity = "Player 1";
                            p1.Name = p1.Identity;
                            p1.RgbColor = Util.ToRgbColor(colorArgs.SelectedColor.Value);
                            p1.Team = ObjectModel.PlayerTeam.Home;

                            var p2 = ObjectModel.Factory.NewPlayer();
                            p2.Type = ObjectModel.PlayerType.Local;
                            p2.Identity = "Player 2";
                            p2.Name = p2.Identity;
                            p2.RgbColor = Util.ToRgbColor(colorArgs2.SelectedColor.Value);
                            p2.Team = ObjectModel.PlayerTeam.Away;
                                
                            ObjectModel.IGameDescriptor gd = await app.Model.CreatePvpGameAsync(p1, p2);
                            if (gd == null)
                            {
                                app.Navigator.PopCurrentView();
                                return;
                            }
                            App.Mine.Model.ResumeGame(gd);
                        }
                    };
                    app.Navigator.PushCurrentView(p2Color);
                }
            };
            app.Navigator.PushCurrentView(p1Color);
        }
    }


 
}
