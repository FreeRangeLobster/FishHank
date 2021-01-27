using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serial_Communication_WPF.Models
{
    class SerialCommSettings
    {
        private String _Port;

        public String Port
        {
            get { return _Port; }
            set { _Port = value; }
        }

        private int _baudrate;

        public int BaudRate
        {
            get { return _baudrate; }
            set { _baudrate = value; }
        }

        private string _parity;

        public string Parity
        {
            get { return _parity; }
            set { _parity = value; }
        }

        private string _handshake;

        public string Handshake
        {
            get { return _handshake; }
            set { _handshake = value; }
        }

        private int _databits;

        public int DataBits
        {
            get { return _databits; }
            set { _databits = value; }
        }

        private int _stopbits;

        public int StopBits
        {
            get { return _stopbits; }
            set { _stopbits = value; }
        }



    }
}
