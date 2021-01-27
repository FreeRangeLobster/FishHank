using System;
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
using Serial_Communication_WPF.Models;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml;
using System.Xml.Serialization;




namespace Serial_Communication_WPF
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    /// 



    public partial class Window1 : Window
    {
        SerialPort ComPort = new SerialPort();
        string recieved_data;
      

        string OutputText;






        /// <summary>
        /// Raised when a property on this object has a new value.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        private void serializer_UnknownNode(object sender, XmlNodeEventArgs e)
        {
            Console.WriteLine("Unknown Node:" + e.Name + "\t" + e.Text);
        }

        private void serializer_UnknownAttribute
        (object sender, XmlAttributeEventArgs e)
        {
            System.Xml.XmlAttribute attr = e.Attr;
            Console.WriteLine("Unknown attribute " +
            attr.Name + "='" + attr.Value + "'");
        }


        public Window1()
        {
            InitializeComponent();



            CommandClass[] Commands = new CommandClass[4];

            Commands[0] = new CommandClass();
            Commands[0].Command = "?";
            Commands[0].Description = "Help";
            Commands[0].ExpectedReply = "OK";
            Commands[0].ID = 0;
            
            Commands[1] = new CommandClass();
            Commands[1].Command = "K";
            Commands[1].Description = "Initialise";
            Commands[1].ExpectedReply = "OK";
            Commands[1].ID = 0;
            

            Commands[2] = new CommandClass();
            Commands[2].Command = "U";
            Commands[2].Description = "Update Screen";
            Commands[2].ExpectedReply = "OK";
            Commands[2].ID = 0;

            Commands[3] = new CommandClass();
            Commands[3].Command = "M";
            Commands[3].Description = "Check Events";
            Commands[3].ExpectedReply = "OK";
            Commands[3].ID = 0;


            //Serialise
                      string fileName = @"C:\Temp\Mahesh.txt";
            FileStream fs = File.Create(fileName);
            XmlSerializer serializer = new XmlSerializer(typeof(CommandClass[]));
            serializer.Serialize(fs,Commands);
            fs.Close();


            //Deserialise
            
                // Create an instance of the XmlSerializer class;
                // specify the type of object to be deserialized.
                XmlSerializer deserializer = new XmlSerializer(typeof(CommandClass[]));
                /* If the XML document has been altered with unknown
                nodes or attributes, handle them with the
                UnknownNode and UnknownAttribute events.*/
                deserializer.UnknownNode += new
                XmlNodeEventHandler(serializer_UnknownNode);

                deserializer.UnknownAttribute += new
                XmlAttributeEventHandler(serializer_UnknownAttribute);

                // A FileStream is needed to read the XML document.
                FileStream dfs = new FileStream(fileName, FileMode.Open);
            // Declare an object variable of the type to be deserialized.
            CommandClass[] deCommands = new CommandClass[4];
            /* Use the Deserialize method to restore the object's state with
            data from the XML document. */
           

            


            deCommands = (CommandClass[])serializer.Deserialize(dfs);
                
                foreach (CommandClass oi in deCommands)
                {
                Console.WriteLine("\t" +
                oi.Command + "\t" +
                oi.Description + "\t" +
                oi.ExpectedReply + "\t" +
                oi.ID);


                //nButton.Width = 100;
                //nButton.Height = 40;
                //nButton.Content = "hello cruel";


                Button lButton = new Button();
                // lButton.AddHandler()

                
                lButton.Content = oi.Description;
                this.stackPanelCommandButtons.Children.Add(lButton);

            }




            //Adding buttons to command panel
            //this.MyStackPanel.Children.Add(new Button());
            Button nButton = new Button();

            nButton.Width = 100;
            nButton.Height = 40;
            nButton.Content = "hello cruel";



            this.stackPanelCommandButtons.Children.Add(nButton);







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
            OutputBox.ScrollToEnd();
        }

      

        private void UpdateTerminal2(string text)
        {
            OutputBox2.Text = OutputBox2.Text + text;
            OutputBox2.ScrollToEnd();
        }




        private void WriteData(string text)
        {
            // Assign the value of the recieved_data to the RichTextBox.
          

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

                    

                    String sStringTx = "    TX->" + data + "\r\n";

                    Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal), sStringTx);
                    Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal2), sStringTx);


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
                    
                    Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal), "Failed to SEND" + data + "\n" + ex + "\n");
                    Dispatcher.Invoke(DispatcherPriority.Send, new UpdateUiTextDelegate(UpdateTerminal2), "Failed to SEND" + data + "\n" + ex + "\n");

                }
            }
            else
            {
            }
        }

        private void CmdClear_Click(object sender, RoutedEventArgs e)
        {

           
            OutputBox.Clear();
            OutputBox2.Clear();


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
            String sMessage = "";

            switch (myButton.Name)
            {
                case "cmdHelp":
                    sMessage = "?" + txtParameter1.Text;
                    break;

                case "cmdInitilise":
                    sMessage = "K" + txtParameter1.Text;
                                        break;

                case "cmdIdle":
                    sMessage = "I" + txtParameter1.Text;
                    break;

                    
                    

                case "cmdUpdateScreen":
                    sMessage = "N" + txtParameter1.Text;
                    break;

                    

                case "cmdCheckEvents":
                    sMessage = "M" + txtParameter1.Text;
                    break;

                    

                case "cmdUpdateOutputs":
                    sMessage = "O " + txtParameter1.Text;
                    break;
                    

                case "cmdDeleteMemory":
                    sMessage = "D" + txtParameter1.Text;
                    break;

                    

                case "cmdReadEvents":
                    sMessage = "0" + txtParameter1.Text;
                    break;
                    

                case "cmdAddEvent":
                    sMessage = "1" + txtParameter1.Text;
                    break;


                case "cmdDisableEvent":
                    sMessage = "2" + txtParameter1.Text;
                    break;
                    
                    

                case "cmdTraceON":
                    sMessage = "3" + txtParameter1.Text;
                    break;

                    

                case "cmdUpdateOutputOne":
                    sMessage = "4";
                    break;

                case "cmdWriteStatus":
                    sMessage = "5" + txtParameter1.Text;
                    break;

                    

                case "cmdSetRTC":
                    DateTime now = DateTime.Now;
                                       
                         sMessage = "6" + " " + now.ToString("ddMMyyyy HH:mm:ss");   //txtParameter1.Text;
                   
                    break;

                    

                case "cmdReadTime":
                    sMessage = "7" + txtParameter1.Text;
                    break;


                case "cmdCheckCurrentEvent":
                    sMessage = "8" + txtParameter1.Text;
                    break;

                    
                case "cmdTraceOff":
                    sMessage = "9" + txtParameter1.Text;
                    break;


            }
            SerialCmdSend(sMessage);
            myButton.Background = Brushes.Yellow;
        }

        private void CboComPortsAvailable_SelectionChanged()
        {

        }
    }

    }

