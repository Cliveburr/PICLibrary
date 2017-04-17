using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
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
using System.Xml;
using System.Xml.Serialization;

namespace FOTA_Flash_App
{
    public partial class MainWindow : Window
    {
        public string CONFIG_FILE = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "FOTA_Flash_app.config.xml");

        public MainWindow()
        {
            InitializeComponent();

            LoadConfig();
        }

        private void btCheckDeviceUser_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var context = DataContext as MainContext;

                using (var tcp = new TcpClient())
                {
                    var ip = new IPEndPoint(IPAddress.Parse(context.DeviceIP), context.SendPort);

                    tcp.Connect(ip);

                    using (var mem = new MemoryStream())
                    using (var write = new BinaryWriter(mem))
                    {
                        write.Write((byte)5);     // 5 = request userbin_check

                        byte[] bytes = mem.ToArray();
                        tcp.Client.Send(bytes, bytes.Length, SocketFlags.None);
                    }

                    var receiveBuffer = new byte[10];
                    var received = tcp.Client.Receive(receiveBuffer, SocketFlags.None);

                    if (received != 1)
                        throw new Exception("Received message wrong!");

                    if (receiveBuffer[0] == 1)
                    {
                        spFlashUser1.IsEnabled = true;
                        spFlashUser2.IsEnabled = false;
                    }
                    else
                    {
                        spFlashUser1.IsEnabled = false;
                        spFlashUser2.IsEnabled = true;
                    }
                }

                gbDeviceConnect.IsEnabled = false;
                gbUserFlash.IsEnabled = true;

            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void LoadConfig()
        {
            if (System.IO.File.Exists(CONFIG_FILE))
            {
                using (var stream = System.IO.File.OpenRead(CONFIG_FILE))
                {
                    var xml = new XmlSerializer(typeof(MainContext));
                    DataContext = xml.Deserialize(stream) as MainContext;
                }
            }
            else
            {
                DataContext = new MainContext
                {
                    DeviceIP = "192.168.0.1",
                    User1Path = @"D:\user1.bin",
                    User2Path = @"D:\user2.bin",
                    SendPort = 15555,
                    ReceivePort = 15556
                };
            }
        }

        private void SaveConfig()
        {
            using (var stream = System.IO.File.OpenWrite(CONFIG_FILE))
            {
                var xml = new XmlSerializer(typeof(MainContext));
                xml.Serialize(stream, DataContext);
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            SaveConfig();
        }

        private void btFlashUser1_Click(object sender, RoutedEventArgs e)
        {

            spFlashUser1.IsEnabled = false;
            spFlashUser2.IsEnabled = true;

        }

        private void btFlashUser2_Click(object sender, RoutedEventArgs e)
        {

            spFlashUser1.IsEnabled = true;
            spFlashUser2.IsEnabled = false;

        }

        private void btDisconnect_Click(object sender, RoutedEventArgs e)
        {
            gbDeviceConnect.IsEnabled = true;
            gbUserFlash.IsEnabled = false;
        }
    }

    public class MainContext
    {
        public string DeviceIP { get; set; }
        public string User1Path { get; set; }
        public string User2Path { get; set; }
        public int SendPort { get; set; }
        public int ReceivePort { get; set; }
    }
}
