using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace TetraSlide.Login.facebook
{
    public partial class Default : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            Redir redir = (Redir) Master;
            redir.LoginProvider = Global.GoogleLoginProvider;
        }
    }
}