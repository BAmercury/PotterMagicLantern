using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MagicLanternApp.UWP
{
    public class PrintMeService : IPrintMeService
    {
        public int printme()
        {
            return 5;
        }
    }
}
