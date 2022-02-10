using System;
using System.Linq;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.ServiceModel.Web;
using System.Net.Http.Formatting;
using System.Net.Http.Headers;

using TetraSlide.Api.Exceptions;
using TetraSlide.Api.Models;
using TetraSlide.Api.Infrastructure;

namespace TetraSlide.Api.Services
{
    public class GamesService : ServiceBase
    {
        protected static IGameRepository GameRepository;        

        public GamesService(IAccountRepository accountRepository, IGameRepository gameRepository )  : base(accountRepository)
        {
            GameRepository = gameRepository;
        }       
        
        [WebGet(UriTemplate = "mine?access_token={accessToken}")]
        public IEnumerable<Game> Get(string accessToken)
        {
            // Get the caller's account
            Account caller = GetAccountByAccessToken(accessToken);

            // get games where email is a player
            return GameRepository.List(caller.email_address);            
        }       
    }    
}