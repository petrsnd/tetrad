// -----------------------------------------------------------------------
// <copyright file="AccountClient.cs" company="Implbits Software LLC">
// All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

namespace TetraSlide.Api.Clients
{
    using System.Net;
    using System.Net.Http;
    using System.Threading.Tasks;
    using Models;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class AccountClient : ApiClient
    {
        /// <summary>
        /// Initializes a new instance of the AccountClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        public AccountClient(string accessToken)
            : base(accessToken)
        {
        }

        /// <summary>
        /// Initializes a new instance of the AccountClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        /// <param name="baseUrl">Base url of the Tetraslide API</param>
        public AccountClient(string accessToken, string baseUrl)
            : base(accessToken, baseUrl)
        {
        }

        /// <summary>
        /// Get account of the caller
        /// </summary>        
        /// <returns>The accout</returns>
        public Account GetMy()
        {
            return this.Get("me");
        }

        /// <summary>
        /// Get account for the specified email address
        /// </summary>   
        /// <param name="emailAddress">The email address of the account to get</param>
        /// <returns>The account</returns>
        public Account Get(string emailAddress)
        {
            string url = string.Format("account/{0}?access_token={1}", emailAddress, AccessToken);
            using (var responseTask = Client.GetAsync(url))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Create specified account
        /// </summary>   
        /// <param name="emailAddress">The account to create</param>
        /// <returns>The account</returns>
        public Account Create(Account account)
        {
            // Create the new account
            string url = string.Format("account?access_token={0}", AccessToken);

            ObjectContent content = new ObjectContent(
               typeof(Account),
               account,
               JsonMediaTypeFormatter.DefaultMediaType);
            
            using (var responseTask = Client.PostAsync(url, content))
            {
                return this.HandleResponseTask(responseTask);
            }  
        }

        /// <summary>
        /// Update account data
        /// </summary>
        /// <param name="email">Email of the account to update</param>
        /// <param name="account">Updated account</param>
        /// <returns>The updated account</returns>
        public Account Update(string email, Account account)
        {
            string url = string.Format("account/{0}?access_token={1}", email, AccessToken);
                        
            ObjectContent content = new ObjectContent(
                typeof(Account),
                account,
                JsonMediaTypeFormatter.DefaultMediaType);

            using (var responseTask = Client.PutAsync(url, content))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Process HttpReponse task
        /// </summary>
        /// <param name="responseTask">The response task to handle</param>
        /// <returns>The account returned in the http response</returns>
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
    }
}
