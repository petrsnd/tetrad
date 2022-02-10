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
using System.Collections.ObjectModel;
using TetraSlide.Api.Models;

namespace WPF
{
    /// <summary>
    /// Interaction logic for InvitePlayerView.xaml
    /// </summary>
    public partial class InvitePlayerView : UserControl
    {
        Navigator nav;
        public InvitePlayerView()
        {
            InitializeComponent();
            nav = App.Mine.Navigator;
            this.DataContext = this;
            this.IsVisibleChanged += new DependencyPropertyChangedEventHandler(InvitePlayerView_IsVisibleChanged);
        }

        void InvitePlayerView_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if ((bool)e.NewValue == true)
            {
                InitButtonLists();
            }
        }

        async void InitButtonLists()
        {
            MyTurnStatus = "Loading...";
            TheirTurnStatus = "Loading...";
            MyTurnStatusVisibility = Visibility.Visible;
            TheirTurnStatusVisibility = Visibility.Visible;
            var myTurnButtons = new ObservableCollection<Control>();
            var theirTurnButtons = new ObservableCollection<Control>();
            var games = await App.Mine.Model.GetGamesAsync();
            var myGames = new List<ObjectModel.IGameDescriptor>();
            var theirGames = new List<ObjectModel.IGameDescriptor>();
            foreach (var g in games)
            {
                if (g.GetTurn() == ObjectModel.PlayerTeam.Home)
                    myGames.Add(g);
                else
                    theirGames.Add(g);
            }
            
            InitGameList(myGames, myTurnButtons);
            InitGameList(theirGames, theirTurnButtons);

            MyTurnList.ItemsSource = myTurnButtons;
            TheirTurnList.ItemsSource = theirTurnButtons;

            if (myTurnButtons.Count == 0)
                MyTurnStatus = "No games found!";
            else
                MyTurnStatusVisibility = Visibility.Collapsed;
                
            if (theirTurnButtons.Count == 0)
                TheirTurnStatus = "No games found!";
            else
                TheirTurnStatusVisibility = Visibility.Collapsed;              
        }

        void InitGameList(IEnumerable<ObjectModel.IGameDescriptor> games, ObservableCollection<Control> gameList)
        {
            foreach (ObjectModel.IGameDescriptor gd in games)
            {
                Button btn = new Button()
                {
                    HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch,
                    Content = new OutlinedText() { 
                        Text = gd.FriendlyName, 
                        FontSize = 13, 
                        StrokeThickness = 1, 
                        Stroke = new SolidColorBrush(Colors.Black) },
                    DataContext = gd
                };
                btn.Click += new RoutedEventHandler(btn_Click);
                gameList.Add(btn);
            }
        }

        void btn_Click(object sender, RoutedEventArgs e)
        {
            Button btn = e.OriginalSource as Button;
            if (btn == null) return;
            ObjectModel.IGameDescriptor gd = btn.DataContext as ObjectModel.IGameDescriptor;
            LaunchGame(gd);
        }

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            nav.PushCurrentView(new MessageView() { Message = "Creating the game... hang on a sec" });
            try
            {
                var p1 = ObjectModel.Factory.NewPlayer();
                p1.Type = ObjectModel.PlayerType.Local;
                p1.Identity = Account.email_address;
                p1.Name = Account.name;
                p1.RgbColor = 0x00000000;
                p1.Team = ObjectModel.PlayerTeam.Home;
               
                var p2 = ObjectModel.Factory.NewPlayer();
                p2.Type = ObjectModel.PlayerType.Remote;
                p2.Identity = Email;
                p2.Name = Email;
                p2.RgbColor = 0x00000000;
                p2.Team = ObjectModel.PlayerTeam.Away;
                
                ObjectModel.IGameDescriptor gd = await App.Mine.Model.CreatePvpGameAsync(p1, p2);
                if( gd != null )
                    LaunchGame(gd);
            }
            catch (Exception ex)
            {
                string msg = "Failed to create the game:\n" + ex.Message;
                nav.PushCurrentView(new MessageView() { Message = msg });
            }
        }

        void LaunchGame(ObjectModel.IGameDescriptor gd)
        {
            if( Account.name != null )
                gd.GetHomePlayer().Name = Account.name;
            if (gd.GetHomePlayer().RgbColor == 0)
            {
                ColorSelection cs = new ColorSelection();
                cs.DisableColor(Util.UintToColor(gd.GetAwayPlayer().RgbColor)); 
                cs.Dismissed += (obj, arg) =>
                {
                    App.Mine.Navigator.PopCurrentView();
                    App.Mine.Navigator.PushCurrentView(new MessageView() { Message = "Loading game..." });
                    if (arg.SelectedColor.HasValue)
                    {
                        gd.GetHomePlayer().RgbColor = Util.ToRgbColor(arg.SelectedColor.Value);
                        App.Mine.Model.ResumeGame(gd);
                    }
                    
                };
                App.Mine.Navigator.PushCurrentView(cs);
            }
            else
            {
            	App.Mine.Navigator.PopCurrentView();
                App.Mine.Navigator.PushCurrentView(new MessageView() { Message = "Loading game..." });
                App.Mine.Model.ResumeGame(gd);
            }
        }

        public TetraSlide.Api.Models.Account Account
        {
            get;
            set;
        }


        public string MyTurnStatus
        {
            get { return (string)GetValue(MyTurnStatusProperty); }
            set { SetValue(MyTurnStatusProperty, value); }
        }

        // Using a DependencyProperty as the backing store for MyTurnStatus.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MyTurnStatusProperty =
            DependencyProperty.Register("MyTurnStatus", typeof(string), typeof(InvitePlayerView));



        public string TheirTurnStatus
        {
            get { return (string)GetValue(TheirTurnStatusProperty); }
            set { SetValue(TheirTurnStatusProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TheirTurnStatus.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TheirTurnStatusProperty =
            DependencyProperty.Register("TheirTurnStatus", typeof(string), typeof(InvitePlayerView));



        public Visibility MyTurnStatusVisibility
        {
            get { return (Visibility)GetValue(MyTurnStatusVisibilityProperty); }
            set { SetValue(MyTurnStatusVisibilityProperty, value); }
        }

        // Using a DependencyProperty as the backing store for MyTurnStatusVisibility.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MyTurnStatusVisibilityProperty =
            DependencyProperty.Register("MyTurnStatusVisibility", typeof(Visibility), typeof(InvitePlayerView));

        public Visibility TheirTurnStatusVisibility
        {
            get { return (Visibility)GetValue(TheirTurnStatusVisibilityProperty); }
            set { SetValue(TheirTurnStatusVisibilityProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TheirTurnStatusVisibility.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TheirTurnStatusVisibilityProperty =
            DependencyProperty.Register("TheirTurnStatusVisibility", typeof(Visibility), typeof(InvitePlayerView));




        public string Email
        {
            get { return (string)GetValue(EmailProperty); }
            set { SetValue(EmailProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Email.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EmailProperty =
            DependencyProperty.Register("Email", typeof(string), typeof(InvitePlayerView));
    }
}
