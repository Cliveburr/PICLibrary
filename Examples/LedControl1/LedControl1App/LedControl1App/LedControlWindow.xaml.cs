using System;
using System.Windows;
using UsbHidLibrary;

namespace LedControl1App
{
    /// <summary>
    /// Interaction logic for LedControlWindow.xaml
    /// </summary>
    public partial class LedControlWindow : Window
    {
        public HidDevice Device { get; set; }

        public LedControlWindow()
        {
            InitializeComponent();
        }

        private void btChangeUSBControl_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var msg = new byte[13];
                msg[0] = 10;

                var bs = BitConverter.GetBytes(int.Parse(tbRedOn.Text));
                msg[1] = bs[0];
                msg[2] = bs[1];
                bs = BitConverter.GetBytes(int.Parse(tbRedOff.Text));
                msg[3] = bs[0];
                msg[4] = bs[1];
                bs = BitConverter.GetBytes(int.Parse(tbGreenOn.Text));
                msg[5] = bs[0];
                msg[6] = bs[1];
                bs = BitConverter.GetBytes(int.Parse(tbGreenOff.Text));
                msg[7] = bs[0];
                msg[8] = bs[1];
                bs = BitConverter.GetBytes(int.Parse(tbBlueOn.Text));
                msg[9] = bs[0];
                msg[10] = bs[1];
                bs = BitConverter.GetBytes(int.Parse(tbBlueOff.Text));
                msg[11] = bs[0];
                msg[12] = bs[1];

                Device.Write(msg);

            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message);
            }
        }
    }
}