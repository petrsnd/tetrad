#include "pch.h"
#include "GameText.h"

#include <sstream>

// The Templated Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234235

using namespace Windows::UI::Xaml;
namespace TsDX
{

REGISTER_DEPENDENCY_PROPERTY( Platform::String, GameText, Font )
REGISTER_DEPENDENCY_PROPERTY( Windows::UI::Color, GameText, OutlineColor )
REGISTER_DEPENDENCY_PROPERTY( Windows::UI::Color, GameText, FillColor )
REGISTER_DEPENDENCY_PROPERTY( int, GameText, StrokeWidth )

static void OnTextChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ e)
{
    GameText^ gt = (GameText^)d;
    if ( gt != nullptr )
        gt->InvalidateMeasure();
}

Windows::UI::Xaml::DependencyProperty^ GameText::m_dpText = 
    Windows::UI::Xaml::DependencyProperty::Register( "Text", Platform::String::typeid, GameText::typeid, 
                                                     ref new PropertyMetadata( nullptr, ref new PropertyChangedCallback( &OnTextChanged ) ) );

GameText::GameText()
{
    DefaultStyleKey = "TsDX.GameText";
}
void GameText::OnApplyTemplate()
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
                        ref new Windows::UI::Xaml::SizeChangedEventHandler( this, &GameText::OnMapSurfaceSizeChanged );
        LoadFontAsync();
    }
}
Windows::Foundation::Size GameText::MeasureOverride(  Windows::Foundation::Size size )
{
    if ( m_dwFontFace != nullptr )
    {
        PrepareTextResources();
        return m_textSize;
    }
    return Windows::Foundation::Size( 0.0f, 0.0f );
}
Windows::Foundation::Size GameText::ArrangeOverride( Windows::Foundation::Size size )
{
    if ( m_textGeometry )
    {
        Windows::Foundation::Rect rect( 0.0f, 0.0f, m_textSize.Width, m_textSize.Height );
        m_borderElement->Arrange( rect );
        Draw();
        return m_textSize;
    }
    return Windows::Foundation::Size( 0.0f, 0.0f );
}
void GameText::OnMapSurfaceSizeChanged( Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e )
{
    if ( m_dwFontFace != nullptr )
    {
        Windows::Foundation::Rect rect( 0.0f, 0.0f, e->NewSize.Width, e->NewSize.Height );
        Draw();
    }
}
void GameText::PrepareTextResources()
{
    const std::wstring sText( Text->Data(), Text->Length() );

    m_textGeometry = DxUtil::Instance()->CreatePathGeometry();
    Microsoft::WRL::ComPtr< ID2D1GeometrySink > textGeometrySink;
    ThrowIfFailed( m_textGeometry->Open( &textGeometrySink ) );
    auto glyphIndices = GetGlyphIndicesForFontFace( m_dwFontFace, sText );
    ThrowIfFailed( m_dwFontFace->GetGlyphRunOutline( ConvertFontSizeToDIP( static_cast< float >( FontSize ) ),
                                                     &glyphIndices[0],
                                                     NULL,
                                                     NULL,
                                                     static_cast< UINT32 >( glyphIndices.size() ),
                                                     FALSE,
                                                     FALSE,
                                                     textGeometrySink.Get() ) );
    ThrowIfFailed( textGeometrySink->Close() );

    // Set sizes for the geometry
    D2D1_RECT_F textBounds = { 0 };
    ThrowIfFailed( m_textGeometry->GetBounds( NULL, &textBounds ) );
    m_textSize.Width = textBounds.right - textBounds.left + (StrokeWidth * 2) + 1;
    m_textSize.Height = textBounds.bottom - textBounds.top + (StrokeWidth * 2) + 1;
}
void GameText::CreateSurface()
{
    if ( m_borderElement != nullptr && m_textGeometry != nullptr )
    {
        int width = ( int )m_borderElement->ActualWidth;
        int height = ( int )m_borderElement->ActualHeight;
        m_dwImageSource = ref new DirectWriteImageSource( width, height, false, m_textGeometry, m_textSize.Width, m_textSize.Height );
    }
}
void GameText::Draw()
{
    if ( m_borderElement != nullptr && m_textGeometry != nullptr )
    {
        CreateSurface();
        if ( m_dwImageSource )
        {
            m_dwImageSource->BeginDraw();
            m_dwImageSource->Clear( Windows::UI::Colors::Transparent );
            m_dwImageSource->RenderText( OutlineColor, FillColor, static_cast< float >( StrokeWidth ) );
            m_dwImageSource->EndDraw();
            auto brush = ref new Windows::UI::Xaml::Media::ImageBrush();
            brush->ImageSource = m_dwImageSource;
            m_borderElement->Background = brush;
        }
    }
}
void GameText::LoadFontAsync()
{
    m_dwFontFace = nullptr;
    const std::wstring sFont( Font->Data(), Font->Length() );
    concurrency::create_task( [this, sFont]()
        {
            this->m_dwFontFace = DxUtil::Instance()->CreateFontFaceByName( sFont );
        } )
        .then( [this]()
            {
                this->TriggerRedraw();
            } );
}
void GameText::TriggerRedraw()
{
    Dispatcher->RunAsync( Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler( [this]()
        {
            this->InvalidateMeasure();
        } ) );
}

} // TsDX
