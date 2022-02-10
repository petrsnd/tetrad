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
    public class GameServiceTest : ApiClient
    {
        AccountClient accountClient;
        GameClient gameClient;
        
        Account opponent = new Account()
        {
            email_address = "dan@implbits.com",
            identity_provider = Idp.Google,
            name = "Dan Peterson"
        };

        
        
        [SetUp]
        public void Init()
        {
            accountClient = new AccountClient();
            gameClient = new GameClient();

            // Create an account that we can invite to games
            opponent = accountClient.Create(opponent);            
        }

        [TearDown]
        public void Deinit()
        {
            accountClient.Delete(opponent.email_address);             
        }

        [Test]
        public void ValidCRUD()
        {
            
            // Create
            Game initGame = new Game()
            {
                players = new Player[]
                {
                    new Player()
                    {
                        email = accountClient.SuperEmailAddress
                    },
                    new Player()               
                    {
                        email = opponent.email_address
                    }
                },
                data = "valid game data"
            };

            Game createdGame = gameClient.Create(initGame);
            Assert.NotNull(createdGame);
            Assert.NotNull(createdGame.id);
            Assert.NotNull(createdGame.timestamp);
            Assert.NotNull(createdGame.players);
            Assert.Greater(createdGame.players.Count(), 1);

            // Get
            Game gottenGame = gameClient.Get(createdGame.id);
            Assert.NotNull(gottenGame);
            Assert.NotNull(gottenGame.id);
            Assert.NotNull(gottenGame.timestamp);
            Assert.NotNull(gottenGame.players);
            Assert.AreEqual(createdGame.players.Count(), gottenGame.players.Count());
            Assert.AreEqual(createdGame.players[0].email,gottenGame.players[0].email);
            Assert.AreEqual(createdGame.players[1].email,gottenGame.players[1].email);
            Assert.AreEqual(createdGame.data, gottenGame.data);

            // Update
            Game gameUpdate = new Game() { data = "Updated Data" };
            Game updatedGame = gameClient.Update(gottenGame.id, gameUpdate);
            Assert.NotNull(updatedGame);
            Assert.NotNull(updatedGame.id);
            Assert.NotNull(updatedGame.timestamp);
            Assert.NotNull(updatedGame.players);
            Assert.AreEqual(gottenGame.players.Count(), updatedGame.players.Count());
            Assert.AreEqual(gottenGame.players[0].email, updatedGame.players[0].email);
            Assert.AreEqual(gottenGame.players[1].email, updatedGame.players[1].email);
            Assert.AreEqual(gameUpdate.data, updatedGame.data);
            
            // Delete
            Game deletedGame = gameClient.Delete(updatedGame.id);
            Assert.NotNull(deletedGame);
            Assert.NotNull(deletedGame.id);
            Assert.NotNull(deletedGame.timestamp);
            Assert.NotNull(deletedGame.players);
            Assert.AreEqual(updatedGame.players.Count(), deletedGame.players.Count());
            Assert.AreEqual(updatedGame.players[0].email, deletedGame.players[0].email);
            Assert.AreEqual(updatedGame.players[1].email, deletedGame.players[1].email);
            Assert.AreEqual(updatedGame.data, deletedGame.data);

            // Get (make sure game was deleted). Should throw.
            ApiClientException ex = Assert.Throws<ApiClientException>(() => gameClient.Delete(deletedGame.id));
            Assert.AreEqual(HttpStatusCode.NotFound, ex.StatusCode);
        }
    }
}