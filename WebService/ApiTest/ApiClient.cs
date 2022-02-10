namespace ApiTest
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Net.Http;
    using System.Net.Http.Formatting;
    using System.Net.Http.Headers;

    using TetraSlide.Api.Models;

    public class ApiClient
    {
        private Account superUserAccount;
            
        public ApiClient()
        {
            this.Client = new HttpClient();
            this.Client.BaseAddress = new Uri(this.BaseUrl);
            this.Client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            
            // TODO set these from app.config.
            this.superUserAccount = new Account();
            this.superUserAccount.email_address = "tetraslide@implbits.com";
            this.SuperUserAccount.access_token = "dGV0cmFzbGlkZUBpbXBsYml0cy5jb201";
            this.SuperUserAccount.name = "Implbits Superuser";
            this.SuperUserAccount.identity_provider = Idp.Google;
        }
        
        // TODO: Read these form the app.config
        public string BaseUrl 
        {
            get { return "http://localhost:50809"; }
        }

        public string SuperAccessToken
        {
            get { return SuperUserAccount.access_token; }
        }

        public string SuperEmailAddress
        {
            get { return SuperUserAccount.email_address; }
        }

        public Account SuperUserAccount
        {
            get { return superUserAccount; }
        }

        protected HttpClient Client { get; set; }
    }
}