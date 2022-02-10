using System;
using System.Linq;
using System.Text;
using TetraSlide.Api.Clients;
using TetraSlide.Api.Models;

namespace MattSux
{
    class Program
    {
        static int PrintUsage()
        {
            Console.WriteLine("AUTH TOKENS:");
            Console.WriteLine("  petsnd@gmail.com   == cGV0cnNuZEBnbWFpbC5jb201");
            Console.WriteLine("  dan@implbits       == ZGFuQGltcGxiaXRzLmNvbQ2");
            Console.WriteLine("  charone8@gmail.com == Y2hhcm9uZThAZ21haWwuY29t0");
            Console.WriteLine();
            StringBuilder sb = new StringBuilder();
            sb.AppendLine("USAGE:");
            sb.AppendLine("  To Test Game Play: MattSux <player1accessToken> <player2accessToken>");
            sb.AppendLine("      To List Games: MattSux <playeraccessToken>");
            sb.AppendLine("    To Delete Games: MattSux <playeraccessToken> delete <gameIdentifier1> ...");
            throw new Exception(sb.ToString());
        }
        static string GetEmailAddressForAccount(Account a)
        {
            if (a == null)
            {
                Console.WriteLine("Account was NULL");
                return "";
            }
            Console.WriteLine(string.Format("Account for: {0}", a.access_token));
            Console.WriteLine(string.Format("  Name:      {0}", a.name));
            Console.WriteLine(string.Format("  Email:     {0}", a.email_address));
            Console.WriteLine(string.Format("  Provider:  {0}", a.identity_provider));
            return a.email_address;
        }
        static void CheckGameForNull(Game g)
        {
            if (g == null)
            {
                throw new Exception("Game came back as null!");
            }
        }
        static void PrintGame(Game g)
        {
            Console.WriteLine("New game identifier: {0}", g.id);
            Console.WriteLine("  Player 1:  {0}", g.players[0].email);
            Console.WriteLine("  Player 2:  {0}", g.players[1].email);
            Console.WriteLine("  Timestamp: {0}", g.timestamp);
            Console.WriteLine("  Data:      {0}", g.data);
        }
        static void RunTwoMoveTest(string[] args)
        {
            var accountVerifier1 = new AccountClient(args[0]);
            var accountVerifier2 = new AccountClient(args[1]);

            Console.WriteLine("Getting account 1...");
            string email1 = GetEmailAddressForAccount(accountVerifier1.GetMy());
            Console.WriteLine("Getting account 2...");
            string email2 = GetEmailAddressForAccount(accountVerifier2.GetMy());

            if (string.IsNullOrEmpty(email1) || string.IsNullOrEmpty(email2))
            {
                Console.WriteLine("Please provide valid access tokens");
                PrintUsage();
            }

            Console.WriteLine("\n\n{0} creates a new game against {1}...", email1, email2);
            var clientPlayer1 = new GameClient(args[0]);
            var newGame = clientPlayer1.Create(email1, email2, "");
            CheckGameForNull(newGame);
            string newGameId = newGame.id;
            PrintGame(newGame);

            Console.WriteLine("\n\n{0} executes the first move...", email1);
            var updateGame = clientPlayer1.Update(newGameId, "==string for move one==");
            CheckGameForNull(updateGame);
            string updateGameId = updateGame.id;
            PrintGame(updateGame);
            if (newGameId != updateGameId)
            {
                throw new Exception(string.Format("Game ID changed from {0} to {1}!", newGameId, updateGameId));
            }
            if (!updateGame.data.Equals("==string for move one=="))
            {
                throw new Exception("Game did not come back with correct data");
            }

            Console.WriteLine("\n\n{1} tries to get the game he has been invited to by {0}...", email1, email2);
            var getterPlayer2 = new GamesClient(args[1]);
            var games = getterPlayer2.GetMine();
            if (games == null || games.Count() < 1)
            {
                throw new Exception("No games were returned!");
            }
            var getGame = games.FirstOrDefault(g => { return g.id == updateGameId; });
            if (getGame == null)
            {
                Console.WriteLine("Game identifiers returned:");
                foreach (var g in games)
                {
                    Console.WriteLine("  {0}", g.id);
                }
                throw new Exception(string.Format("Unable to find game with identifier: {0}", updateGameId));
            }
            if (!getGame.data.Equals("==string for move one=="))
            {
                throw new Exception(string.Format("Game did not have the current move from {0}!", email1));
            }

            Console.WriteLine("\n\n{0} executes the second move...", email2);
            var clientPlayer2 = new GameClient(args[1]);
            updateGame = clientPlayer2.Update(updateGameId, "==string for move TWO==");
            CheckGameForNull(updateGame);
            PrintGame(updateGame);
            string updateGameId2 = updateGame.id;
            if (updateGameId != updateGameId2)
            {
                throw new Exception(string.Format("Game ID changed from {0} to {1}!", updateGameId, updateGameId2));
            }
            if (!updateGame.data.Equals("==string for move TWO=="))
            {
                throw new Exception("Game did not come back with correct data");
            }

            Console.WriteLine("\n\n{0} gets the second move from {1}...", email1, email2);
            var getGame2 = clientPlayer1.Get(updateGameId);
            CheckGameForNull(updateGame);
            PrintGame(updateGame);
            string getGameId2 = getGame2.id;
            if (updateGameId != getGameId2)
            {
                throw new Exception(string.Format("Game ID changed from {0} to {1}!", updateGameId, getGameId2));
            }
            if (!getGame2.data.Equals("==string for move TWO=="))
            {
                throw new Exception("Game did not come back with correct data");
            }
        }
        static void RunGetGamesTest(string[] args)
        {
            Console.WriteLine("Get all games for: {0}", args[0]);
            var getterPlayer = new GamesClient(args[0]);
            var games = getterPlayer.GetMine();
            if (games == null || games.Count() < 1)
            {
                throw new Exception("No games were returned!");
            }
            Console.WriteLine("Game identifiers returned:");
            foreach (var g in games)
            {
                Console.WriteLine("  {0}", g.id);
            }
        }
        static void DeleteGames(string[] args)
        {
            Console.WriteLine("For: {0}", args[0]);
            var deleterPlayer = new GameClient(args[0]);
            for (int i = 2; i < args.Length; i++)
            {
                Console.WriteLine("  Deleting: {0}", args[i]);
                deleterPlayer.Delete(args[i]);
            }
        }
        static int Main(string[] args)
        {
            int result = 0;
            try
            {
                if (args.Count() > 2 && args[1].Equals("delete", StringComparison.OrdinalIgnoreCase))
                {
                    DeleteGames(args);
                }
                else if (args.Count() == 2)
                {
                    RunTwoMoveTest(args);
                }
                else if (args.Count() == 1)
                {
                    RunGetGamesTest(args);
                }
                else
                {
                    PrintUsage();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                result = 1;
            }
            

            Console.Write("\n\nPress any key...");
            Console.ReadKey();

            return result;
        }
    }
}
