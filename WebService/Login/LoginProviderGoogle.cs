using System;
using System.Web;
using System.Web.Script.Serialization;

using TetraSlide.Api.Models;

namespace TetraSlide.Login
{

#region GoogleJsonTypes
    class GoogleAccessToken
    {
        public string access_token { get; set; }
        public string expires_in { get; set; }
        public string token_type { get; set; }
    }

    class GoogleIdentity
    {
        public string id { get; set; }
        public string email { get; set; }
        public bool verified_email { get; set; }
        public string name { get; set; }
        public string given_name { get; set; }
        public string family_name { get; set; }
    }
    
#endregion
    
    public class LoginProviderGoogle : LoginProviderAbstract
    {
        public LoginProviderGoogle(string clientId, string clientSecret, string redirectUrl) :
            base(clientId, clientSecret, redirectUrl)
        {
        }

        public override string GetRedirectUrl()
        {
            return RedirectUrl;
        }

        public override string GetAuthenticationUrl()
        {
            return String.Format("https://accounts.google.com/o/oauth2/auth?response_type=code&client_id={0}&redirect_uri={1}&scope={2}",
                ClientId,
                HttpUtility.UrlEncode(GetRedirectUrl()),
                HttpUtility.UrlEncode("https://www.googleapis.com/auth/userinfo.email https://www.googleapis.com/auth/userinfo.profile"));
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
            string accessToken = FetchGoogleAccessToken(authorizationCode);
            Account account = FetchGoogleIdentity(accessToken);
            return account;
        }

        protected string FetchGoogleAccessToken(string authorizationCode)
        {
            string postData = String.Format(
                "code={0}&client_id={1}&client_secret={2}&redirect_uri={3}&grant_type=authorization_code",
                authorizationCode,
                ClientId,
                ClientSecret,
                HttpUtility.UrlEncode(GetRedirectUrl()));            
            try
            {
                
                GoogleAccessToken accessToken = WebRequestUtility.JsonHttpPost<GoogleAccessToken>(
                    "https://accounts.google.com/o/oauth2/token",
                    postData);                
                if(accessToken.access_token == null)
                {
                    throw new LoginException("Google access token response did not contain access_token.");
                }
                        
                return accessToken.access_token;                                  
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Google Access token: " + exception.Message);
            }
        }

        protected Account FetchGoogleIdentity(string accessToken)
        {
            string userInfoUrl = String.Format("https://www.googleapis.com/oauth2/v1/userinfo?access_token={0}",
                HttpUtility.HtmlEncode(accessToken));
            try
            {
                GoogleIdentity googleIdentity = WebRequestUtility.JsonHttpGet<GoogleIdentity>(userInfoUrl);
                return new Account()
                {
                    email_address = googleIdentity.email,
                    name = googleIdentity.name,
                    identity_provider = Idp.Google
                };                
            }
            catch (Exception exception)
            {
                throw new LoginException("Unable to obtain Facebook Access Code: " + exception.Message);
            }
        }
    }
}