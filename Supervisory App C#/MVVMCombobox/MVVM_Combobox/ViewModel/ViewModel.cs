using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MVVM_Combobox.Model;
using System.Collections.ObjectModel;

namespace MVVM_Combobox.ViewModel
{
    public class ViewModel
    {
        private ObservableCollection<Person> _persons;

        public ObservableCollection<Person> Persons
        {
            get { return _persons; }
            set { _persons = value; }
        }


        private Person _sperson;

        public Person SPerson
        {
            get { return _sperson; }
            set { _sperson = value; }
        }

        public ViewModel()
        {
            Persons = new ObservableCollection<Person>() {new Person(){ Id=1, Name="Nirav"}
            ,new Person(){Id=2,Name="Kapil"}
            ,new Person(){Id=3 , Name="Arvind"}
            ,new Person(){Id=4 , Name="Rajan"}};
        }
        
    }
}
