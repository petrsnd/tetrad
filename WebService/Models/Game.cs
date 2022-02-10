namespace TetraSlide.Api.Models
{
    public class Game
    {
        public string id { get; set; }
        public string timestamp { get; set; }
        public Player[] players { get; set; }
        public string data { get; set; }

        public override bool Equals(System.Object obj)
        {
            // If parameter is null return false.
            if (obj == null)
            {
                return false;
            }

            // If parameter cannot be cast to Point return false.
            Game g = obj as Game;
            if ((System.Object)g == null)
            {
                return false;
            }

            // call Equals(Player)
            return Equals(g);
        }

        public bool Equals(Game p)
        {
            // If parameter is null return false:
            return id.Equals(p.id);
        }

        public override int GetHashCode()
        {
            return id.GetHashCode();
        }
    }
}