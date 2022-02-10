using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Infrastructure
{
    public class InvalidGameException : Exception
    {
        public InvalidGameException(string message) :
            base("Invalid Game: " + message)
        {
        }
    }    
}