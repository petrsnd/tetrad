namespace TetraSlide.Login
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Net;
    using System.Net.Http;
    using System.Net.Http.Formatting;
    using System.Net.Http.Headers;
    using System.Web;

    using TetraSlide.Api.Clients;
    using TetraSlide.Api.Models;

    public class TetraSlideAccountClient
    {
        protected AccountClient AccountClient{ get; set; }
        protected string AccessToken { get; set; }

        public TetraSlideAccountClient(string accessToken, string baseAddress)
        {
            AccountClient = new AccountClient(accessToken, baseAddress);
        }

        public Account UpdateOrCreateAccount(Account newAccount)
        {
            try
            {
                return AccountClient.Update(newAccount.email_address, newAccount);
            }
            catch (ApiClientException e)
            {
                if (e.StatusCode == HttpStatusCode.NotFound)
                {
                    return AccountClient.Create(newAccount);
                }

                throw e;
            }
        }       
    }
}