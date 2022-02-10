using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace TetraSlide.Win8
{
    class PageContent : DependencyObject, IPageContent
    {
        public PageContent(DependencyObject viewModel, FrameworkElement content, FrameworkElement snappedContent)
        {
            ViewModel = viewModel;
            Content = content;
            SnappedContent = snappedContent;
            Content.DataContext = viewModel;
            SnappedContent.DataContext = viewModel;
        }

        public FrameworkElement Title 
        {
            get{ return View.TitleControl.Instance;}
        }
        public FrameworkElement SnappedTitle
        {
            get { return View.SnappedTitleControl.Instance; }
        }

        public FrameworkElement Content
        {
            get { return (FrameworkElement)GetValue(ContentProperty); }
            set { SetValue(ContentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Content.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ContentProperty =
            DependencyProperty.Register("Content", typeof(FrameworkElement), typeof(PageContent), null);



        public FrameworkElement SnappedContent
        {
            get { return (FrameworkElement)GetValue(SnappedContentProperty); }
            set { SetValue(SnappedContentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for SnappedContent.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SnappedContentProperty =
            DependencyProperty.Register("SnappedContent", typeof(FrameworkElement), typeof(PageContent), null);


        public DependencyObject ViewModel
        {
            get { return (DependencyObject)GetValue(ViewModelProperty); }
            set { SetValue(ViewModelProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ViewModel.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ViewModelProperty =
            DependencyProperty.Register("ViewModel", typeof(DependencyObject), typeof(PageContent), null);

    }
}
