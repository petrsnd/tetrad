#include "pch.h"
#include "DxTiledBitmapImageSource.h"
#include "DxHelper.h"


namespace TsDX
{

DxTiledBitmapImageSource::DxTiledBitmapImageSource( int pixelWidth, int pixelHeight, bool isOpaque,
                                                    Microsoft::WRL::ComPtr< IWICBitmapSource > sourceImage ) :
    DxImageSourceBase( pixelWidth, pixelHeight, isOpaque ),
    m_sourceImage( sourceImage )
{}
void DxTiledBitmapImageSource::BeginDraw()
{
    BeginDraw( Windows::Foundation::Rect( 0, 0, static_cast< float >( m_pixelWidth ), static_cast< float >( m_pixelHeight ) ) );
}
void DxTiledBitmapImageSource::RenderTiledImage()
{
    Microsoft::WRL::ComPtr< ID2D1Bitmap1 > bitmap;
    ThrowIfFailed( m_d2dContext->CreateBitmapFromWicBitmap( m_sourceImage.Get(), &bitmap ) );
    Microsoft::WRL::ComPtr< ID2D1BitmapBrush1 > bitmapBrush;
    D2D1_BITMAP_BRUSH_PROPERTIES1 bitmapBrushProps = { D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP, D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR };
    ThrowIfFailed( m_d2dContext->CreateBitmapBrush( bitmap.Get(), bitmapBrushProps, &bitmapBrush ) );
    D2D1_RECT_F rect = D2D1::RectF( 0.0f, 0.0f, static_cast< float >( m_pixelWidth ), static_cast< float >( m_pixelHeight ) );
    m_d2dContext->FillRectangle( rect, bitmapBrush.Get() );
}

} // TsDX
