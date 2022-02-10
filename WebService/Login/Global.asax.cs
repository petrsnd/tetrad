using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.SessionState;
using System.Configuration;
using System.Collections.Specialized;

namespace TetraSlide.Login
{
    public class Global : System.Web.HttpApplication
    {
        public static ILoginProvider GoogleLoginProvider;
        public static ILoginProvider FacebookLoginProvider;
        public static ILoginProvider WindowsLiveLoginProvider;
        public static TetraSlideAccountClient AccountClient; 

        protected void Application_Start(object sender, EventArgs e)
        {
            NameValueCollection appSettings = ConfigurationManager.AppSettings;
            
            GoogleLoginProvider = new LoginProviderGoogle(
                appSettings["GoogleAppId"],
                appSettings["GoogleAppSecret"],
                appSettings["GoogleRedirectUrl"]);

            FacebookLoginProvider = new LoginProviderFacebook(
                appSettings["FacebookAppId"],
                appSettings["FacebookAppSecret"],
                appSettings["FacebookRedirectUrl"]);
            
            WindowsLiveLoginProvider = new WindowsLoginProviderLive(
                appSettings["WindowsLiveAppId"],
                appSettings["WindowsLiveAppSecret"],
                appSettings["WindowsLiveRedirectUrl"]);

            AccountClient = new TetraSlideAccountClient(
                appSettings["TetraSlideAccessToken"],
                appSettings["TetraSlideApiBaseUrl"]);
        }

        protected void Session_Start(object sender, EventArgs e)
        {

        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {

        }

        protected void Application_AuthenticateRequest(object sender, EventArgs e)
        {

        }

        protected void Application_Error(object sender, EventArgs e)
        {

        }

        protected void Session_End(object sender, EventArgs e)
        {

        }

        protected void Application_End(object sender, EventArgs e)
        {

        }
    }
}