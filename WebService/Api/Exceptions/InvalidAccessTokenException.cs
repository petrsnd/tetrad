using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Exceptions
{
    public class InvalidAccessTokenException : Exception
    {
        public string BadAccessToken { get; private set; }

        public InvalidAccessTokenException(string badAccessToken) : 
            base ("Invalid Access Token: " + badAccessToken)
        {
            this.BadAccessToken = badAccessToken;            
        }
    }    
}