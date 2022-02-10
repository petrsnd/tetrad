namespace ApiClientTest
{
    using System;
    using System.Collections.Generic;
    using System.Linq;    
    using System.Net;
    using NUnit.Framework;

    using TetraSlide.Api.Clients;
    using TetraSlide.Api.Models;
   
    
    [TestFixture]
    public class GameClientTest : ApiClientTestBase
    {
        GameClient gameClient;
        Account myAccount;        
        
        [SetUp]
        public void Init()
        {
            gameClient = new GameClient(ACCESS_TOKEN, BASE_URL);
            AccountClient accountClient = new AccountClient(ACCESS_TOKEN, BASE_URL);
            myAccount = accountClient.GetMy();
        }

        [TearDown]
        public void Deinit()
        {
        }

        [Test]
        public void CreateGetUpdateDelete()
        {
            Game createdGame = gameClient.Create(myAccount.email_address, "matt@peterzone.org", "Game Data string");
            Assert.NotNull(createdGame);

            Game gottenGame1 = gameClient.Get(createdGame.id);
            Assert.NotNull(gottenGame1);

            Game updatedGame = gameClient.Update(gottenGame1.id, "Updated game data");
            Assert.NotNull(updatedGame);

            Game gottenGame2 = gameClient.Get(updatedGame.id);
            Assert.NotNull(gottenGame2);
            Assert.AreEqual("Updated game data", gottenGame2.data);

            Game deletedGame = gameClient.Delete(gottenGame2.id);
            Assert.NotNull(deletedGame);

            ApiClientException ex = Assert.Throws<ApiClientException>(() => gameClient.Get(deletedGame.id));
            Assert.AreEqual(HttpStatusCode.NotFound, ex.StatusCode);
        }
        
    }
}