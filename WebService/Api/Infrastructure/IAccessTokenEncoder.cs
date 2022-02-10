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
    public interface IAccessTokenEncoder
    {
        string Encode(string emailAddress);     
        string Decode(string accessToken);        
    }
}