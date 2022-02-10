#include "pch.h"
#include "DirectWriteImageSource.h"
#include "DxHelper.h"


namespace TsDX
{

DirectWriteImageSource::DirectWriteImageSource( int pixelWidth, int pixelHeight, bool isOpaque,
                                                Microsoft::WRL::ComPtr< ID2D1PathGeometry >& textGeometry, float textWidth, float textHeight ) :
    DxImageSourceBase( pixelWidth, pixelHeight, isOpaque ),
    m_textGeometry( textGeometry ),
    m_textWidth( textWidth ),
    m_textHeight( textHeight )
{
    CreateDeviceIndependentResources();
    CreateDeviceResources();
}
void DirectWriteImageSource::BeginDraw()
{
    BeginDraw( Windows::Foundation::Rect( 0, 0, m_textWidth, m_textHeight ) );
}
void DirectWriteImageSource::RenderText( Windows::UI::Color outlineColor, Windows::UI::Color fillColor, float strokeWidth )
{
    Microsoft::WRL::ComPtr< ID2D1SolidColorBrush > outlineBrush;
    ThrowIfFailed( m_d2dContext->CreateSolidColorBrush( ConvertUiColorToD2D1ColorF( outlineColor ), &outlineBrush ) );
    Microsoft::WRL::ComPtr< ID2D1SolidColorBrush > fillBrush;
    ThrowIfFailed( m_d2dContext->CreateSolidColorBrush( ConvertUiColorToD2D1ColorF( fillColor ), &fillBrush ) );

    m_d2dContext->SetTransform( D2D1::Matrix3x2F::Translation( strokeWidth, m_textHeight - strokeWidth ) );
    m_d2dContext->DrawGeometry( m_textGeometry.Get(), outlineBrush.Get(), strokeWidth );
    m_d2dContext->FillGeometry( m_textGeometry.Get(), fillBrush.Get() );
}

} // TsDX
