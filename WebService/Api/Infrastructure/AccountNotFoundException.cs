using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Infrastructure
{
    public class AccountNotFoundException : FormatException
    {
        public string MissingAccount { get; private set; }

        public AccountNotFoundException(string missingAccount) : 
            base ("Account not found: " + missingAccount)
        {
            MissingAccount = missingAccount;            
        }
    }
}