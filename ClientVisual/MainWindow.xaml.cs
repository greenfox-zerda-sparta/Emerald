using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Net;
using System.Net.Sockets;

namespace Devices
{
    public partial class MainWindow : Window
    {
        private Socket server;
        private byte[] bytes;
        private string _nameOfUser;
        private bool connected;
        private string serverIP = "10.27.6.228";
        public Thread thread;
        ManualResetEvent pause = new ManualResetEvent(true);
        ManualResetEvent shutdown = new ManualResetEvent(false);
        private int selectedShipLength = 0;
        private bool isHorizontal = false;
        Button[,] buttonArray;
        Button[,] enemyButtonArray;

        public MainWindow()
        {
            InitializeComponent();
            bytes = new byte[1024];
            _nameOfUser = "";
            connected = false;
            Connect();
            thread = new Thread(OnMsgReceive);
            thread.IsBackground = true;
            thread.Start();
            buttonArray = new Button[,] { 
                            { a00, a01, a02, a03, a04, a05, a06, a07, a08, a09 },
                            { a10, a11, a12, a13, a14, a15, a16, a17, a18, a19 },
                            { a20, a21, a22, a23, a24, a25, a26, a27, a28, a29 },
                            { a30, a31, a32, a33, a34, a35, a36, a37, a38, a39 },
                            { a40, a41, a42, a43, a44, a45, a46, a47, a48, a49 },
                            { a50, a51, a52, a53, a54, a55, a56, a57, a58, a59 },
                            { a60, a61, a62, a63, a64, a65, a66, a67, a68, a69 },
                            { a70, a71, a72, a73, a74, a75, a76, a77, a78, a79 },
                            { a80, a81, a82, a83, a84, a85, a86, a87, a88, a89 },
                            { a90, a91, a92, a93, a94, a95, a96, a97, a98, a99 } };

          
        }

        public void Connect()
        {
            while (!connected)
            {
                try
                {
                    IPHostEntry ipHostInfo = Dns.Resolve(serverIP);
                    IPAddress ipAddress = ipHostInfo.AddressList[0];
                    IPEndPoint remoteEP = new IPEndPoint(ipAddress, 1234);

                    server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                    try
                    {
                        server.Connect(remoteEP);

                        textBlock.Text += $"Socket connected to {server.RemoteEndPoint.ToString()}";
                        connected = true;

                    }
                    catch (ArgumentNullException ane)
                    {
                        //Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
                    }
                    catch (SocketException se)
                    {
                        //Console.WriteLine("SocketException : {0}", se.ToString());
                    }
                    catch (Exception e)
                    {
                        //Console.WriteLine("Unexpected exception : {0}", e.ToString());
                    }
                }
                catch (Exception e)
                {
                    //Console.WriteLine(e.ToString());
                }
            }

            if (connected)
            {
                if (server.Available > 0)
                {
                    int bytesRec = server.Receive(bytes);
                    textBlock.Text += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                    _nameOfUser = textBox.Text;
                    Send(_nameOfUser);
                }
            }
        }

        private void Send(string message)
        {
            message += "\n";
            try
            {
                byte[] msg = Encoding.ASCII.GetBytes(message);
                int bytesSent = server.Send(msg);
            }
            catch (ArgumentNullException ane)
            {
                //Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
            }
            catch (SocketException se)
            {
                //Console.WriteLine("SocketException : {0}", se.ToString());
            }
            catch (Exception e)
            {
                //Console.WriteLine("Unexpected exception : {0}", e.ToString());
            }
        }

        private void Receive()
        {
            try
            {
                while (true)
                {
                    int bytesRec = server.Receive(bytes);
                    textBlock.Text += Encoding.ASCII.GetString(bytes, 0, bytesRec);
                }

                //Console.Write(Encoding.ASCII.GetString(bytes, 0, bytesRec));
            }
            catch (ArgumentNullException ane)
            {
                //Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
            }
            catch (SocketException se)
            {
                //Console.WriteLine("SocketException : {0}", se.ToString());
            }
            catch (Exception e)
            {
                //Console.WriteLine("Unexpected exception : {0}", e.ToString());
            }
        }

        ~MainWindow()
        {
            try
            {
                server.Shutdown(SocketShutdown.Both);
                server.Close();
                shutdown.Set();
            }
            catch (ArgumentNullException ane)
            {
                //Console.WriteLine("ArgumentNullException : {0}", ane.ToString());
            }
            catch (SocketException se)
            {
                //Console.WriteLine("SocketException : {0}", se.ToString());
            }
            catch (Exception e)
            {
                //Console.WriteLine("Unexpected exception : {0}", e.ToString());
            }
        }


