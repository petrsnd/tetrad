using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;


namespace TetraSlide.Login
{
    public partial class Default : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            
        }

        protected void FacebookImageButton_Click(object sender, ImageClickEventArgs e)
        {
            string authUrl = Global.FacebookLoginProvider.GetAuthenticationUrl();
            Response.Redirect(authUrl);  
        }

        protected void GoogleImageButton_Click(object sender, ImageClickEventArgs e)
        {
            string authUrl = Global.GoogleLoginProvider.GetAuthenticationUrl();
            Response.Redirect(authUrl);
        }

        protected void LiveImageButton_Click(object sender, ImageClickEventArgs e)
        {
            string authUrl = Global.WindowsLiveLoginProvider.GetAuthenticationUrl();
            Response.Redirect(authUrl); 
        }

        protected void YahooImageButton_Click(object sender, ImageClickEventArgs e)
        {

        }        
    }
}