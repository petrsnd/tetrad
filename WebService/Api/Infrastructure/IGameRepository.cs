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
    public interface IGameRepository
    {
        /// <summary>
        /// Get a game using game id
        /// </summary>
        /// <param name="gameId">The id of the game requested/param>
        /// <returns>The requested game. Throws GameIdNotFoundException if gameId not found.</returns>
        Game Get(string gameId);

        /// <summary>
        /// Create game.  
        /// </summary>        
        /// <param name="game">The game to be create</param>
        /// <returns>The new game. </returns>
        Game Create(Game game);

        /// <summary>
        /// Update a game
        /// </summary>
        /// <param name="gameId">The game to be updated</param>
        /// <param name="data">updated game</param>
        /// <returns>The new game. Throws GameIdNotFoundException if gameId not found.</returns>        
        Game Update(string gameId, Game game);        

        /// <summary>
        /// Delete game
        /// </summary>
        /// <param name="gameId">The game to be deleted</param>
        /// <returns>The deleted game. Throws GameIdNotFoundException if gameId not found.</returns>
        Game Delete(string gameId);

        /// <summary>
        /// List games where the specified email address is a player
        /// </summary>
        /// <param name="emailAddress">player's email address</param>
        /// <returns></returns>
        IEnumerable<Game> List(string emailAddress);
    }   
}