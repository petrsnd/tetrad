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

    using NUnit.Framework;

    using TetraSlide.Api.Models;
    using TetraSlide.Api.Services;
    
    [TestFixture]
    public class AccountServiceTest : ApiClient
    {
        [Test]
        public void InvalidToken()
        {
            // Invalid access_token
            string url = string.Format("/account/me?access_token={0}", "BADTOKEN");
            using (var responseTask = Client.GetAsync(url))
            {
                Assert.AreEqual(HttpStatusCode.Forbidden,responseTask.Result.StatusCode);                
            }
        }

        public HttpStatusCode Create(Account account)
        {
            string url = string.Format("/account?access_token={0}", SuperAccessToken);
            ObjectContent content = new ObjectContent(
                   typeof(Account),
                   account,
                   JsonMediaTypeFormatter.DefaultMediaType);

            using (var responseTask = Client.PostAsync(url, content))
            {
                return responseTask.Result.StatusCode;
            }
        }
        
        [Test]
        public void InvalidPost()
        {
            {
                // Account with missing email
                Account invalidAccount = new Account()
                {
                    name = "New Test User",
                    identity_provider = Idp.Facebook
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }

            {
                // Account with empty email name
                Account invalidAccount = new Account()
                {
                    email_address = "",
                    name = "New Test User",
                    identity_provider = Idp.Facebook
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }

            {
                // Account with bad email name
                Account invalidAccount = new Account()
                {
                    email_address = "invalid.example.com",
                    name = "New Test User",
                    identity_provider = Idp.Facebook
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }

            {
                // Account with no name
                Account invalidAccount = new Account()
                {
                    email_address = "invalid@example.com",
                    identity_provider = Idp.Facebook
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }

            {
                // Account with empty name
                Account invalidAccount = new Account()
                {
                    email_address = "invalid@example.com",
                    name = "",
                    identity_provider = Idp.Facebook
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }

            {
                // Account with no identity_provider
                Account invalidAccount = new Account()
                {
                    email_address = "invalid@example.com",
                    name =  "New Test User",                    
                };
                Assert.AreEqual(HttpStatusCode.BadRequest, Create(invalidAccount));
            }
        }

        [Test]
        public void DoubleDelete()
        {
            // The new account
            Account newAccount = new Account()
            {
                email_address = "newuser@example.com",
                name = "New Test User",
                identity_provider = Idp.Facebook
            };

            // Create the new account
            Assert.AreEqual(HttpStatusCode.OK, Create(newAccount));                        

            // Delete the new account
            {
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.DeleteAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account deletedAccount = contentTask.Result;
                        Assert.NotNull(deletedAccount);
                        Assert.NotNull(deletedAccount.access_token);
                        Assert.AreEqual(newAccount.email_address, deletedAccount.email_address);
                        Assert.AreEqual(newAccount.name, deletedAccount.name);
                        Assert.AreEqual(newAccount.identity_provider, deletedAccount.identity_provider);
                    }
                }
            }

            // Delete the new account (again)
            {
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.DeleteAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.NotFound, responseTask.Result.StatusCode);                    
                }
            }
        }

        [Test]
        public void NonexistentGet()
        {
            // The new account
            Account nonExistent = new Account()
            {
                email_address = "nonexistent@example.com",
                name = "nonexistent Test User",
                identity_provider = Idp.Facebook
            };
            
            // Read the new account
            {
                string url = string.Format("/account/{0}?access_token={1}", nonExistent.email_address, SuperAccessToken);
                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.NotFound, responseTask.Result.StatusCode);                    
                }
            }
        }

        [Test]
        public void DoubleCreate()
        {
            
            // The new account
            Account firstAccount = new Account()
            {
                email_address = "newuser@example.com",
                name = "First Creation",
                identity_provider = Idp.Facebook
            };
            
            // Create the first account
            Assert.AreEqual(HttpStatusCode.OK,Create(firstAccount));                
            {
                // Read the new account
                string url = string.Format("/account/{0}?access_token={1}", firstAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account readAccount = contentTask.Result;
                        Assert.NotNull(readAccount);
                        Assert.NotNull(readAccount.access_token);
                        Assert.AreEqual(firstAccount.email_address, readAccount.email_address);
                        Assert.AreEqual(firstAccount.name, readAccount.name);
                        Assert.AreEqual(firstAccount.identity_provider, readAccount.identity_provider);
                    }
                }
            }

            
            // The second account differs only by name
            Account secondAccount = new Account()
            {
                email_address = "newuser@example.com",
                name = "Second Creation",
                identity_provider = Idp.Facebook
            };
            // Create again.
            Assert.AreEqual(HttpStatusCode.OK, Create(secondAccount));
            {
                // make sure that the second account is there
                string url = string.Format("/account/{0}?access_token={1}", secondAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account readAccount = contentTask.Result;
                        Assert.NotNull(readAccount);
                        Assert.NotNull(readAccount.access_token);
                        Assert.AreEqual(secondAccount.email_address, readAccount.email_address);
                        Assert.AreEqual(secondAccount.name, readAccount.name);
                        Assert.AreEqual(secondAccount.identity_provider, readAccount.identity_provider);
                    }
                }
            }           
        }

        [Test]
        public void ValidPostGetDelete()
        {
            // The new account
            Account newAccount = new Account()
            {
                email_address = "newuser@example.com",
                name = "New Test User",
                identity_provider = Idp.Facebook
            };
            
            // Create the new account
            {
                string url = string.Format("/account?access_token={0}", SuperAccessToken);
                ObjectContent content = new ObjectContent(
                    typeof(Account),
                    newAccount,
                    JsonMediaTypeFormatter.DefaultMediaType);
                
                using (var responseTask = Client.PostAsync(url,content))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account createdAccount = contentTask.Result;
                        Assert.NotNull(createdAccount);
                        Assert.NotNull(createdAccount.access_token);
                        Assert.AreEqual(newAccount.email_address, createdAccount.email_address);
                        Assert.AreEqual(newAccount.name, createdAccount.name);
                        Assert.AreEqual(newAccount.identity_provider,createdAccount.identity_provider);
                    }
                }
            }

            {
                // Read the new account
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account createdAccount = contentTask.Result;
                        Assert.NotNull(createdAccount);
                        Assert.NotNull(createdAccount.access_token);
                        Assert.AreEqual(newAccount.email_address, createdAccount.email_address);
                        Assert.AreEqual(newAccount.name, createdAccount.name);
                        Assert.AreEqual(newAccount.identity_provider, createdAccount.identity_provider);
                    }
                }
            }            

            // Read the new account
            {
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);                
                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account createdAccount = contentTask.Result;
                        Assert.NotNull(createdAccount);
                        Assert.NotNull(createdAccount.access_token);
                        Assert.AreEqual(newAccount.email_address, createdAccount.email_address);
                        Assert.AreEqual(newAccount.name, createdAccount.name);
                        Assert.AreEqual(newAccount.identity_provider, createdAccount.identity_provider);
                    }
                }
            }

            // Delete the new account
            {
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);
                using (var responseTask = Client.DeleteAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.OK, responseTask.Result.StatusCode);
                    using (var contentTask = responseTask.Result.Content.ReadAsAsync<Account>())
                    {
                        Account createdAccount = contentTask.Result;
                        Assert.NotNull(createdAccount);
                        Assert.NotNull(createdAccount.access_token);
                        Assert.AreEqual(newAccount.email_address, createdAccount.email_address);
                        Assert.AreEqual(newAccount.name, createdAccount.name);
                        Assert.AreEqual(newAccount.identity_provider, createdAccount.identity_provider);
                    }
                }
            }

            // Read the deleted account (should be gone)
            {
                string url = string.Format("/account/{0}?access_token={1}", newAccount.email_address, SuperAccessToken);

                using (var responseTask = Client.GetAsync(url))
                {
                    Assert.AreEqual(HttpStatusCode.NotFound, responseTask.Result.StatusCode);                    
                }
            }
        }
    }
}
