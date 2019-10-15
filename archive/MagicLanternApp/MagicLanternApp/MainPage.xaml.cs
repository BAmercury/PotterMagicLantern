using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;


namespace MagicLanternApp
{
    // Learn more about making custom code visible in the Xamarin.Forms previewer
    // by visiting https://aka.ms/xamarinforms-previewer
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {

        private int default_hue = 899;
        public MainPage()
        {
            InitializeComponent();
            cntrl_slider.Value = default_hue;
        }

        private void Cntrl_slider_ValueChanged(object sender, ValueChangedEventArgs e)
        {


        }

        private void Cntrl_connect_Clicked(object sender, EventArgs e)
        {
            int result = DependencyService.Get<IPrintMeService>().printme();
            Debug.WriteLine("Test");
            Debug.WriteLine(result.ToString());
        }

        private void Cntrl_cast_Clicked(object sender, EventArgs e)
        {

        }

        private void Cntrl_animation_cycle_Clicked(object sender, EventArgs e)
        {
            
        }

    }
}
