namespace TetraSlide.Api
{
    using Microsoft.ApplicationServer.Http;
    using System;
    using System.Net.Http;
    using System.ServiceModel;
    using System.Web;
    using System.Web.Routing;
    using TetraSlide.Api.Infrastructure;
    using TetraSlide.Api.Services;
    using System.Text;
    
    public class Global : System.Web.HttpApplication
    {
        static IAccountRepository AccountRepository { get; set; }
        static IGameRepository GameRepository { get; set; }

        protected object CreateServiceInstance(Type type, InstanceContext context, HttpRequestMessage message)
        {
            if (AccountRepository == null)
            {
                AccountRepository = new TestAccountRepository(new TestTokenEncoder());
            }
            if (GameRepository == null)
            {
                GameRepository = new TestGameRepository();
            }
            
            if (type == typeof(AccountService))
            {
                return new AccountService(AccountRepository);
            }

            if (type == typeof(GameService))
            {
                return new GameService(AccountRepository,GameRepository);
            }

            if (type == typeof(GamesService))
            {
                return new GamesService(AccountRepository, GameRepository);
            }

            return null;
        }

        protected void ReleaseServiceInstance(InstanceContext context, object service)
        {
            // TODO: maybe do something here if we need to someday.
        }

        protected void Application_Start(object sender, EventArgs e)
        {
            RouteTable.Routes.SetDefaultHttpConfiguration(new WebApiConfiguration() {
                EnableTestClient = true,               
                CreateInstance = this.CreateServiceInstance,
                ReleaseInstance = this.ReleaseServiceInstance
            });

            RouteTable.Routes.MapServiceRoute<AccountService>("account");
            RouteTable.Routes.MapServiceRoute<GameService>("game");
            RouteTable.Routes.MapServiceRoute<GamesService>("games");
        }

        protected void Session_Start(object sender, EventArgs e)
        {

        }

        protected void Application_BeginRequest(object sender, EventArgs e)
        {

        }

        protected void Application_AuthenticateRequest(object sender, EventArgs e)
        {

        }

        protected void Application_Error(object sender, EventArgs e)
        {

        }

        protected void Session_End(object sender, EventArgs e)
        {

        }

        protected void Application_End(object sender, EventArgs e)
        {

        }
    }
}