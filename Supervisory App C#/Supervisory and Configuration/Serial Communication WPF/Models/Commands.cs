using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serial_Communication_WPF.Models
{
    class Commands
    {
        private int _id;

        public int ID
        {
            get { return _id; }
            set { _id = value; }
        }

        private string _description;

        public string Description
        {
            get { return _description; }
            set { _description = value; }
        }

        private string _command;

        public string Command
        {
            get { return _command; }
            set { _command = value; }
        }

        private string _expectedReply;

        public string ExpectedReply
        {
            get { return _expectedReply; }
            set { _expectedReply = value; }
        }

    }
}
