// -----------------------------------------------------------------------
// <copyright file="ApiClientException.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ApiTest
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Net;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ApiClientException : Exception        
    {
        public HttpStatusCode StatusCode {get; set; }

        public ApiClientException(HttpStatusCode statusCode, string message) :
            base(message)
        {
            this.StatusCode = statusCode;
        }
    }
}
