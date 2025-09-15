using System.Diagnostics;
using System.Security.Policy;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WebSocketSharp;
using Wpf.Ui.Appearance;
using Wpf.Ui.Controls;

namespace electronic_scale_win
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : FluentWindow
    {
        public MainWindow()
        {
            InitializeComponent();
            ApplicationThemeManager.Apply(this);
        }
        private WebSocket ws;

        private void connect_Click(object sender, RoutedEventArgs e)
        {
          


            ws = new WebSocket("ws://" + ip.Text + ":81");
            ws.OnMessage += (sender, e) => {

                weight.Dispatcher.Invoke(new Action(() => {
                    weight.Content = e.Data+"克";
                }));
            };
            ws.OnOpen += (sender, e) => {
                Debug.WriteLine(e.ToString());
            };
            ws.Connect();
        }
    }
}