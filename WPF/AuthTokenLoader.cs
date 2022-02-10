using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.ComponentModel;


namespace WPF
{
    class AuthTokenLoader
    {
        AutoResetEvent loadEvent = new AutoResetEvent(false);

        public void Load(System.Windows.Threading.Dispatcher uiThread)
        {
           
            uiThread.BeginInvoke(new Action(() =>
                {
                    AuthTokenView atv = new AuthTokenView();
                    atv.Completed += new EventHandler<AuthTokenEventArgs>(atv_Completed);
                    App.Mine.Navigator.PushCurrentView(atv);
                }));
            loadEvent.WaitOne();
        }

        void atv_Completed(object sender, AuthTokenEventArgs e)
        {
            Success = e.Success;
            AuthToken = e.AuthToken;
            Error = e.Error;
            loadEvent.Set();
        }

        public bool Success
        {
            get;
            private set;
        }

        public string AuthToken
        {
            get;
            private set;
        }

        public string Error
        {
            get;
            private set;
        }
    }
}
