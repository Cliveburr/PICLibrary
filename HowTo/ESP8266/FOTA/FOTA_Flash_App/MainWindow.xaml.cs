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

            var udp = new UdpClient();

            var ip = new IPEndPoint(IPAddress.Parse("192.168.4.1"), 15556);
            var sendBuffer = new byte[] { 3, 2, 6, 7};

            udp.Send(sendBuffer, sendBuffer.Length, ip);

            //using (var tcp = new TcpClient())
            //{
            //    var ip = new IPEndPoint(IPAddress.Parse("192.168.1.103"), 15555);

            //    tcp.Connect(ip);

            //    var sendBuffer = new byte[4096];
            //    var rnd = new Random((int)DateTime.Now.Ticks);
            //    for (var i = 0; i < sendBuffer.Length; i++)
            //    {
            //        sendBuffer[i] = (byte)rnd.Next(0, 256);
            //    }

            //    var test = tcp.Client.Send(sendBuffer, sendBuffer.Length, SocketFlags.None);

            //    //var receiveBuffer = new byte[2];
            //    //var received = tcp.Client.Receive(receiveBuffer, SocketFlags.None);

            //    var a = 1;
            //}

            //upgrade_buffer = new byte[SPI_FLASH_SEC_SIZE];

            //var context = DataContext as MainContext;
            //var file_bytes = File.ReadAllBytes(context.User2Path);
            //var chunks = file_bytes
            //    .Select((x, i) => new { Index = i, Value = x })
            //    .GroupBy(x => x.Index / 1460)
            //    .Select(x => x.Select(v => v.Value).ToList())
            //    .ToList();

            //upgrade_length = (uint)file_bytes.Length;
            //upgrade_sector = 257;

            //foreach (var chunk in chunks)
            //{
            //    write_system_upgrade(chunk.ToArray(), (ushort)chunk.Count);
            //}
        }

        private const int SPI_FLASH_SEC_SIZE = 4096;
        uint upgrade_length;
        ushort upgrade_sector;
        byte[] upgrade_buffer;
        ushort upgrade_buffer_length;

        private void write_system_upgrade(byte[] data, ushort length)
        {
            ushort tlength = 0;
            ushort left = 0;

            if (upgrade_buffer_length + length > SPI_FLASH_SEC_SIZE)
            {
                tlength = (ushort)(SPI_FLASH_SEC_SIZE - upgrade_buffer_length);
                left = (ushort)(length - tlength);

                //os_memcpy(upgrade_buffer + upgrade_buffer_length, data, tlength);
                Buffer.BlockCopy(data, 0, upgrade_buffer, upgrade_buffer_length, tlength);
                upgrade_buffer_length += tlength;
            }
            else
            {
                //os_memcpy(upgrade_buffer + upgrade_buffer_length, data, length);
                Buffer.BlockCopy(data, 0, upgrade_buffer, upgrade_buffer_length, length);

                upgrade_buffer_length += length;
            }

            if (upgrade_buffer_length == SPI_FLASH_SEC_SIZE)
            {
                //spi_flash_erase_sector(upgrade_sector);
                //spi_flash_write(upgrade_sector * SPI_FLASH_SEC_SIZE, (uint32*)upgrade_buffer, SPI_FLASH_SEC_SIZE);

                upgrade_sector++;
                //os_memset(upgrade_buffer, 0, SPI_FLASH_SEC_SIZE);
                upgrade_buffer = new byte[SPI_FLASH_SEC_SIZE];
                upgrade_buffer_length = 0;
            }

            if (left > 0)
            {
                //os_memcpy(upgrade_buffer, data + tlength, left);
                Buffer.BlockCopy(data, tlength, upgrade_buffer, 0, left);
                upgrade_buffer_length += left;
            }

            upgrade_length -= length;

            if (upgrade_length == 0)
            {
                if (upgrade_buffer_length > 0)
                {
                    //os_printf("spi_flash_write: %u bytes for %u sector\n", upgrade_buffer_length, upgrade_sector);

                    //spi_flash_erase_sector(upgrade_sector);
                    //spi_flash_write(upgrade_sector * SPI_FLASH_SEC_SIZE, (uint32*)upgrade_buffer, SPI_FLASH_SEC_SIZE);
                }

                //system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
                //system_upgrade_reboot();
            }
        }

        private void btCheckDeviceUser_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var context = DataContext as MainContext;

                using (var tcp = new TcpClient())
                {
                    tcp.SendTimeout = 1000;
                    var ip = new IPEndPoint(IPAddress.Parse(context.DeviceIP), context.SendPort);

                    tcp.Connect(ip);

                    using (var mem = new MemoryStream())
                    using (var write = new BinaryWriter(mem))
                    {
                        write.Write((byte)5);     // 5 = request userbin_check

                        byte[] bytes = mem.ToArray();
                        tcp.Client.Send(bytes, bytes.Length, SocketFlags.None);
                    }

                    var receiveBuffer = new byte[1];
                    var received = tcp.Client.Receive(receiveBuffer, SocketFlags.None);

                    if (received != 1)
                        throw new Exception("Received wrong message!");

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
            var context = DataContext as MainContext;
            ushort flash_sector = 1;   // 0x001000 / 4096 = User 1 flash position / SPI_FLASH_SEC_SIZE

            try
            {
                SendFota(flash_sector, context.User1Path);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message);
            }

            spFlashUser1.IsEnabled = false;
            spFlashUser2.IsEnabled = true;
        }

        private void btFlashUser2_Click(object sender, RoutedEventArgs e)
        {
            var context = DataContext as MainContext;
            ushort flash_sector = 257;   // 0x101000 / 4096 = User 2 flash position / SPI_FLASH_SEC_SIZE

            try
            {
                SendFota(flash_sector, context.User2Path);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message);
            }

            spFlashUser1.IsEnabled = true;
            spFlashUser2.IsEnabled = false;
        }

        private void SendFota(ushort flash_sector, string file_path)
        {
            var context = DataContext as MainContext;


                var file_bytes = File.ReadAllBytes(file_path);
                var chunks = file_bytes
                    .Select((x, i) => new { Index = i, Value = x })
                    .GroupBy(x => x.Index / 1460)
                    .Select(x => x.Select(v => v.Value).ToList())
                    .ToList();

            using (var tcp = new TcpClient())
            {
                var ip = new IPEndPoint(IPAddress.Parse(context.DeviceIP), context.SendPort);

                tcp.Connect(ip);


                var startBuffer = new byte[7];
                startBuffer[0] = 6;   // code to start

                var sectorBuffer = BitConverter.GetBytes(flash_sector);
                Buffer.BlockCopy(sectorBuffer, 0, startBuffer, 1, 2);

                //var lengthBuffer = BitConverter.GetBytes((ushort)chunks.Count);
                //Buffer.BlockCopy(lengthBuffer, 0, startBuffer, 3, 2);

                var lengthBuffer = BitConverter.GetBytes((uint)file_bytes.Length);
                Buffer.BlockCopy(lengthBuffer, 0, startBuffer, 3, 4);

                tcp.Client.Send(startBuffer, startBuffer.Length, SocketFlags.None);

                foreach (var chunk in chunks)
                {
                    tcp.Client.Send(chunk.ToArray(), chunk.Count, SocketFlags.None);

                    var receiveBuffer = new byte[1];
                    var received = tcp.Client.Receive(receiveBuffer, SocketFlags.None);

                    if (receiveBuffer[0] == 86)
                    {
                        tcp.Close();
                        return;
                    }

                    if (receiveBuffer[0] != 85)
                        throw new Exception("Write to flash fail!");

                    //System.Threading.Thread.Sleep(10);
                    //var packages = chunk
                    //    .Select((x, i) => new { Index = i, Value = x })
                    //    .GroupBy(x => x.Index / 1024)
                    //    .Select(x => x.Select(v => v.Value).ToList())
                    //    .ToList();

                    //for (var i = 0; i < 4; i++)
                    //{
                    //    if (i < packages.Count)
                    //        tcp.Client.Send(packages[i].ToArray(), packages[i].Count, SocketFlags.None);
                    //    else
                    //        tcp.Client.Send(new byte[1], 1, SocketFlags.None);
                    //}
                }

                //var bytesWrited = 0;
                //using (var file = File.OpenRead(context.User2Path))
                //{
                //    var startBuffer = new byte[8];
                //    startBuffer[0] = 6;   // code to start

                //    var sectorBuffer = BitConverter.GetBytes(flash_sector);
                //    Buffer.BlockCopy(sectorBuffer, 0, startBuffer, 1, 2);

                //    var lengthBuffer = BitConverter.GetBytes((uint)file.Length);
                //    Buffer.BlockCopy(lengthBuffer, 0, startBuffer, 4, 4);

                //    tcp.Client.Send(startBuffer, startBuffer.Length, SocketFlags.None);



                //    while (bytesWrited < file.Length)
                //    {
                //        var left = file.Length - bytesWrited;
                //        var tlength = left < chunkSize ? left : chunkSize;

                //        var writeBuffer = new byte[tlength];
                //        var readed = file.Read(writeBuffer, 0, (int)tlength);

                //        tcp.Client.Send(writeBuffer, writeBuffer.Length, SocketFlags.None);

                //        bytesWrited += readed;
                //    }
                //}




                //var finishBuffer = new byte[1];
                //finishBuffer[0] = 8;   // code to start
                //tcp.Client.Send(finishBuffer, finishBuffer.Length, SocketFlags.None);
            }

            throw new Exception("Write to flash fail!");
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
