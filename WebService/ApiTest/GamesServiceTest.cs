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

    using NUnit.Framework;

    using TetraSlide.Api.Models;
    using TetraSlide.Api.Services;
    
    [TestFixture]
    public class GamesServiceTest : ApiClient
    {
        AccountClient accountClient;
        GameClient gameClient;
        GamesClient gamesClient;        

        static Account[] accounts = 
        {
            new Account()
            {
                email_address = "matt@implbits.com",
                identity_provider = Idp.Google,
                name = "Matt Peterson"
            },

            new Account()
            {
                email_address = "dan@implbits.com",
                identity_provider = Idp.Google,
                name = "Dan Peterson"
            },
            
            new Account()
            {
                email_address = "cody@implbits.com",
                identity_provider = Idp.Google,
                name = "Cody Batt"
            },

            new Account()
            {
                email_address = "john@implbits.com",
                identity_provider = Idp.Google,
                name = "John Bowers"
            }
        };

        static Player[] players =
        {
            new Player()
            {
                email = accounts[0].email_address,
                name = accounts[0].name
            },

            new Player()
            {
                email = accounts[1].email_address,
                name = accounts[1].name
            },

            new Player()
            {
                email = accounts[2].email_address,
                name = accounts[2].name
            }
        };    

        Game[] games =
        {
            new Game() 
            {
                players = new Player[]
                {
                    players[0],
                    players[1]
                },
                data = "opaque game data for player 0 and player 1"
            },
           
            new Game() 
            {
                players = new Player[]
                {
                    players[0],
                    players[2]
                },
                data = "opaque game data for player 0 and player 2"
            }

            // Player3 is not involved in any game.
        };
        
        [SetUp]
        public void Init()
        {
            accountClient = new AccountClient();
            gameClient = new GameClient();
            gamesClient = new GamesClient();

            // Create accounts
            foreach (Account a in accounts)
            {
                Account createdAccount = accountClient.Create(a);
                a.access_token = createdAccount.access_token;
            }

            // Create games
            foreach (Game g in games)
            {
                Game createdGame = gameClient.Create(g,accounts[0].access_token);
                g.id = createdGame.id;
            }
        }

        [TearDown]
        public void Deinit()
        {
            // Delete games
            foreach (Game g in games)
            {
                gameClient.Delete(g.id, accounts[0].access_token);
            }

            // Delete accounts
            foreach (Account a in accounts)
            {
                accountClient.Delete(a.email_address);
            }
        }

        [Test]
        public void GetValidGames()
        {
            // Get games for player 0
            IEnumerable<Game> player0Games = gamesClient.GetMyGames(accounts[0].access_token);
            Assert.AreEqual(2, player0Games.Count());            

            // Get games for player 1
            IEnumerable<Game> player1Games = gamesClient.GetMyGames(accounts[1].access_token);
            Assert.AreEqual(1, player1Games.Count());
            Assert.AreEqual(games[0].data, player1Games.First().data);

            // Get games for player 2
            IEnumerable<Game> player2Games = gamesClient.GetMyGames(accounts[2].access_token);
            Assert.AreEqual(1, player2Games.Count());
            Assert.AreEqual(games[1].data, player2Games.First().data);

            // Get games for player 3
            IEnumerable<Game> player3Games = gamesClient.GetMyGames(accounts[3].access_token);
            Assert.AreEqual(0, player3Games.Count());
        }
    }
}