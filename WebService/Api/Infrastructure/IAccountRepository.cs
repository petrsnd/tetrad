using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using TetraSlide.Api.Models;

namespace TetraSlide.Api.Infrastructure
{
    /// <summary>
    /// 
    /// </summary>
    public interface IAccountRepository
    {
        /// <summary>
        /// Find an account with access token
        /// </summary>
        /// <param name="emailAddress"></param>
        /// <returns></returns>
        Account FindByAccessToken(string accessToken);

        /// <summary>
        /// Get get an account using email address
        /// </summary>
        /// <param name="emailAddress"></param>
        /// <returns></returns>
        Account Get(string emailAddress);
        
        /// <summary>
        /// Create account.  Create over writes existing account.  
        /// </summary>
        /// <param name="emailAddress">account to be updated or created</param>
        /// <param name="account">new account values</param>
        /// <returns>the new account</returns>
        Account Create(Account account);

        /// <summary>
        /// Update a account
        /// </summary>
        /// <param name="gameId">The account to be updated</param>
        /// <param name="account">updated account</param>
        /// <returns>The updated account</returns>        
        Account Update(string emailAddress, Account account); 

        /// <summary>
        /// Delete account
        /// </summary>
        /// <param name="emailAddress">account to delete</param>
        /// <returns></returns>
        Account Delete(string emailAddress);        
    }   
}