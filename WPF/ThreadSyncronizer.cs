using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace WPF
{
    class ThreadSynchronizer
    {
        public static string FoundTetradSignal = "FoundTetrad";
        private static ThreadSynchronizer instance = new ThreadSynchronizer();

        public static void WaitForSignal(string signalName)
        {
            if (!instance.handles.ContainsKey(signalName))
                instance.handles.Add(signalName, new EventWaitHandle(false, EventResetMode.ManualReset));
            instance.handles[signalName].WaitOne();
        }

        public static void Signal(string signalName)
        {
            if (instance.handles.ContainsKey(signalName))
            {
                instance.handles[signalName].Set();
                instance.handles.Remove(signalName);
            }
        }

        Dictionary<string, EventWaitHandle> handles = new Dictionary<string, EventWaitHandle>();

        EventWaitHandle waitHandle = new EventWaitHandle(false, EventResetMode.ManualReset);
    }
}
