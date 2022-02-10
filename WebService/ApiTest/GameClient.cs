// -----------------------------------------------------------------------
// <copyright file="GameClient.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ApiTest
{
    using System;
    using System.Collections.Generic;    
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
    public class GameClient : ApiClient
    {
        protected Game HandleResponseTask(Task<HttpResponseMessage> responseTask)
        {
            if (responseTask.Result.StatusCode != HttpStatusCode.OK)
            {
                throw new ApiClientException(responseTask.Result.StatusCode, responseTask.Result.Content.ReadAsStringAsync().Result);
            }

            using (var contentTask = responseTask.Result.Content.ReadAsAsync<Game>())
            {
                return contentTask.Result;
            }

        }

        public Game Create(Game game, string accessToken)
        {
            string url = string.Format("/game?access_token={0}", accessToken);
            ObjectContent content = new ObjectContent(
                typeof(Game),
                game,
                JsonMediaTypeFormatter.DefaultMediaType);

            using (var responseTask = Client.PostAsync(url, content))
            {
                return HandleResponseTask(responseTask);
            }
        }

        public Game Create(Game game)
        {
            return Create(game, SuperAccessToken);  
        }

        public Game Get(string gameId)
        {
            string url = string.Format("/game/{0}?access_token={1}", gameId, SuperAccessToken);
            using (var responseTask = Client.GetAsync(url))
            {
                return HandleResponseTask(responseTask);
            }
        }

        public Game Update(string gameId, Game game)
        {
            string url = string.Format("/game/{0}?access_token={1}", gameId, SuperAccessToken);
            ObjectContent content = new ObjectContent(
                typeof(Game),
                game,
                JsonMediaTypeFormatter.DefaultMediaType);

            using (var responseTask = Client.PutAsync(url, content))
            {
                return HandleResponseTask(responseTask);
            }
        }

        public Game Delete(string gameId, string accessToken)
        {
            string url = string.Format("/game/{0}?access_token={1}", gameId, accessToken);
            using (var responseTask = Client.DeleteAsync(url))
            {
                return HandleResponseTask(responseTask);
            }
        }

        public Game Delete(string gameId)
        {
            return Delete(gameId, SuperAccessToken);
        }
    }
}