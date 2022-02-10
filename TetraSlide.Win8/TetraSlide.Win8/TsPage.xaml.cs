using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234237

namespace TetraSlide.Win8
{
    /// <summary>
    /// A basic page that provides characteristics common to most applications.
    /// </summary>
    public sealed partial class TsPage : TetraSlide.Win8.Common.LayoutAwarePage
    {
        public TsPage()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Populates the page with content passed during navigation.  Any saved state is also
        /// provided when recreating a page from a prior session.
        /// </summary>
        /// <param name="navigationParameter">The parameter value passed to
        /// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested.
        /// </param>
        /// <param name="pageState">A dictionary of state preserved by this page during an earlier
        /// session.  This will be null the first time a page is visited.</param>
        protected override void LoadState(Object navigationParameter, Dictionary<String, Object> pageState)
        {
            this.DefaultViewModel["Title"] = View.TitleControl.Instance;
            this.DefaultViewModel["SnappedTitle"] = View.SnappedTitleControl.Instance;

            IPageContent content = (navigationParameter as IPageContent);
            this.DefaultViewModel["Content"] = content.Content;
            this.DefaultViewModel["SnappedContent"] = content.SnappedContent;
            this.DataContext = this.DefaultViewModel;
        }

        protected override void GoBack(object sender, RoutedEventArgs e)
        {
            if (DefaultViewModel["Content"] is View.GameBoard ||
                DefaultViewModel["Content"] is View.GameSnapped)
            {
                if (App.Mine.Game.IsGameOver)
                {
                    this.GoHome(sender, e);
                    return;
                }
                App.Mine.Game.GameSuspended += async (s, a) =>
                {
                    await this.Dispatcher.RunAsync(
                        Windows.UI.Core.CoreDispatcherPriority.Normal,
                        () => {
                            this.GoHome(sender, e);
                        });
                };
                App.RootFrame.Navigate(typeof(View.WaitPage), "Suspending the game...");
                App.Mine.Game.Suspend();
            }
            else
            {
                base.GoBack(sender, e);
            }
        }
    }
}
