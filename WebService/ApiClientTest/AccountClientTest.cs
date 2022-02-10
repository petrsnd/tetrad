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
    public class AccountClientTest : ApiClientTestBase
    {
        AccountClient accountClient;
        
        [SetUp]
        public void Init()
        {
            accountClient = new AccountClient(ACCESS_TOKEN, BASE_URL);
        }

        [TearDown]
        public void Deinit()
        {
        }

        [Test]
        public void GetMy()
        {
            Account myAccount = accountClient.GetMy();
            Assert.NotNull(myAccount);
            Assert.AreEqual(ACCESS_TOKEN, myAccount.access_token);
        }
    }
}