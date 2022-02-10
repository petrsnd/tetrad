// -----------------------------------------------------------------------
// <copyright file="GameClient.cs" company="Implbits Software LLC">
// All rights reserved.
// </copyright>
// -----------------------------------------------------------------------

using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using TetraSlide.Api.Models;
using Newtonsoft.Json;

namespace TetraSlide.Api.Clients
{
    /// <summary>
    /// Client for calling tetraslide Game API
    /// </summary>
    public class GameClient : ApiClient
    {
        /// <summary>
        /// Initializes a new instance of the GameClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        public GameClient(string accessToken)
            : base(accessToken)
        {
        }

        /// <summary>
        /// Initializes a new instance of the GameClient class.
        /// </summary>
        /// <param name="accessToken">Access token used to call the TetraSlide API</param>
        /// <param name="baseUrl">Base url of the Tetraslide API</param>
        public GameClient(string accessToken, string baseUrl)
            : base(accessToken, baseUrl)
        {
        }

        /// <summary>
        /// Create a game
        /// </summary>
        /// <param name="homeEmail">Home player's email address</param>
        /// <param name="awayEmail">Away players' email address</param>
        /// <param name="gameData">Game data</param>
        /// <returns>The created Game</returns>
        public Game Create(string homeEmail, string awayEmail, string gameData)
        {
            string url = string.Format("game?access_token={0}", AccessToken);

            Player[] newPlayers = 
            {
                new Player()
                {
                    email = homeEmail
                },
                new Player()
                {
                    email = awayEmail
                }
            };

            Game newGame = new Game()
            {
                players = newPlayers,
                data = gameData
            };

            var content = new StringContent(JsonConvert.SerializeObject(newGame));
            
            using (var responseTask = Client.PostAsync(url, content))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Get a game
        /// </summary>
        /// <param name="gameId">Id of the game to get</param>
        /// <returns>The game</returns>
        public Game Get(string gameId)
        {
            string url = string.Format("game/{0}?access_token={1}", gameId, AccessToken);
            using (var responseTask = Client.GetAsync(url))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Update game data
        /// </summary>
        /// <param name="gameId">Id of the game to update</param>
        /// <param name="gameData">updated data</param>
        /// <returns>The updated game</returns>
        public Game Update(string gameId, string gameData)
        {
            string url = string.Format("game/{0}?access_token={1}", gameId, AccessToken);

            Game updateGame = new Game()
            {
                id = gameId,
                data = gameData
            };

            var content = new StringContent(JsonConvert.SerializeObject(updateGame));

            using (var responseTask = Client.PutAsync(url, content))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Delete a game
        /// </summary>
        /// <param name="gameId">Id of game to delete</param>
        /// <returns>The deleted game</returns>
        public Game Delete(string gameId)
        {
            string url = string.Format("game/{0}?access_token={1}", gameId, AccessToken);
            using (var responseTask = Client.DeleteAsync(url))
            {
                return this.HandleResponseTask(responseTask);
            }
        }

        /// <summary>
        /// Process HttpReponse task
        /// </summary>
        /// <param name="responseTask">The response task to handle</param>
        /// <returns>The game returned in the http response</returns>
        /// <exception cref="ApiClientException">For any response other than HTTP 200</exception>
        protected Game HandleResponseTask(Task<HttpResponseMessage> responseTask)
        {
            if (responseTask.Result.StatusCode != HttpStatusCode.OK)
            {
                throw new ApiClientException(responseTask.Result.StatusCode, responseTask.Result.Content.ReadAsStringAsync().Result);
            }

            using (var contentTask = responseTask.Result.Content.ReadAsStringAsync())
            {
                return JsonConvert.DeserializeObject<Game>(contentTask.Result);
            }
        }
    }
}
