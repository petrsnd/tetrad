namespace TetraSlide.Api.Models
{
    public class Player
    {
        public string email {get; set; }
        public string name { get; set; }

        public override bool Equals(System.Object obj)
        {
            // If parameter is null return false.
            if (obj == null)
            {
                return false;
            }

            // If parameter cannot be cast to Point return false.
            Player p = obj as Player;
            if ((System.Object)p == null)
            {
                return false;
            }
            
            // call Equals(Player)
            return Equals(p);
        }

        public bool Equals(Player p)
        {
            // If parameter is null return false:
            return email.Equals(p.email);
        }

        public override int GetHashCode()
        {
            return email.GetHashCode();
        }
    }
}