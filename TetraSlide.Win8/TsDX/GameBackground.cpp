#include "pch.h"
#include "GameText.h"

// The Templated Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234235

namespace TsDX
{

REGISTER_DEPENDENCY_PROPERTY( Platform::String, GameBackground, Image )

GameBackground::GameBackground()
{
    DefaultStyleKey = "TsDX.GameBackground";
}
void GameBackground::OnApplyTemplate()
{
    if ( m_borderElement != nullptr )
    {
        m_borderElement->SizeChanged -= m_sizeChangedToken;
        m_borderElement = nullptr;
    }
    m_borderElement = ( Windows::UI::Xaml::Controls::Border^ )GetTemplateChild( "DrawSurface" );
    if ( m_borderElement != nullptr )
    {
        m_sizeChangedToken = m_borderElement->SizeChanged +=
                        ref new Windows::UI::Xaml::SizeChangedEventHandler( this, &GameBackground::OnMapSurfaceSizeChanged );
        LoadImageAsync();
    }
}
Windows::Foundation::Size GameBackground::MeasureOverride( Windows::Foundation::Size size )
{
    if ( m_wicBitmapSource != nullptr )
        return size;
    return Windows::Foundation::Size( 0.0f, 0.0f );
}
Windows::Foundation::Size GameBackground::ArrangeOverride( Windows::Foundation::Size size )
{
    if ( m_wicBitmapSource != nullptr )
    {
        Windows::Foundation::Rect rect( 0.0f, 0.0f, size.Width, size.Height );
        m_borderElement->Arrange( rect );
        Draw();
        return size;
    }
    return Windows::Foundation::Size( 0.0f, 0.0f );
}
void GameBackground::OnMapSurfaceSizeChanged( Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e )
{
    if ( m_wicBitmapSource != nullptr )
    {
        Windows::Foundation::Rect rect( 0.0f, 0.0f, e->NewSize.Width, e->NewSize.Height );
        Draw();
    }
}
void GameBackground::CreateSurface()
{
    if ( m_borderElement != nullptr && m_wicBitmapSource != nullptr )
    {
        int width = ( int )m_borderElement->ActualWidth;
        int height = ( int )m_borderElement->ActualHeight;
        m_tiledImageSource = ref new DxTiledBitmapImageSource( width, height, false, m_wicBitmapSource );
    }
}
void GameBackground::Draw()
{
    if ( m_borderElement != nullptr && m_wicBitmapSource != nullptr )
    {
        CreateSurface();
        if ( m_tiledImageSource )
        {
            m_tiledImageSource->BeginDraw();
            m_tiledImageSource->Clear( Windows::UI::Colors::Transparent );
            m_tiledImageSource->RenderTiledImage();
            m_tiledImageSource->EndDraw();
            auto brush = ref new Windows::UI::Xaml::Media::ImageBrush();
            brush->ImageSource = m_tiledImageSource;
            m_borderElement->Background = brush;
        }
    }
}
void GameBackground::LoadImageAsync()
{
    m_wicBitmapSource = nullptr;
    const std::wstring sImage( Image->Data(), Image->Length() );
    concurrency::create_task( [this, sImage]()
        {
            this->m_wicBitmapSource = DxUtil::Instance()->GetWICBitmapSourceFromFile( sImage );
        } )
        .then( [this]()
            {
                this->TriggerRedraw();
            } );
}
void GameBackground::TriggerRedraw()
{
    Dispatcher->RunAsync( Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler( [this]()
        {
            this->InvalidateMeasure();
        } ) );
}

} // TsDX
