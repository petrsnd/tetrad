using System;
using System.Collections.Generic;

using TetraSlide.Api.Exceptions;
using TetraSlide.Api.Models;

namespace TetraSlide.Api.Infrastructure
{
    /// <summary>
    /// 
    /// </summary>
    public class TestAccountRepository : IAccountRepository
    {
        IAccessTokenEncoder AccessTokenEncoder;
        Dictionary<string, Account> Repository;

        public TestAccountRepository( IAccessTokenEncoder accessTokenEncoder)
        {
            AccessTokenEncoder = accessTokenEncoder;
            
            Repository = new Dictionary<string, Account>();


            Account superAccount = Create(new Account()
            {
                email_address = "tetraslide@implbits.com",
                name = "TetraSlide SuperUser",
                identity_provider = Idp.Google
            });
            
            // The Super User account access_token is: dGV0cmFzbGlkZUBpbXBsYml0cy5jb201
        }

        public Account FindByAccessToken(string accessToken)
        {
            string callerEmail = AccessTokenEncoder.Decode(accessToken);
            Account value = Get(callerEmail);
            return value;
        }

        public Account Get(string emailAddress)
        {
            Account value;

            lock (Repository)
            {
                if (Repository.TryGetValue(emailAddress, out value) == false)
                {
                    throw new AccountNotFoundException(emailAddress);
                }
            }

            return value;           
        }

        public Account Create(Account account)
        {
            account.access_token = AccessTokenEncoder.Encode(account.email_address);
            
            lock(Repository)
            {
                Repository.Remove(account.email_address);
                Repository.Add(account.email_address, account);
            }

            return account;
        }


        public Account Update(string emailAddress, Account account)
        {
            lock (Repository)
            {
                // get the value we're gonna update
                Account value = GetAccount(emailAddress);

                // only update the stuff we need to.
                if (account.access_token != null) { value.access_token = account.access_token; }
                if (account.email_address != null) { value.email_address = account.email_address; }
                if (account.identity_provider != null) { value.identity_provider = account.identity_provider; }
                if (account.name != null) { value.name = account.name; }

                Repository[emailAddress] = value;

                return value;
            }
        }

        public Account Delete(string emailAddress)
        {
            Account value = Get(emailAddress);

            lock (Repository)
            {
                Repository.Remove(emailAddress);
            }

            return value;            
        }

        protected Account GetAccount(string emailAddress)
        {
            Account value;
            
            if (Repository.TryGetValue(emailAddress, out value) == false)
            {
                throw new AccountNotFoundException(emailAddress);
            }            

            return value; 
        }
    }
}