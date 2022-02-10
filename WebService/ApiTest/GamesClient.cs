// -----------------------------------------------------------------------
// <copyright file="GamesClient.cs" company="">
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
    public class GamesClient : ApiClient
    {
        protected IEnumerable<Game> HandleResponseTask(Task<HttpResponseMessage> responseTask)
        {
            if (responseTask.Result.StatusCode != HttpStatusCode.OK)
            {
                throw new ApiClientException(responseTask.Result.StatusCode, responseTask.Result.Content.ReadAsStringAsync().Result);
            }

            using (var contentTask = responseTask.Result.Content.ReadAsAsync<IEnumerable<Game>>())
            {
                return contentTask.Result;
            }

        }      

        public IEnumerable<Game> GetMyGames(string accessToken)
        {
            string url = string.Format("games/mine?access_token={0}", accessToken);
            using (var responseTask = Client.GetAsync(url))
            {
                return HandleResponseTask(responseTask);
            }
        }
    }
}