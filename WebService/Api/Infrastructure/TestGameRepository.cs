using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using TetraSlide.Api.Exceptions;
using TetraSlide.Api.Models;

namespace TetraSlide.Api.Infrastructure
{
    public class TestGameRepository : IGameRepository
    {
        Dictionary<string, Game> Repository;

        public TestGameRepository()
        {
            Repository = new Dictionary<string, Game>();
        }

        public Game Get(string gameId)
        {
            Game value;

            lock (Repository)
            {
                value = GetGame(gameId);
            }

            return value; 
        }

        public Game Create(Game game)
        {
            game.id = GenerateGameId();
            game.timestamp = DateTime.UtcNow.ToString();

            lock (Repository)
            {
                Repository.Add(game.id, game);
            }

            return game;
        }

        public Game Update(string gameId, Game game)
        {
            lock (Repository)
            {
                // get the value we're gonna update
                Game value = GetGame(gameId);                

                // only update the stuff we need to.
                if (game.id != null) { value.id = game.id; }
                if (game.timestamp != null) { value.id = game.timestamp; }
                if (game.players != null) { value.players = game.players; }
                if (game.data != null) { value.data = game.data; }

                Repository[gameId] = value;

                return value;
            }                           
        }

        public Game Delete(string gameId)
        {
            Game value;
                        
            lock (Repository)
            {
                value = GetGame(gameId);
                Repository.Remove(gameId);
            }

            return value;
        }

        public IEnumerable<Game> List(string emailAddress)
        {
            Player player = new Player();
            player.email = emailAddress;

            var games = from g in Repository where g.Value.players.Contains(player) select g.Value;            
            if(games == null)
            {
                // if no matches, return empty enumerable;
                return new Game[0];
            }
            return games;
        }

        protected string GenerateGameId()
        {
            return HttpServerUtility.UrlTokenEncode(Guid.NewGuid().ToByteArray());
        }

        protected Game GetGame(string gameId)
        {
            Game value;

            if (Repository.TryGetValue(gameId, out value) == false)
            {
                throw new GameNotFoundException(gameId);
            }

            return value;
        }
    }   
}