using System.Windows;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System;

namespace WpfApplication2
{
    public partial class MainWindow : INotifyPropertyChanged
    {
        public MainWindow()
        {
            DataContext = this;
            InitializeComponent();
        }


        private void Reset_Click(object sender, RoutedEventArgs e)
        {
            BoundNumber = 0;
            ButtonState = !ButtonState;
        }

        private bool _buttonState;

        public bool ButtonState
        {
            get { return _buttonState; }
            set {

                if (_buttonState != value)
                {
                    _buttonState = value;
                    OnPropertyChanged();
                    Console.WriteLine("Button pressed " + value.ToString());
                }
              

            }
        }


        private int _boundNumber;
        public int BoundNumber
        {
            get { return _boundNumber; }
            set
            {
                if(_boundNumber != value)
                {
                    _boundNumber = value;
                    OnPropertyChanged();
                }
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {

        }
    }
}
