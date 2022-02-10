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
    public class GamesClientTest : ApiClientTestBase
    {
        GameClient gameClient;
        GamesClient gamesClient;
        Account myAccount;        
        
        [SetUp]
        public void Init()
        {
            gameClient = new GameClient(ACCESS_TOKEN, BASE_URL);
            gamesClient = new GamesClient(ACCESS_TOKEN, BASE_URL);
            AccountClient accountClient = new AccountClient(ACCESS_TOKEN, BASE_URL);
            myAccount = accountClient.GetMy();
        }

        [TearDown]
        public void Deinit()
        {
        }

        [Test]
        public void CreateEnumerateDelete()
        {
            Game createdGame1 = gameClient.Create(myAccount.email_address, "matt@implbits.com", "Game Data string");
            Game createdGame2 = gameClient.Create(myAccount.email_address, "dan@implbits.com", "Game Data string");
            Game createdGame3 = gameClient.Create(myAccount.email_address, "cody@implbits.com", "Game Data string");


            IEnumerable<Game> games1 = gamesClient.GetMine();

            Assert.NotNull(games1);
            Assert.AreEqual(3, games1.Count());

            Assert.True((from g in games1 where g == createdGame1 select g).Count() == 0);
            Assert.True((from g in games1 where g == createdGame2 select g).Count() == 0);
            Assert.True((from g in games1 where g == createdGame3 select g).Count() == 0);

            gameClient.Delete(createdGame1.id);

            IEnumerable<Game> games2 = gamesClient.GetMine();
            Assert.AreEqual(2, games2.Count());
            Assert.True((from g in games2 where g == createdGame2 select g).Count() == 0);
            Assert.True((from g in games2 where g == createdGame3 select g).Count() == 0);

            gameClient.Delete(createdGame2.id);
            gameClient.Delete(createdGame3.id);

            IEnumerable<Game> games3 = gamesClient.GetMine();
            Assert.AreEqual(0, games3.Count());

        }

        
    }
}