using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TetraSlide.Api.Models
{
    public class Idp
    {
        public const string Unknown = "Unknown";
        public const string Tetraslide = "Tetraslide";
        public const string Google = "Google";
        public const string Facebook = "Facebook";
        public const string WindowsLive = "WindowsLive";

        public static string[] Valid = { Unknown, Tetraslide, Google, Facebook, WindowsLive };

        public static bool IsValid(string identityProvider)
        {
            return Valid.Contains(identityProvider);
        }
    }
}