using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TetraSlide.Win8
{
    class PersistentStorage
    {
        private static PersistentStorage m_instance = null;
        public static PersistentStorage Instance()
        {
            if (m_instance == null)
                m_instance = new PersistentStorage();
            return m_instance;
        }
    }
}
