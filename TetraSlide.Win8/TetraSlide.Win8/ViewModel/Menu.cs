using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace TetraSlide.Win8.ViewModel
{

    class MenuCommandItem
    {
        public string Title { get; set; }
        public string Subtitle { get; set; }
        public ICommand Command { get; set; }
        public string CommandParameter { get; set; }
    }

    class Menu : DependencyObject
    {
        public Menu()
        {
            
        }

        ObservableCollection<MenuCommandItem> _Commands = new ObservableCollection<MenuCommandItem>();
        public ObservableCollection<MenuCommandItem> Commands
        {
            get
            {
                return _Commands;
            }
        }
    }
}
