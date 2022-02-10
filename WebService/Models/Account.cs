namespace TetraSlide.Api.Models
{
    using System;        

    public class Account
    {
        public string access_token { get; set; }
        public string email_address { get; set; }
        public string identity_provider { get; set; }
        public string name { get; set; }
    }
}