using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WPF
{
    public class Move
    {
        public Move(ObjectModel.IPosition from, ObjectModel.Direction d, int distance)
        {
            Direction = d;
            From = from;
            Distance = distance;
            To = From.Translate(Direction, Distance);
        }

        public int Distance
        {
            get;
            private set;
        }
        public ObjectModel.Direction Direction
        {
            get;
            private set;
        }
        public ObjectModel.IPosition From
        {
            get;
            private set;
        }
        public ObjectModel.IPosition To
        {
            get;
            private set;
        }
    }
}
