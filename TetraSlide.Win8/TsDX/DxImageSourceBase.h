#pragma once

#include "pch.h"

namespace TsDX
{

    public ref class DxImageSourceBase : Windows::UI::Xaml::Media::Imaging::SurfaceImageSource
    {
    internal:
        DxImageSourceBase::DxImageSourceBase( int pixelWidth, int pixelHeight, bool isOpaque );

    public:
        void BeginDraw( Windows::Foundation::Rect updateRect );
        void EndDraw();

        void Clear( Windows::UI::Color color );

    private protected:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

        Microsoft::WRL::ComPtr< ISurfaceImageSourceNative > m_sisNative;

        Microsoft::WRL::ComPtr< ID3D11Device > m_d3dDevice;
        Microsoft::WRL::ComPtr< ID3D11DeviceContext > m_d3dContext;

        Microsoft::WRL::ComPtr< ID2D1Device > m_d2dDevice;
        Microsoft::WRL::ComPtr< ID2D1DeviceContext > m_d2dContext;

        int m_pixelWidth;
        int m_pixelHeight;
    };

} // TsDX
