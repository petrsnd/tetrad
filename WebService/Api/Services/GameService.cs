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
    public class GameService : ServiceBase
    {
        protected static IGameRepository GameRepository;        

        public GameService(IAccountRepository accountRepository, IGameRepository gameRepository )  : base(accountRepository)
        {
            GameRepository = gameRepository;
        }       
        
        [WebGet(UriTemplate = "{gameId}?access_token={accessToken}")]
        public Game Get(string gameId, string accessToken)
        {
            // Get the caller's account
            Account caller = GetAccountByAccessToken(accessToken);

            Game value = GetGameById(gameId);
            
            if( (from p in value.players where p.email == caller.email_address select p).Count() == 0 )
            {
                // AUTHORIZATION: The caller is not a player in this game.
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Access denied to to game with id: " + gameId);
            }

            return value;
        }

        [WebInvoke(UriTemplate = "?access_token={accessToken}", Method = "POST")]
        public Game Create(string accessToken, Game game)
        {
            // Get the caller's account
            Account caller = GetAccountByAccessToken(accessToken);
            List<Account> playerAccounts = new List<Account>();

            // Make sure there are two players opponent (player in the game that is not the caller) and that the opponent exists.
            if (game.players == null && game.players.Count() != 2)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "Must be two players");
            }

            // Make sure that the caller is a player
            if( (from p in game.players where p.email == caller.email_address select p).Count() == 0)
            {
                // AUTHORIZATION: caller is trying to create a game where they are not a player
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "Caller must be a player");
            }
                
            // Make sure the players have accounts.
            foreach (Player p in game.players)
            {
                try
                {
                    playerAccounts.Add(AccountRepository.Get(p.email));
                }
                catch (AccountNotFoundException)
                {
                    // Account doesn't exist for one of the players.  Create a place holder account (unknown IDP).
                    playerAccounts.Add(AccountRepository.Create( new Account() { email_address = p.email, identity_provider = Idp.Unknown } ) );                            
                }
            }            

            // Create the game
            try
            {
                return GameRepository.Create(game);
            }
            catch (InvalidGameException ex)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, ex.Message);
            }                     
            
        }

        [WebInvoke(UriTemplate = "{gameId}?access_token={accessToken}", Method = "PUT")]
        public Game Update(string gameId, string accessToken, Game game)
        {
            // Get the caller's account
            Account caller = GetAccountByAccessToken(accessToken);

            Game target = GetGameById(gameId);
            if ((from p in target.players where p.email == caller.email_address select p).Count() == 0)
            {
                // AUTHORIZATION: The caller is not a player in this game. Can't update.
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Access denied to to game with id: " + gameId);
            }

            try
            {
                // Null out the fields that we don't want to be updated                
                game.id = null;
                game.timestamp = null;
                game.players = null;

                return GameRepository.Update(gameId, game);
            }
            catch (InvalidGameException ex)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, ex.Message);
            }
        }

        [WebInvoke(UriTemplate = "{gameId}?access_token={accessToken}", Method = "DELETE")]
        public Game Delete(string gameId, string accessToken)
        {
            Account caller = GetAccountByAccessToken(accessToken);            
            Game value = GetGameById(gameId);

            if ((from p in value.players where p.email == caller.email_address select p).Count() == 0)
            {
                // AUTHORIZATION: The caller is not a player in this game. Can't delete.
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Access denied to to game with id: " + gameId);
            }

            try
            {
                // AUTHORIZATION
                return GameRepository.Delete(gameId);
            }
            catch (InvalidGameException ex)
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, ex.Message);
            }            
        }

        protected Game GetGameById(string gameId)
        {
            if (String.IsNullOrWhiteSpace(gameId))
            {
                throw new InvalidInvocationException(HttpStatusCode.BadRequest, "gameId was omitted or whitespace");
            }
            try
            {
                return GameRepository.Get(gameId);
            }
            catch (GameNotFoundException ex)
            {
                throw new InvalidInvocationException(HttpStatusCode.NotFound, ex.Message);
            }           
        }
    }    
}