        private void OnMsgReceive()
        {
            while (true)
            {
                int bytesRec = this.server.Receive(bytes);

                if (bytesRec > 0)
                {
                    string data = Encoding.ASCII.GetString(bytes, 0, bytesRec);
                    if (data[0] == '/' && data[1] == '&' && data[2] == '~')
                    {
                        Dispatcher.Invoke(() =>
                        {
                            setCoordinate(bytesRec, data);
                        });
                    }
                    else if(data[0] == '#' && data[1] == '>' && data[2] == '<')
                    {
                        Dispatcher.Invoke(() =>
                        {
                            registerEnemyHit(bytesRec, data);
                        });
                    }
                    else
                    {
                        Dispatcher.Invoke(() =>
                        {
                            setText(bytesRec);
                        });
                    }
                }
            }
        }

        private void setText(int bytesRec)
        {
            textBlock.Text += "\n" + Encoding.ASCII.GetString(bytes, 0, bytesRec);
        }

        private void registerEnemyHit(int bytesRec, string dataIn)
        {
            int x = dataIn[4] - '0';
            int y = dataIn[5] - '0';
            if (!enemyButtonArray[x, y].IsEnabled)
            {
                if(dataIn[6] == 'h')
                {
                    enemyButtonArray[x, y].Foreground = Brushes.Red;
                    enemyButtonArray[x, y].Content = "ON";
                }
                else if (dataIn[6] == 'm')
                {
                    enemyButtonArray[x, y].Foreground = Brushes.Black;
                    enemyButtonArray[x, y].Content = "OFF";
                }
            }
        }
        private void setCoordinate(int bytesRec, string dataIn)
        {
            int x = dataIn[4] - '0';
            int y = dataIn[5] - '0';
            if(!buttonArray[x, y].IsEnabled)
            {
                buttonArray[x, y].Foreground = Brushes.Red;
                buttonArray[x, y].Content = "ON";
                string msg = "#><";
                msg += buttonArray[x, y].Name.ToString() + "h";
                Send(msg);
            }
            else
            {
                buttonArray[x, y].Foreground = Brushes.Black;
                buttonArray[x, y].Content = "OFF";
                string msg = "#><";
                msg += buttonArray[x, y].Name.ToString() + "m";
                Send(msg);
            }
        }

        private void sendButton_Click(object sender, RoutedEventArgs e)
        {
            string message;
            message = "";
            message = textBox.Text;
            textBlock.Text += "\n Your message: " + textBox.Text;
            textBox.Text = "";
            Send(message);
        }

        private void OnFieldButtonClick(object sender, RoutedEventArgs e)
        {
            int x, y;
            Button thisButton = (Button)sender;
            x = thisButton.Name[1] - '0';
            y = thisButton.Name[2] - '0';
            ShipPlacementChecker checker = new ShipPlacementChecker(buttonArray);
            if (isHorizontal && checker.IsSelectedHorizontalPlaceEmpty(x, y, selectedShipLength))
            {
                checker.HorizontalPlaceTheShip(x, y, selectedShipLength);
                buttonArray = checker.Board;                
            }
            else if (!isHorizontal && checker.IsSelectedVerticalPlaceEmpty(x, y, selectedShipLength))
            {
                checker.VerticalPlaceTheShip(x, y, selectedShipLength);
                buttonArray = checker.Board;
            }
            else
            {
                buttonArray = checker.Board;
            }
        }

        private void carrier_Click(object sender, RoutedEventArgs e)
        {
            selectedShipLength = 5;
        }

        private void battleship_Click(object sender, RoutedEventArgs e)
        {
            selectedShipLength = 4;
        }

        private void cruiser_Click(object sender, RoutedEventArgs e)
        {
            selectedShipLength = 3;
        }

        private void submarine_Click(object sender, RoutedEventArgs e)
        {
            selectedShipLength = 3;
        }

        private void destroyer_Click(object sender, RoutedEventArgs e)
        {
            selectedShipLength = 2;
        }

        private void horOrVerButton_Click(object sender, RoutedEventArgs e)
        {
            Button thisButton = (Button)sender;
            if (thisButton.Content.ToString() == "Horizontal")
            {
                isHorizontal = true;
            }
            else if (thisButton.Content.ToString() == "Vertical")
            {
                isHorizontal = false;
            }
        }

        private void OnEnemyFieldButtonClick(object sender, RoutedEventArgs e)
        {
            Button thisButton = (Button)sender;
            thisButton.FontSize = 16;
            string msg = "/&~";
            msg += thisButton.Name.ToString();
            thisButton.IsEnabled = false;
            Send(msg);
        }
    }
}
