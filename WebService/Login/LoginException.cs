using System;

namespace TetraSlide.Login
{
    public class LoginException : Exception
    {
        public LoginException(string errorMessage)
            : base(errorMessage)
        {
        }
    }
}