#include "pch.h"
#include "DxImageSourceBase.h"
#include "DxHelper.h"


namespace TsDX
{

DxImageSourceBase::DxImageSourceBase( int pixelWidth, int pixelHeight, bool isOpaque ) :
    SurfaceImageSource( pixelWidth, pixelHeight, isOpaque ),
    m_pixelWidth( pixelWidth ),
    m_pixelHeight( pixelHeight )
{
    CreateDeviceIndependentResources();
    CreateDeviceResources();
}
void DxImageSourceBase::BeginDraw( Windows::Foundation::Rect updateRect )
{
    POINT offset;
    Microsoft::WRL::ComPtr< IDXGISurface > surface;

    RECT updateRectNative = ConvertFoundationRectToNativeRect( updateRect );
    HRESULT beginDrawHR = m_sisNative->BeginDraw( updateRectNative, &surface, &offset );
    if ( beginDrawHR == DXGI_ERROR_DEVICE_REMOVED || beginDrawHR == DXGI_ERROR_DEVICE_RESET )
    {
        // recreate device and try to continue drawing
        CreateDeviceResources();
        BeginDraw( updateRect );
        return;
    }
    else
    {
        ThrowIfFailed( beginDrawHR );
    }

    // create render target from surface
    Microsoft::WRL::ComPtr< ID2D1Bitmap1 > bitmap;
    ThrowIfFailed( m_d2dContext->CreateBitmapFromDxgiSurface( surface.Get(), NULL, &bitmap ) );

    // set D2D drawing context as render target
    m_d2dContext->SetTarget( bitmap.Get() );

    m_d2dContext->BeginDraw();

    // handle the offset
    m_d2dContext->PushAxisAlignedClip( D2D1::RectF( static_cast< float >( offset.x ), static_cast< float >( offset.y ),
                                                    static_cast< float >( offset.x + updateRect.Width ), static_cast< float >( offset.y + updateRect.Height ) ),
                                       D2D1_ANTIALIAS_MODE_ALIASED );
    m_d2dContext->SetTransform( D2D1::Matrix3x2F::Translation( static_cast< float >( offset.x ), static_cast< float >( offset.y ) ) );
}
void DxImageSourceBase::EndDraw()
{
    m_d2dContext->PopAxisAlignedClip();

    ThrowIfFailed( m_d2dContext->EndDraw() );
    ThrowIfFailed( m_sisNative->EndDraw() );
}
void DxImageSourceBase::Clear( Windows::UI::Color color )
{
    m_d2dContext->Clear( ConvertUiColorToD2D1ColorF( color ) );
}
void DxImageSourceBase::CreateDeviceIndependentResources()
{
    ThrowIfFailed( reinterpret_cast< IUnknown* >( this )->QueryInterface( IID_PPV_ARGS( &m_sisNative ) ) );
}
void DxImageSourceBase::CreateDeviceResources()
{
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined( _DEBUG )
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
                                          D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2,
                                          D3D_FEATURE_LEVEL_9_1 };
    ThrowIfFailed( D3D11CreateDevice( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, featureLevels, ARRAYSIZE( featureLevels ),
                                      D3D11_SDK_VERSION, &m_d3dDevice, NULL, &m_d3dContext ) );
    Microsoft::WRL::ComPtr< IDXGIDevice > dxgiDevice;
    ThrowIfFailed( m_d3dDevice.As( &dxgiDevice ) );
    ThrowIfFailed( DxUtil::Instance()->GetD2DFactory()->CreateDevice( dxgiDevice.Get(), &m_d2dDevice ) );
    ThrowIfFailed( m_d2dDevice->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext ) );

    // TODO: SetDpi?  Sample was doing this...

    ThrowIfFailed( m_sisNative->SetDevice( dxgiDevice.Get() ) );
}

} // TsDX
