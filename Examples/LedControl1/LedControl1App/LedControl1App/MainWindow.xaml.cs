using System.Linq;
using System.Windows;
using UsbHidLibrary;

namespace LedControl1App
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private const int _vendorId = 0x1781;
        private const int _productId = 0x07D0;
        private HidDevice _device;
        private UsbMonitor _monitor;

        private LedControlWindow _ledControlWindow;

        public MainWindow()
        {
            InitializeComponent();

            _monitor = new UsbMonitor();
            _monitor.UsbChangeEventArrival += new UsbMonitor.UsbChangeEventHandler(monitor_Arrival);
            _monitor.UsbChangeEventRemoved += new UsbMonitor.UsbChangeEventHandler(monitor_Removed);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            IsConnected();
        }

        private void monitor_Arrival()
        {
            IsConnected();
        }

        private void monitor_Removed()
        {
            if (_device != null)
            {
                var devices = HidDevice.Enumerate()
                    .Select(de => new HidDevice(de));

                var connected = devices
                    .Where(d => d.Attributes != null)
                    .FirstOrDefault(d => d.Attributes.VendorId == _vendorId && d.Attributes.ProductId == _productId);

                if (connected == null)
                {
                    _device.Dispose();
                    _device = null;
                    CloseControlWindow();
                }
            }
        }

        public void IsConnected()
        {
            if (_device == null)
            {
                var devices = HidDevice.Enumerate()
                    .Select(de => new HidDevice(de));

                var connected = devices
                    .Where(d => d.Attributes != null)
                    .FirstOrDefault(d => d.Attributes.VendorId == _vendorId && d.Attributes.ProductId == _productId);

                if (connected != null)
                {
                    _device = connected;
                    OpenControlWindow();
                }
            }
        }

        private void OpenControlWindow()
        {
            if (_ledControlWindow == null)
            {
                _ledControlWindow = new LedControlWindow();
                _ledControlWindow.Device = _device;
                _ledControlWindow.Closed += _ledControlWindow_Closed;
                _ledControlWindow.Show();
            }
        }

        private void _ledControlWindow_Closed(object sender, System.EventArgs e)
        {
            if (_device != null)
            {
                _device.Dispose();
                _device = null;
            }
            _ledControlWindow = null;
        }

        private void CloseControlWindow()
        {
            if (_ledControlWindow != null)
            {
                _ledControlWindow.Close();
            }
        }
    }
}
