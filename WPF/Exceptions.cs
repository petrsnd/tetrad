using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WPF
{
    class TsException : Exception
    {
        public TsException() : base() { }
        public TsException(string message) : base(message) { }
    }

    class GameCreationFailedException : TsException
    {
        public GameCreationFailedException() :  base("Failed to create the game.")
        {}
    }
}
