// -----------------------------------------------------------------------
// <copyright file="ApiClientException.cs" company="Implbits Software LLC">
// All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

namespace TetraSlide.Api.Clients
{
    using System;
    using System.Net;

    /// <summary>
    /// Represents an error that has occured in an TetraSlide API client
    /// </summary>
    public class ApiClientException : Exception        
    {
        /// <summary>
        /// Initializes a new instance of the ApiClientException class.
        /// </summary>
        /// <param name="statusCode">An http status code</param>
        /// <param name="message">A useful message for humans</param>
        public ApiClientException(HttpStatusCode statusCode, string message) :
            base(message)
        {
            this.StatusCode = statusCode;
        }

        /// <summary>
        /// Gets the HTTP status code
        /// </summary>
        public HttpStatusCode StatusCode
        {
            get;
            private set;
        }
    }
}
