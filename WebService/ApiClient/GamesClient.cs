// -----------------------------------------------------------------------
// <copyright file="GamesClient.cs" company="Implbits Software LLC">
// All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

using Newtonsoft.Json;

namespace TetraSlide.Api.Clients
{
    using System.Collections.Generic;
    using System.Net;
    using System.Net.Http;
    using System.Threading.Tasks;
    using Models;

    /// <summary>
    /// Client API for calling the Games collection service (for listing games)
    /// </summary>
    public class GamesClient : ApiClient
    {
        /// <summary>
        /// Initializes a new instance of the GamesClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        public GamesClient(string accessToken)
            : base(accessToken)
        {
        }

        /// <summary>
        /// Initializes a new instance of the GamesClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        /// <param name="baseUrl">Base url of the Tetraslide API</param>
        public GamesClient(string accessToken, string baseUrl)
            : base(accessToken, baseUrl)
        {
        }
                
        /// <summary>
        /// Get games where caller is either the home or away player
        /// </summary>        
        /// <returns>The games</returns>
        public IEnumerable<Game> GetMine()
        {
            string url = string.Format("games/mine?access_token={0}", AccessToken);
            using (var responseTask = Client.GetAsync(url))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Process HttpReponse task
        /// </summary>
        /// <param name="responseTask">The response task to handle</param>
        /// <returns>The games returned in the http response</returns>
        protected IEnumerable<Game> HandleResponseTask(Task<HttpResponseMessage> responseTask)
        {
            if (responseTask.Result.StatusCode != HttpStatusCode.OK)
            {
                throw new ApiClientException(responseTask.Result.StatusCode, responseTask.Result.Content.ReadAsStringAsync().Result);
            }

            using (var contentTask = responseTask.Result.Content.ReadAsStringAsync())
            {
                return JsonConvert.DeserializeObject<IEnumerable<Game>>(contentTask.Result);
            }
        }
    }
}
