using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Destiny.Models;
using System.Collections.ObjectModel;


namespace Destiny.ViewModels
{
    public class ViewModel
    {
        private ObservableCollection<SerialPort> _serialPorts;

        public ObservableCollection<SerialPort> SerialPorts
        {
            get { return _serialPorts; }
            set { _serialPorts = value; }
        }






        //private ObservableCollection<SerialPort> _SerialPorts;

        //public ObservableCollection<SerialPort> SerialPorts
        //{
        //    get { return _SerialPorts; }
        //    set { _SerialPorts = value; }
        //}


        public ViewModel() {

            SerialPorts = new ObservableCollection<SerialPort>() { new SerialPort() { PortNumber = 1,Name="Hello" } };

        }
    }
}
