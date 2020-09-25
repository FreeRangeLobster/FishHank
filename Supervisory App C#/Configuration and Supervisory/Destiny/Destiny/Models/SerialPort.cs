using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Destiny.Models
{
    public class SerialPort
    {

        private int _PortNumber;
        public int PortNumber
        {
            get { return _PortNumber; }
            set { _PortNumber = value; }
        }

        private string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

    }
}
