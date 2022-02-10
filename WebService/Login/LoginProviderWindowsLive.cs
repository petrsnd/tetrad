using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using TetraSlide.Api.Models;

namespace TetraSlide.Login
{
    public class WindowsLoginProviderLive : LoginProviderAbstract
    {
        #region WindowsLiveJsonTypes
        class WindowsLiveAccessToken
        {
            public string access_token { get; set; }
            public int expires_in { get; set; }
            public string scope { get; set; }
            public string token_type { get; set; } 
        }

        class WindowsLiveEmails
        {
            public string preferred { get; set; }
            public string account { get; set; }
            public string personal { get; set; }
            public string business { get; set; } 
        }

        class WindowsLiveIdentity
        {
            public string id { get; set; }
            public string name { get; set; }
            public WindowsLiveEmails emails { get; set; } 
        }
        #endregion

        public WindowsLoginProviderLive(string clientId, string clientSecret, string redirectUrl) :
            base(clientId, clientSecret, redirectUrl)
        {
        }

        public override string GetRedirectUrl()
        {
            return RedirectUrl;
        }

        public override string GetAuthenticationUrl()
        {
            return string.Format("https://oauth.live.com/authorize?client_id={0}&scope={1}&response_type=code&redirect_uri={2}",
                  HttpUtility.UrlEncode(ClientId),
                  "wl.basic wl.emails",
                  HttpUtility.UrlEncode(GetRedirectUrl()));            
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
            string accessToken = FetchLiveAccessToken(authorizationCode);
            Account account = FetchLiveIdentity(accessToken);
            return account;
        }

        protected string FetchLiveAccessToken(string authorizationCode)
        {
            string tokenUrl = String.Format("https://oauth.live.com/token?client_id={0}&redirect_uri={1}&client_secret={2}&code={3}&grant_type=authorization_code",
                    HttpUtility.UrlEncode(ClientId), //<== microsoft said this needs to be urlencoded. Stupid huh.
                    HttpUtility.UrlEncode(GetRedirectUrl()),
                    HttpUtility.UrlEncode(ClientSecret), //<== microsoft said this needs to be urlencoded. Stupid huh.
                    authorizationCode
                    );
            try
            {

                WindowsLiveAccessToken accessToken = WebRequestUtility.JsonHttpGet<WindowsLiveAccessToken>( tokenUrl);
                if (accessToken.access_token == null)
                {
                    throw new LoginException("Windows Live access token response did not contain access_token.");
                }

                return accessToken.access_token;
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Windows Live Access token: " + exception.Message);
            }
        }

        protected Account FetchLiveIdentity(string accessToken)
        {

            string userInfoUrl = string.Format("https://apis.live.net/v5.0/me?access_token={0}", HttpUtility.UrlEncode(accessToken));

            try
            {
                WindowsLiveIdentity liveIdentity = WebRequestUtility.JsonHttpGet<WindowsLiveIdentity>(userInfoUrl);
                return new Account()
                {
                    email_address = liveIdentity.emails.account,
                    name = liveIdentity.name,
                    identity_provider = Idp.WindowsLive
                };
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Windows Live Identity: " + exception.Message);
            }
        }
    }
}