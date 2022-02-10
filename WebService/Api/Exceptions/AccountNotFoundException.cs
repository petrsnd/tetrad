using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Exceptions
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