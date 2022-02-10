#pragma once

#include "pch.h"
#include "DxImageSourceBase.h"

namespace TsDX
{

    public ref class DxTiledBitmapImageSource sealed : DxImageSourceBase
    {
    internal:
        DxTiledBitmapImageSource( int pixelWidth, int pixelHeight, bool isOpaque,
                                  Microsoft::WRL::ComPtr< IWICBitmapSource > sourceImage );

    public:
        void BeginDraw();
        void RenderTiledImage();

    private:
        Microsoft::WRL::ComPtr< IWICBitmapSource > m_sourceImage;
    };

} // TsDX