using System;
using System.Net;
using System.Net.Http;
using System.ServiceModel.Web;
using System.Net.Http.Formatting;
using System.Net.Http.Headers;

using TetraSlide.Api.Exceptions;
using TetraSlide.Api.Models;
using TetraSlide.Api.Infrastructure;


namespace TetraSlide.Api.Services
{
    public class AccountService : ServiceBase
    {
        public static string ME = "me";        
        
        public AccountService(IAccountRepository accountRepository) : base(accountRepository)
        {
        }

        [WebGet(UriTemplate = "{emailAddress}?access_token={accessToken}")]
        public Account Get(string emailAddress, string accessToken)
        {
            // Check the accessToken
            Account caller = GetAccountByAccessToken(accessToken);

            // Fixup the emailAddress so that we can handle "me" and null email the same way   
            emailAddress = FixEmailAddressForME(emailAddress, caller);
            
            Account value;
            if (emailAddress == caller.email_address)
            {
                // AUTHORIZATION: caller wants to get their own account                
                value = caller;
            }
            else
            {
                // AUTHORIZATION: caller wants an get another account other than their own.  This requires privilege.
                try
                {
                    value = AccountRepository.Get(emailAddress);
                }
                catch (AccountNotFoundException)
                {
                    throw new InvalidInvocationException(HttpStatusCode.NotFound, "Account not found with with email address: " + emailAddress);
                }
            }

            return value;
        }

        [WebInvoke(UriTemplate = "?access_token={accessToken}", Method="POST")]
        public Account Create(string accessToken, Account account)
        {
            // Check the accessToken
            Account caller = GetAccountByAccessToken(accessToken);

            // AUTHORIZATION: caller wants to add or modify an account.  This requires privilege.
            if (IsGodLike(caller) == false)
            {
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Access denied to create account with email: " + account.email_address);
            }

            CheckAccountBody(account);
            
            Account value = AccountRepository.Create(account);
            
            return value;            
        }

        [WebInvoke(UriTemplate = "{emailAddress}?access_token={accessToken}", Method = "PUT")]
        public Account Update(string emailAddress, string accessToken, Account account)        
        {
            // Get the caller's account
            Account caller = GetAccountByAccessToken(accessToken);
            
            // AUTHORIZATION: The caller is trying to update an account.
            if(IsGodLike(caller) == false)
            {
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Access denied to update account with email: " + account.email_address);
            }            

            try
            {
                // Null out the fields that we don't want to be updated
                // account.access_token = null;
                return AccountRepository.Update(emailAddress, account);
            }
            catch (AccountNotFoundException ex)
            {
                throw new InvalidInvocationException(HttpStatusCode.NotFound, "Account not found with with email address: " + ex.MissingAccount);
            }        
        }

        [WebInvoke(UriTemplate = "{emailAddress}?access_token={accessToken}",Method="DELETE")]
        public Account Delete(string emailAddress, string accessToken)
        {
            // Check the accessToken
            Account caller = GetAccountByAccessToken(accessToken);

            emailAddress = FixEmailAddressForME(emailAddress, caller);
            if (emailAddress == caller.email_address)
            {
                // AUTHORIZATION: caller wants to delete their own account
            }
            else
            {
                // AUTHORIZATION: caller wants to delete an account.  This requires privilege.                
            }

            try
            {
                Account value = AccountRepository.Delete(emailAddress);
                return value;
            }
            catch (AccountNotFoundException)
            {
                throw new InvalidInvocationException(HttpStatusCode.NotFound, String.Empty);
            }                
        }       

        protected string FixEmailAddressForME(string emailAddress, Account account)
        {
            // If email address ommitted, the caller wants their own account
            if (string.IsNullOrWhiteSpace(emailAddress) || emailAddress == ME)
            {
                return account.email_address;
            }

            return emailAddress;            
        }

        protected void CheckAccountBody(Account account)
        {
            // Check the post body to make sure it isn't missing anything stupid.
            if (account == null)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "request body is empty");
            }
            if (EmailAddressValidator.IsValidEmailAddress(account.email_address) == false)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "missing email_address in request body account object");
            }
            if (String.IsNullOrWhiteSpace(account.name))
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "missing name in request body account object");
            }
            if (String.IsNullOrWhiteSpace(account.identity_provider) || Idp.IsValid(account.identity_provider) == false)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "invalid or missing identity_provider in request body account object");
            }
        }

        protected bool IsGodLike(Account account)
        {
            if (account.email_address == "tetraslide@implbits.com")
            {
                return true;
            }
            return false;
        }
    }
}