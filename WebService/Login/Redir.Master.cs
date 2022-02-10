using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using TetraSlide.Api.Models;

namespace TetraSlide.Login
{
    public partial class Redir : System.Web.UI.MasterPage
    {
        public ILoginProvider LoginProvider;        
        
        public void Login_Success(Account account)
        {
            Page.Title = String.Format("success code={0}", account.access_token);
            LoginStatusLabel.Text = "Success";
            LoginMessageLabel.Text = "";
            AccessCodeTextbox.Text = account.access_token;
            NameLabel.Text = account.name;
            EmailAddressLabel.Text = account.email_address;
            IdentityProviderLabel.Text = account.identity_provider;
            LoginAttributesDiv.Visible = true;
        }

        public void Login_Error(string errorMessage)
        {
            Page.Title = String.Format("error message={0}", errorMessage);
            LoginStatusLabel.Text = "Failed";
            LoginMessageLabel.Text = errorMessage;
            LoginAttributesDiv.Visible = false;            
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if( string.IsNullOrEmpty(Page.ClientQueryString))
            {
                Response.Redirect(LoginProvider.GetAuthenticationUrl());
            }            
            
            try
            {
                string authorizationCode = LoginProvider.ParseAuthorizationResponse(Request);
                Account account = LoginProvider.GetIdentity(authorizationCode);
                account = Global.AccountClient.UpdateOrCreateAccount(account);                
                Login_Success(account);
            }
            catch (LoginException exception)
            {
                Login_Error(exception.Message);
            }                        
        }         
    }
}