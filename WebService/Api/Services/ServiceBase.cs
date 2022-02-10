using System;
using Microsoft.ApplicationServer.Http.Dispatcher;
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
    public class ServiceBase
    {
        protected static IAccountRepository AccountRepository;

        public ServiceBase(IAccountRepository accountRepository)
        {
            AccountRepository = accountRepository;
        }

        protected Account GetAccountByAccessToken(string accessToken)
        {
            if (String.IsNullOrWhiteSpace(accessToken))
            {
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "access_token was omitted or whitespace");
            }
            try
            {
                return AccountRepository.FindByAccessToken(accessToken);
            }
            catch (InvalidAccessTokenException)
            {
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Bad access_token=" + accessToken);
            }
            catch (AccountNotFoundException)
            {
                throw new InvalidInvocationException(HttpStatusCode.Forbidden, "Bad access_token=" + accessToken);
            }
        }
    }
}