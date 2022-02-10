using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace WPF
{
    class Logger
    {
        static Logger logger = null;
        public static void Log(string msg)
        {
            lock (typeof(Logger))
            {
                StackFrame CallStack = new StackFrame(1, true);
                if (logger == null)
                    logger = new Logger();
                logger.LogInternal(msg, CallStack.GetFileName(), CallStack.GetFileLineNumber().ToString());
            }
        }

        public static void Log(string msg, string file, string line)
        {
            lock (typeof(Logger))
            {
                if (logger == null)
                    logger = new Logger();
                logger.LogInternal(msg, file, line);
            }
        }

        StreamWriter m_logfile = null;
        StringBuilder m_msgBuilder = new StringBuilder();
        private Logger()
        {
            string path = System.IO.Path.GetDirectoryName(
               System.Reflection.Assembly.GetExecutingAssembly().GetName().CodeBase);
            path = new Uri(path).LocalPath;
            path = Path.Combine(path, "ts-log.txt");
            
            m_logfile = new StreamWriter(new FileStream(path, FileMode.Create));
        }
        
        

        private void LogInternal(string msg, string file, string line)
        {
            DateTime ts = DateTime.Now;
            m_msgBuilder.Clear();
            m_msgBuilder.Append("[");
            m_msgBuilder.Append(ts.ToShortTimeString());
            m_msgBuilder.Append("]");
            m_msgBuilder.AppendFormat("{0}:{1} - {2}", Path.GetFileName(file), line, msg);
            m_logfile.WriteLine(m_msgBuilder.ToString());
            m_logfile.Flush();
        }

        public static string TilesToStringList(IEnumerable<int> tileIds)
        {
            StringBuilder sb = new StringBuilder();
            foreach (int i in tileIds)
                sb.AppendFormat("{0},", i.ToString());
            return sb.ToString();
        }

        public static string DirectionToString(ObjectModel.Direction dir)
        {
            switch (dir)
            {
                case ObjectModel.Direction.Down: return "DOWN";
                case ObjectModel.Direction.Up: return "UP";
                case ObjectModel.Direction.Right: return "RIGHT";
                case ObjectModel.Direction.Left: return "LEFT";
            }
            return "** INVALID DIRECTION **";
        }
    }
}
