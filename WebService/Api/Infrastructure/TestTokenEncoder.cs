using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Text;

using TetraSlide.Api.Exceptions;
using TetraSlide.Api.Models;

namespace TetraSlide.Api.Infrastructure
{
    public class TestTokenEncoder : IAccessTokenEncoder
    {
        
        
        public TestTokenEncoder()
        {        
        }

        public string Decode(string accessToken)
        {
            try
            {
                System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
                return encoding.GetString(HttpServerUtility.UrlTokenDecode(accessToken));
            }
            catch (Exception)
            {
                throw new InvalidAccessTokenException(accessToken);  
            }
        }

        public string Encode(string emailAddress)
        {
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            return HttpServerUtility.UrlTokenEncode(encoding.GetBytes(emailAddress));
        }
    }
}