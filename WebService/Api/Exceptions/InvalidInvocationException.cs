using System;
using Microsoft.ApplicationServer.Http.Dispatcher;
using System.Net;
using System.Net.Http;
using System.ServiceModel.Web;
using System.Net.Http.Formatting;
using System.Net.Http.Headers;

namespace TetraSlide.Api.Exceptions
{
    public class InvalidInvocationException : HttpResponseException
    {
        public InvalidInvocationException(HttpStatusCode statusCode, string message) :
            base(new HttpResponseMessage(statusCode))
        {
            Response.Content = new StringContent(message);            
        }
    }
}