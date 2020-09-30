﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Threading;
using System.Windows.Threading;
using System.IO;
using System.Xml.Linq;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.ComponentModel;


namespace Serial_Communication_WPF
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        SerialPort ComPort = new SerialPort();
        string recieved_data;
        Paragraph para = new Paragraph();
        FlowDocument mcFlowDoc = new FlowDocument();

        string OutputText;



        /// <summary>
        /// Raised when a property on this object has a new value.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;


        public Window1()
        {
            InitializeComponent();

            string[] ArrayComPortsNames = null;
            int index = -1;
            string ComPortName = null;
            //Com Ports
            ArrayComPortsNames = SerialPort.GetPortNames();
            do
            {
                index += 1;
                cboComPortsAvailable .Items.Add(ArrayComPortsNames[index]);


            } while (!((ArrayComPortsNames[index] == ComPortName) || (index == ArrayComPortsNames.GetUpperBound(0))));
            Array.Sort(ArrayComPortsNames);


           


            //Baud Rate
            cboBaudRate.Items.Add(300);
            cboBaudRate.Items.Add(600);
            cboBaudRate.Items.Add(1200);
            cboBaudRate.Items.Add(2400);
            cboBaudRate.Items.Add(9600);
            cboBaudRate.Items.Add(14400);
            cboBaudRate.Items.Add(19200);
            cboBaudRate.Items.Add(38400);
            cboBaudRate.Items.Add(57600);
            cboBaudRate.Items.Add(115200);
            cboBaudRate.Items.ToString();
            //get first item print in text
        //    cboBaudRate.Text = cboBaudRate.Items[9].ToString();
            //Data Bits
            cboDataBits.Items.Add(7);
            cboDataBits.Items.Add(8);
            //get the first item print it in the text 
      //      cboDataBits.Text = cboDataBits.Items[1].ToString();

            //Stop Bits
            cboStopBits.Items.Add("One");
            cboStopBits.Items.Add("OnePointFive");
            cboStopBits.Items.Add("Two");
            //get the first item print in the text
    //        cboStopBits.Text = cboStopBits.Items[0].ToString();
            //Parity 
            cboParity.Items.Add("None");
            cboParity.Items.Add("Even");
            cboParity.Items.Add("Mark");
            cboParity.Items.Add("Odd");
            cboParity.Items.Add("Space");
            //get the first item print in the text
  //          cboParity.Text = cboParity.Items[0].ToString();
            //Handshake
            cboHandShaking.Items.Add("None");
            cboHandShaking.Items.Add("XOnXOff");
            cboHandShaking.Items.Add("RequestToSend");
            cboHandShaking.Items.Add("RequestToSendXOnXOff");
            //get the first item print it in the text 
//            cboHandShaking.Text = cboHandShaking.Items[0].ToString();

            //Load settings
            cboComPortsAvailable.Text = Properties.Settings.Default.CommsName;
            cboBaudRate.Text= Properties.Settings.Default.BaudRate;
            cboDataBits.Text=Properties.Settings.Default.Databits;
            cboParity.Text=Properties.Settings.Default.Parity;
            cboStopBits.Text=Properties.Settings.Default.StopBits;
            cboHandShaking.Text=Properties.Settings.Default.Handshake;




        }


        //public event RoutedPropertyChangedEventHandler PropertyChanged;
        //private void OnPropertyChanged([CallerMemberName] string propertyName = null)
        //{
        //    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        //}



        private void CmdConnect_Click(object sender, RoutedEventArgs e)
        {
            
                cmdConnect.Content = "Open";
                ComPort.PortName = Convert.ToString(cboComPortsAvailable.Text );
                ComPort.BaudRate = Convert.ToInt32(cboBaudRate.Text);
                ComPort.DataBits = Convert.ToInt16(cboDataBits.Text);
                ComPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cboStopBits.Text);
                ComPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), cboHandShaking.Text);
                ComPort.Parity = (Parity)Enum.Parse(typeof(Parity), cboParity.Text);
                ComPort.Open();

                ComPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(Recieve);

        }

        private void CmdDisconnect_Click(object sender, RoutedEventArgs e)
        {
                //cmdConnect.Content = "Closed";
                ComPort.Close();
        }



        #region Recieving

        private delegate void UpdateUiTextDelegate(string text);
        private void Recieve(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            // Collecting the characters received to our 'buffer' (string).
            recieved_data = ComPort.ReadExisting();


            Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(WriteData), recieved_data);
            //Update Testboxes
            Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal), recieved_data);
            Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal2), recieved_data);

            

            //OutputText = OutputText + recieved_data;
            //UpdateTerminal(OutputText);

        }



        /// <summary>
        /// Raises this object's PropertyChanged event.
        /// </summary>
        /// <param name="propertyName">The property that has a new value.</param>
        protected virtual void OnPropertyChanged(string propertyName)
        {


            PropertyChangedEventHandler handler = this.PropertyChanged;
            if (handler != null)
            {
                var e = new PropertyChangedEventArgs(propertyName);
                handler(this, e);
            }
        }

        private void UpdateTerminal(string text)
        {
            OutputBox.Text = OutputBox.Text + text;
        }

        private void UpdateTerminal2(string text)
        {
            OutputBox2.Text = OutputBox2.Text + text;
        }




        private void WriteData(string text)
        {
            // Assign the value of the recieved_data to the RichTextBox.
            para.Inlines.Add(text);
            mcFlowDoc.Blocks.Add(para);
            Commdata.Document = mcFlowDoc;
            Commdata.ScrollToEnd();

            //CommdataStatus.Document = mcFlowDoc;
            //CommdataStatus.ScrollToEnd();
        }

        #endregion


        private void cmdSend1_Click(object sender, RoutedEventArgs e) 

        {
            SerialCmdSend(tbCommand1.Text);            
        }

        private void CmdSend2_Click(object sender, RoutedEventArgs e)
        {
            SerialCmdSend(tbCommand2.Text);
        }


        private void CmdSend3_Click(object sender, RoutedEventArgs e)
        {
            SerialCmdSend(tbCommand3.Text);
        }

        private void CmdSend4_Click(object sender, RoutedEventArgs e)
        {
            SerialCmdSend(tbCommand4.Text);
        }


        public void SerialCmdSend(string data)
        {
            if (ComPort.IsOpen)
            {
                try
                {

                    para.Inlines.Add("->" + data + "\r\n");
                    mcFlowDoc.Blocks.Add(para);
                    Commdata.Document = mcFlowDoc;
                    Commdata.ScrollToEnd();

                    //CommdataStatus.Document = mcFlowDoc;
                    //CommdataStatus.ScrollToEnd();

                    // Send the binary data out the port
                    byte[] hexstring = Encoding.ASCII.GetBytes(data);
                    //There is a intermitant problem that I came across
                    //If I write more than one byte in succesion without a 
                    //delay the PIC i'm communicating with will Crash
                    //I expect this id due to PC timing issues ad they are
                    //not directley connected to the COM port the solution
                    //Is a ver small 1 millisecound delay between chracters
                    foreach (byte hexval in hexstring)
                    {
                        byte[] _hexval = new byte[] { hexval }; // need to convert byte to byte[] to write
                        ComPort.Write(_hexval, 0, 1);
                        Thread.Sleep(1);
                    }
                }
                catch (Exception ex)
                {
                    para.Inlines.Add("Failed to SEND" + data + "\n" + ex + "\n");
                    mcFlowDoc.Blocks.Add(para);
                    Commdata.Document = mcFlowDoc;

                    //CommdataStatus.Document = mcFlowDoc;
                }
            }
            else
            {
            }
        }

        private void CmdClear_Click(object sender, RoutedEventArgs e)
        {

            para.Inlines.Clear();
            mcFlowDoc.Blocks.Clear();
            Commdata.Document = mcFlowDoc;
            Commdata.ScrollToEnd();

            //CommdataStatus.Document = mcFlowDoc;
            //CommdataStatus.ScrollToEnd();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Properties.Settings.Default.CommsName = cboComPortsAvailable.Text;
            Properties.Settings.Default.BaudRate = cboBaudRate.Text;
            Properties.Settings.Default.Databits = cboDataBits.Text;
            Properties.Settings.Default.Parity = cboParity.Text;
            Properties.Settings.Default.StopBits = cboStopBits.Text;
            Properties.Settings.Default.Handshake = cboHandShaking.Text ;
            Properties.Settings.Default.Save();
        }

        private void cmdOpenConfigFilePC_Click(object sender, RoutedEventArgs e)
        {
            // Create OpenFileDialog
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            // Set filter for file extension and default file extension
            dlg.DefaultExt = ".txt";
            dlg.Filter = "Text documents (.txt)|*.txt";

            // Display OpenFileDialog by calling ShowDialog method
            Nullable<bool> result = dlg.ShowDialog();

            // Get the selected file name and display in a TextBox
            if (result == true)
            {
                // Open document
                string filename = dlg.FileName;
                FileNameTextBox.Text  = filename;

                Paragraph paragraph = new Paragraph();
                paragraph.Inlines.Add(System.IO.File.ReadAllText(filename));
                FlowDocument document = new FlowDocument(paragraph);
                //FlowDocReader.Document = document;
            }
        }

        private void cmdOpenFile_Click(object sender, RoutedEventArgs e)
        {
            TextRange range;
            FileStream fStream;

            if (File.Exists(FileNameTextBox.Text))
            {
                range = new TextRange(rtbFileContents.Document.ContentStart, rtbFileContents.Document.ContentEnd);
                fStream = new FileStream(FileNameTextBox.Text, FileMode.OpenOrCreate);
                range.Load(fStream, DataFormats.Text);
                fStream.Close();
            }

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            //XElement booksFromFile = XElement.Load(@"books.xml");
            XElement booksFromFile = XElement.Load("C:/Users/Juan/Desktop/HankEagle/FishHank/Supervisory App C#/Serial Communication WPF/Settings/Sample.xml");
            Console.WriteLine(booksFromFile);
            Console.WriteLine(booksFromFile.FirstNode);
        }

        private void cmdUpdateTime_Click(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 5; i++)
            {
                System.Windows.Controls.Button newBtn = new Button();

                newBtn.Content = i.ToString();
                newBtn.Name = "Button" + i.ToString();
                newBtn.Click += new RoutedEventHandler(button_Click);
                //sp.Children.Add(newBtn);
            }
        }

        void button_Click(object sender, RoutedEventArgs e)
        {
            Console.WriteLine(string.Format("You clicked on the {0}. button.", (sender as Button).Name));
        }

       

        private void HandleStatusTestButton(object sender, RoutedEventArgs e)
        {
            Button myButton = (Button)sender;

            switch (myButton.Name)
            {
                case "cmdHelp":
                    SerialCmdSend("?");
                    myButton.Background   = Brushes.Yellow; 

                    break;

                case "cmdInitilise":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdIdle":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdUpdateScreen":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdCheckEvents":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdUpdateOutputs":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdDeleteMemory":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdReadEvents":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdAddEvent":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdDisableEvent":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdSpare1":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdUpdateOutputOne":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdWriteStatus":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdSetRTC":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdReadTime":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdCheckCurrentEvent":
                    myButton.Background = Brushes.Yellow;
                    break;

                case "cmdSpare2":
                    myButton.Background = Brushes.Yellow;
                    break;

            }
        }

        private void CboComPortsAvailable_SelectionChanged()
        {

        }
    }

    }
