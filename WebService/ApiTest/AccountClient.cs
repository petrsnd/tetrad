// -----------------------------------------------------------------------
// <copyright file="AccountClient.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ApiTest
{
    using System;
    using System.Collections.Generic;
    using System.Json;
    using System.Linq;
    using System.Net;
    using System.Net.Http;
    using System.Net.Http.Formatting;
    using System.Net.Http.Headers;
    using System.Text;
    using System.Threading.Tasks;
    using System.Web;
        
    using TetraSlide.Api.Models;
    using TetraSlide.Api.Services;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class AccountClient : ApiClient
    {
        protected Account HandleResponseTask(Task<HttpResponseMessage> responseTask)
        {
            if (responseTask.Result.StatusCode != HttpStatusCode.OK)
            {
                throw new ApiClientException(responseTask.Result.StatusCode, responseTask.Result.Content.ReadAsStringAsync().Result);
            }
                
            using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
            {
                return contentTask.Result;                    
            }
            
        }
        
        public Account Create(Account account)
        {
            string url = string.Format("/account?access_token={0}", SuperAccessToken);
            ObjectContent content = new ObjectContent(
                typeof(Account),
                account,
                JsonMediaTypeFormatter.DefaultMediaType);

            using (var responseTask = Client.PostAsync(url, content))
            {
                return HandleResponseTask(responseTask);
            }
        }

        public Account Delete(string emailAddress)
        {
            string url = string.Format("/account/{0}?access_token={1}", emailAddress, SuperAccessToken);
            using (var responseTask = Client.DeleteAsync(url))
            {
                return HandleResponseTask(responseTask);                
            }
        }
    }
}
