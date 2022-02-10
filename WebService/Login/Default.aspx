<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="TetraSlide.Login.Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>TetraSlide Login</title>    
</head>
<body>
    <form id="form1" runat="server">
    <div align="center">    
        <table>
            <tr>
                <td >
                    <h1>TetraSlide Login</h1>
                    <hr />
                </td>                
            </tr>
            <tr>
                <td >
                    <asp:ImageButton ID="FacebookImageButton" runat="server" 
                        ImageUrl="~/images/facebook_logo.png" 
                        onclick="FacebookImageButton_Click" />
                </td>                
            </tr>
            <tr>
                <td>
                    <asp:ImageButton ID="GoogleImageButton" runat="server" 
                        ImageUrl="~/images/google_logo.png" onclick="GoogleImageButton_Click" />
                </td>                
            </tr>
            <tr>
                <td>
                    <asp:ImageButton ID="LiveImageButton" runat="server" 
                        ImageUrl="~/images/windows_live_logo.png" 
                        onclick="LiveImageButton_Click" />
                </td>                
            </tr>
            <tr>
                <td>
                    <asp:ImageButton ID="YahooImageButton" runat="server" 
                        ImageUrl="~/images/yahoo_logo.png" onclick="YahooImageButton_Click" />
    
                </td>                
            </tr>
        </table>        
    </div>
    </form>
</body>
</html>
