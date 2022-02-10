using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Globalization;
using System.Text.RegularExpressions;

namespace TetraSlide.Api.Infrastructure
{
    public class EmailAddressValidator
    {
        static public bool IsValidEmailAddress(string strIn)
        {
            if (String.IsNullOrEmpty(strIn))
            {
                return false;
            }

            // Use IdnMapping class to convert Unicode domain names.
            try
            {
                strIn = Regex.Replace(strIn, @"(@)(.+)$", EmailAddressValidator.DomainMapper);
            }
            catch (ArgumentException)
            {
                return false;
            }

            // Return true if strIn is in valid e-mail format.
            return Regex.IsMatch(strIn,
                   @"^(?("")(""[^""]+?""@)|(([0-9a-z]((\.(?!\.))|[-!#\$%&'\*\+/=\?\^`\{\}\|~\w])*)(?<=[0-9a-z])@))" +
                   @"(?(\[)(\[(\d{1,3}\.){3}\d{1,3}\])|(([0-9a-z][-\w]*[0-9a-z]*\.)+[a-z0-9]{2,17}))$",
                   RegexOptions.IgnoreCase);
        }

        static private string DomainMapper(Match match)
        {
            IdnMapping idn = new IdnMapping();
            string domainName = match.Groups[2].Value;
            domainName = idn.GetAscii(domainName);            
            return match.Groups[1].Value + domainName;
        }
    }
}