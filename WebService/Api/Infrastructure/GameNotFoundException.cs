using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Infrastructure
{
    public class GameNotFoundException : Exception
    {
        public string BadGameId { get; private set; }

        public GameNotFoundException(string badGameId) :
            base("Game not found:: " + badGameId)
        {
            this.BadGameId = BadGameId;            
        }
    }    
}