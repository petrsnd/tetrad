namespace ApiTest
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;
    using System.Text;
    using NUnit.ConsoleRunner;

    public class Program
    {
        public static void Main(string[] args)
        {
            Runner.Main(new string[] { Assembly.GetExecutingAssembly().Location });
        }
    }
}
