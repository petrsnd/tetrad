using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;

using TetraSlide.Api.Models;

namespace TetraSlide.Login
{
    /// <summary>
    /// Interface for a "Login Provider" which uses OAuth2 "code" flow for
    /// authentication and then subsequent call to identity service to fetch
    /// identity attributes for authenticated user.
    /// </summary>
    public interface ILoginProvider
    {
        /// <summary>
        /// Returns the OAuth2 redirect URL for this provider.  Caller is responsible
        /// for URL encoding if necessary.
        /// </summary> 
        string GetRedirectUrl();

        /// <summary>
        /// Generate OAuth2 authorization URL.  
        /// </summary>
        /// <returns>
        /// Returned string must only contain valid http URL characters (i.e. 
        /// URL encode stuff that needs to be URL encoded).  Caller should
        /// client to this URL
        /// </returns>
        string GetAuthenticationUrl();

        /// <summary>
        /// Parses OAuth2 authorization code that was sent to redirect URL.
        /// </summary>
        /// <param name="request">
        /// The HTTP Request that was sent to redirect URL.
        /// </param>
        /// <returns>
        /// The authorization code
        /// </returns>
        /// <exception cref="LoginException">An error occurred.  Most commonly, the
        /// user denied access that was requested to view full name and email address
        /// </exception>
        string ParseAuthorizationResponse(HttpRequest request);

        /// <summary>
        /// Uses authentication code to fetch identity information.
        /// </summary>
        /// <param name="authorizationUrl">
        /// Authorization code that was returned to the redirect url
        /// </param>
        /// <returns>
        /// Returned string must only contain valid http URL characters (i.e. 
        /// URL encode stuff that needs to be URL encoded)
        /// </returns>        
        Account GetIdentity(string authorizationCode);
    }
    
    
    /// <summary>
    /// Abstract class for a "Login Provider" which uses OAuth2 "code" flow for
    /// authentication and then subsequent call to identity service to fetch
    /// identity attributes for authenticated user.
    /// </summary>
    public abstract class LoginProviderAbstract : ILoginProvider
    {
        protected string ClientId;
        protected string ClientSecret;
        protected string RedirectUrl;
        
        protected LoginProviderAbstract(string clientId, string clientSecret, string redirectUrl)
        {
            ClientId = clientId;
            ClientSecret = clientSecret;
            RedirectUrl = redirectUrl;
        }
       
        public abstract string GetRedirectUrl();
        public abstract string GetAuthenticationUrl();        
        public abstract string ParseAuthorizationResponse(HttpRequest request);
        public abstract Account GetIdentity(string authorizationCode);        
    }
}
