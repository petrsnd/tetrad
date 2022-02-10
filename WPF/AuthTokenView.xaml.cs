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

namespace WPF
{
    /// <summary>
    /// Interaction logic for AuthTokenView.xaml
    /// </summary>
    public partial class AuthTokenView : UserControl
    {
        public event EventHandler<AuthTokenEventArgs> Completed;

        public AuthTokenView()
        {
            InitializeComponent();
            Loaded += new RoutedEventHandler(AuthTokenView_Loaded);
            //Util.WebBrowserHelper.ClearCache();
        }

        void AuthTokenView_Loaded(object sender, RoutedEventArgs e)
        {
            browser.Navigated += new NavigatedEventHandler(browser_Navigated);
            browser.Navigate("https://services.implbits.com/tetraslide/v1/login/");
        }

        void browser_Navigated(object sender, NavigationEventArgs e)
        {
            string navuri = e.Uri.ToString();
            mshtml.HTMLDocument dom = (mshtml.HTMLDocument)browser.Document;
            if (dom.title.StartsWith("success code="))
            {
                string code = dom.title.Split('=')[1];
                Properties.Settings.Default.AuthToken = code;
                Properties.Settings.Default.Save();
                FireCompleted(new AuthTokenEventArgs(true, code));
            }
            else if( dom.title.StartsWith("error message="))
            {
                string error = dom.title.Split('=')[1];
                FireCompleted(new AuthTokenEventArgs(false, error));
            }
        }

        void FireCompleted(AuthTokenEventArgs args)
        {
            if (Completed != null)
                Completed(this, args);
        }

        private void DoRefresh(object sender, RoutedEventArgs e)
        {
            browser.Refresh(true);
        }
    }
}
