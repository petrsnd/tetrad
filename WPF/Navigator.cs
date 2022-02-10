using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Collections.ObjectModel;
using TetraSlide.Api.Models;

namespace WPF
{
    public class Navigator : DependencyObject
    {
        public Navigator()
        {
            App.Mine.Model.LoggedIn += new EventHandler<LoggedInEventArgs>(Model_LoggedIn);
        }

        void Model_LoggedIn(object sender, LoggedInEventArgs e)
        {
            this.MyAccount = e.Account;
        }

        public event EventHandler<ViewPoppedEventArgs> ViewPopped;
        Stack<UserControl> viewStack = new Stack<UserControl>();
        

        public void SetBackView(UserControl view)
        {
            viewStack.Clear();
            viewStack.Push(view);
            UpdateBackButton();
        }

        public void PushCurrentView(UserControl view)
        {
            if (CurrentView != null)
                viewStack.Push(CurrentView);
            CurrentView = view;
            UpdateBackButton();
        }

        public void PopCurrentView()
        {
            ViewPoppedEventArgs args = new ViewPoppedEventArgs(CurrentView);
            if (viewStack.Count > 0)
            {
                UserControl last = viewStack.Pop();
                while (last is MessageView)
                    last = viewStack.Pop();
                if (last != null)
                    CurrentView = last;
                if (ViewPopped != null)
                    ViewPopped(this, args);
            }
            UpdateBackButton();
        }

        public void SetCurrentView(UserControl view)
        {
            viewStack.Clear();
            if (!(view is MainMenu))
            {
                viewStack.Push(new MainMenu());
            }
            CurrentView = view;
            UpdateBackButton();
        }

        void UpdateBackButton()
        {
            BackButtonVisibility = viewStack.Count > 0 ? Visibility.Visible : Visibility.Hidden;
        }

        public string MuteButtonText
        {
            get { return (string)GetValue(MuteButtonTextProperty); }
            set { SetValue(MuteButtonTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for MuteButtonText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MuteButtonTextProperty =
            DependencyProperty.Register("MuteButtonText", typeof(string), typeof(Navigator), new PropertyMetadata("Mute"));
 

        public Account MyAccount
        {
            get { return (Account)GetValue(MyAccountProperty); }
            set { SetValue(MyAccountProperty, value); }
        }

        // Using a DependencyProperty as the backing store for MyAccount.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MyAccountProperty =
            DependencyProperty.Register("MyAccount", typeof(Account), typeof(Navigator), new UIPropertyMetadata(
                new PropertyChangedCallback( (obj, arg) => 
                {
                    Navigator nav = obj as Navigator;
                    Account acc = arg.NewValue as Account;
                    if (acc == null)
                    {
                        nav.LogonText = "Not logged in.";
                    }
                    else
                    {
                        nav.LogonText = "Logged in as: " + acc.name + " (" + acc.email_address + ") ";
                    }
                })));



        public string LogonText
        {
            get { return (string)GetValue(LogonTextProperty); }
            set { SetValue(LogonTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for LogonText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty LogonTextProperty =
            DependencyProperty.Register("LogonText", typeof(string), typeof(Navigator), new UIPropertyMetadata(""));




        public bool IsMuted
        {
            get { return (bool)GetValue(IsMutedProperty); }
            set { SetValue(IsMutedProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsMuted.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsMutedProperty =
            DependencyProperty.Register("IsMuted", typeof(bool), typeof(Navigator), new UIPropertyMetadata( 
                new PropertyChangedCallback( (obj, args) =>
                {
                    Navigator nav = obj as Navigator;
                    bool muted = (bool)args.NewValue;
                    SoundManager.Instance().Muted = muted;
                    if (muted)
                        nav.MuteButtonText = "Rock On";
                    else
                        nav.MuteButtonText = "Mute";
                })));

        

        public Visibility BackButtonVisibility
        {
            get { return (Visibility)GetValue(BackButtonVisibilityProperty); }
            set { SetValue(BackButtonVisibilityProperty, value); }
        }

        // Using a DependencyProperty as the backing store for BackEnabled.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty BackButtonVisibilityProperty =
            DependencyProperty.Register("BackButtonVisibility", typeof(Visibility), typeof(Main), new UIPropertyMetadata(Visibility.Hidden));

        private void OnBack(object sender, RoutedEventArgs e)
        {
            PopCurrentView();
        }


        public UserControl CurrentView
        {
            get { return (UserControl)GetValue(CurrentViewProperty); }
            set { SetValue(CurrentViewProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Subview.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentViewProperty =
            DependencyProperty.Register("CurrentView", typeof(UserControl), typeof(Navigator));

        RelayCommand m_backCommand = null;
        public RelayCommand BackCommand
        {
            get
            {
                if( m_backCommand != null ) return m_backCommand;
                m_backCommand = new RelayCommand(
                    param => 
                    {
                        PopCurrentView();
                        UpdateBackButton();
                    },
                    param =>
                    {
                        return viewStack.Count > 0;
                    } );
                return m_backCommand;
            }
        }

        RelayCommand m_toggleMute = null;
        public RelayCommand ToggleMute
        {
            get
            {
                if (m_toggleMute != null) return m_toggleMute;
                m_toggleMute = new RelayCommand(
                    param =>
                    {
                        this.IsMuted = !this.IsMuted;
                    });
                return m_toggleMute;
            }
        }
    }
}
