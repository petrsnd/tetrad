using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;

namespace WPF
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static string[] Args = null;

        public App()
        {
            Logger.Log("Application initialized.");
        }

        public static App Mine
        {
            get
            {
                return App.Current as App;
            }
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            if (e.Args.Length > 0)
            {
                Args = e.Args;
            }
        }

        public void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            GameManager = GameManager.Instance();
            GameManager.InitGameModel(this.MyMainWindow);
            Navigator = new Navigator();
            
            if (Args != null && Args.Length > 0)
                Mine.Model.AuthToken = Args[0];

            
            Navigator.PushCurrentView(new MainMenu());
            MainWindow.DataContext = Navigator;
            MyMainWindow.CenterWindowOnScreen();
            SoundManager.Instance().PlayMenuMusic();
        }

        public Main MyMainWindow
        {
            get
            {
                return this.MainWindow as Main;
            }
        }

        public Model Model
        {
            get { return GameManager.Model; }
            
        }

        public GameManager GameManager
        {
            get;
            private set;
        }

        public Navigator Navigator
        {
            get;
            set;
        }
    }
}
