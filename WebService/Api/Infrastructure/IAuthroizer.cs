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
    public interface IAuthorizer
    {
        /// <summary>
        /// Check to see if an account has access to a specifified functionality
        /// </summary>
        /// <param name="subjectEmail">The email address of subject to check access for</param>
        /// <param name="functionalityName">The name of functionality to check</param>
        /// <returns>true if access is allowed, otherwise false</returns>
        bool IsAccessAllowed(string subjectEmail, string functionalityName);        
    }
}