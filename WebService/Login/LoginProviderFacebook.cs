using System;
using System.Web;
using System.IO;
using System.Net;

using TetraSlide.Api.Models;

namespace TetraSlide.Login
{
    public class LoginProviderFacebook : LoginProviderAbstract
    {
        #region FacebookJsonTypes
        class FacebookIdentity
        {
            public string id { get; set; }
            public string name { get; set; }
            public string email { get; set; }
        }
        #endregion
        
        public LoginProviderFacebook(string clientId, string clientSecret, string redirectUrl) :
            base(clientId, clientSecret, redirectUrl)
        {
        }

        public override string GetRedirectUrl()
        {
            return RedirectUrl;
        }

        public override string GetAuthenticationUrl()
        {
            return string.Format("https://www.facebook.com/dialog/oauth?client_id={0}&redirect_uri={1}&scope={2}",
                   ClientId,
                    HttpUtility.UrlEncode(GetRedirectUrl()), 
                   "email");                
        }

        public override string ParseAuthorizationResponse(HttpRequest request)
        {
            string error = request.QueryString["error"];
            string code = request.QueryString["code"];

            if (error != null)
            {
                throw new LoginException(HttpUtility.UrlDecode(error));
            }

            if (code == null)
            {
                throw new LoginException(HttpUtility.UrlDecode("No authorization code returned."));
            }

            return code;
        }

        public override Account GetIdentity(string authorizationCode)
        {
            // Get Google access token so that we can call the google identity API
            string accessToken = FetchFacebookAccessToken(authorizationCode);
            Account account = FetchFacebookIdentity(accessToken);
            return account;
        }

        protected string FetchFacebookAccessToken(string authorizationCode)
        {
            string tokenUrl = String.Format("https://graph.facebook.com/oauth/access_token?client_id={0}&redirect_uri={1}&client_secret={2}&code={3}",
                   ClientId,
                   HttpUtility.UrlEncode(GetRedirectUrl()),
                   HttpUtility.UrlEncode(ClientSecret),
                   authorizationCode
                   );

            try
            {
                WebRequest request = HttpWebRequest.Create(tokenUrl);
                request.Method = "GET";
                using (Stream s = request.GetResponse().GetResponseStream())
                {
                    using (StreamReader r = new StreamReader(s))
                    {
                        string tokenData = r.ReadToEnd();
                        string access_token = HttpUtility.ParseQueryString(tokenData)["access_token"];
                        return access_token;
                    }
                }
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Facebook Access token: " + exception.Message);
            }
        }

        protected Account FetchFacebookIdentity(string accessToken)
        {

            string userInfoUrl = string.Format("https://graph.facebook.com/me?access_token={0}", accessToken);           
            
            try
            {
                FacebookIdentity facebookIdentity = WebRequestUtility.JsonHttpGet<FacebookIdentity>(userInfoUrl);
                return new Account()
                {
                    email_address = facebookIdentity.email,
                    name = facebookIdentity.name,
                    identity_provider = Idp.Facebook
                };               
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Facebook Identity: " + exception.Message);
            }
        }
    }
}